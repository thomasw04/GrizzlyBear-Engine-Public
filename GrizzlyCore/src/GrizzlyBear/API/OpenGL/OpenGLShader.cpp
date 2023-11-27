#include <gbpch.h>

#include "OpenGLShader.h"

#include <fstream>
#include <gl/glew.h>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

//TODO rework shader
#include "GrizzlyBear/Environment/Environment.h"

#include "OpenGLBase.h"

namespace GrizzlyBear {

	static GLenum ShaderTypeFromString(const Gstring& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "compute")
			return GL_COMPUTE_SHADER;

		GRIZZLY_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const Gstring& name, const Gbuffer& source)
		: m_RendererID(0), m_Name(name)
	{
		if (!source.is_good())
		{
			return;
		}

		auto shaderSources = split_shader_types(source.m_Buffer);
		compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const Gstring& name, std::initializer_list<std::reference_wrapper<Gbuffer>> sources)
		: m_RendererID(0), m_Name(name)
	{
		std::vector<std::pair<GLenum, Gstring>> shaderSources;
		
		for (auto it = sources.begin(); it != sources.end(); it++)
		{
			shaderSources.push_back({ get_shader_type(it->get().m_Buffer), it->get().m_Buffer});
		}

		compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		GL_VALIDATE(glDeleteProgram(m_RendererID));
	}

	Gstring OpenGLShader::read_file(const Gstring& filepath)
	{
		Gstring result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Could not open file '{0}'", filepath.c_str());
		}

		return result;
	}

	GLenum OpenGLShader::get_shader_type(const Gstring& source)
	{
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		size_t begin = pos + typeTokenLength + 1;

		return ShaderTypeFromString(source.substr(begin, source.find_first_of("\r\n", pos) - begin));
	}

	std::vector<std::pair<GLenum, Gstring>> OpenGLShader::split_shader_types(const Gstring& source)
	{
		std::vector<std::pair<GLenum, Gstring>> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != Gstring::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			GRIZZLY_CORE_ASSERT(eol != Gstring::npos, "Syntax error.");
			size_t begin = pos + typeTokenLength + 1;
			Gstring type = source.substr(begin, eol - begin);
			GRIZZLY_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type.");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources.push_back({ ShaderTypeFromString(type), source.substr(nextLinePos, pos - (nextLinePos == Gstring::npos ? source.size() - 1 : nextLinePos)) });
		}

		return shaderSources;
	}

	void OpenGLShader::compile(const std::vector<std::pair<GLenum, Gstring>>& shaderSources)
	{
		GLuint program;
		GL_VALIDATE(program = glCreateProgram());
		std::vector<GLenum> shaderIDs;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const Gstring& source = kv.second;

			GLuint shader;
			GL_VALIDATE(shader = glCreateShader(type));

			const GLchar* sourceCStr = source.c_str();
			GL_VALIDATE(glShaderSource(shader, 1, &sourceCStr, 0));

			GL_VALIDATE(glCompileShader(shader));

			GLint isCompiled = 0;

			GL_VALIDATE(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;

				GL_VALIDATE(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));

				GRIZZLY_CORE_LOG(LogLevel::Error, "{0}", type);

				std::vector<GLchar> infoLog(maxLength);
				GL_VALIDATE(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));

				GL_VALIDATE(glDeleteShader(shader));

				GRIZZLY_CORE_LOG(LogLevel::Error, "{0}", infoLog.data());
				GRIZZLY_CORE_ASSERT(false, "Failed to compile Shader.");
				return;
			}

			GL_VALIDATE(glAttachShader(program, shader));
			shaderIDs.push_back(shader);

			if (type == GL_COMPUTE_SHADER && hasComputeShader)
			{
				GRIZZLY_CORE_LOG(LogLevel::Warn, "The compute shader of {0} has been overwritten.");
			}

			if (type == GL_COMPUTE_SHADER)
			{
				hasComputeShader = true;
			}
		}

		GL_VALIDATE(glLinkProgram(program));

		GLint isLinked = 0;
		GL_VALIDATE(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;

			GL_VALIDATE(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<GLchar> infoLog(maxLength);

			GL_VALIDATE(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));

			GL_VALIDATE(glDeleteProgram(program));

			for (auto id : shaderIDs)
			{
				GL_VALIDATE(glDeleteShader(id));
			}

			GRIZZLY_CORE_LOG(LogLevel::Error, "{0}", infoLog.data());
			GRIZZLY_CORE_ASSERT(false, "Failed to link Shader.");
			return;
		}

		for (auto id : shaderIDs)
		{
			GL_VALIDATE(glDetachShader(program, id));
		}

		m_RendererID = program;
	}

	void OpenGLShader::bind() const
	{
		GL_VALIDATE(glUseProgram(m_RendererID));
	}

	void OpenGLShader::unbind() const
	{
		GL_VALIDATE(glUseProgram(0));
	}

	void OpenGLShader::upload_uniform_int_array(const Gstring& name, int* values, int count)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniform1iv(location, count, values));
	}

	void OpenGLShader::upload_uniform_int(const Gstring& name, int values)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniform1i(location, values));
	}

	void OpenGLShader::upload_uniform_float_1(const Gstring& name, float values)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniform1f(location, values));
	}

	void OpenGLShader::upload_uniform_float_2(const Gstring& name, const glm::vec2& values)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniform2f(location, values.x, values.y));
	}

	void OpenGLShader::upload_uniform_float_3(const Gstring& name, const glm::vec3& values)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniform3f(location, values.x, values.y, values.z));
	}

	void OpenGLShader::upload_uniform_float_4(const Gstring& name, const glm::vec4& values)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniform4f(location, values.x, values.y, values.z, values.w));
	}

	void OpenGLShader::upload_uniform_mat_3(const Gstring& name, const glm::mat3& matrix)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void OpenGLShader::upload_uniform_mat_4(const Gstring& name, const glm::mat4& matrix)
	{
		GLint location = get_uniform_location(name);
		GL_VALIDATE(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void OpenGLShader::execute_compute(uint32_t x, uint32_t y, uint32_t z)
	{
		if (hasComputeShader)
			GL_VALIDATE(glDispatchCompute(x, y, z));
	}

	GLint OpenGLShader::get_uniform_location(const Gstring& name)
	{
		auto loc = m_UniformCache.find(name);

		if (loc != m_UniformCache.end())
		{
			return m_UniformCache.at(name);
		}

		GLint newEntry;
		GL_VALIDATE(newEntry = glGetUniformLocation(m_RendererID, name.c_str()));

		if (newEntry != -1)
			m_UniformCache.insert({ name, newEntry });

		return newEntry;
	}

}
