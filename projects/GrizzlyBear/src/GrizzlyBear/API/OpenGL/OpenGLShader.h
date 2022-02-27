#pragma once

#include "GrizzlyBear/API/GGL/Shader.h"

#include <vector>

typedef unsigned int GLenum;
typedef int GLint;

namespace GrizzlyBear {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const Gstring& name, const Gbuffer& source);
		OpenGLShader(const Gstring& name, std::initializer_list<std::reference_wrapper<Gbuffer>> sources);

		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const Gstring& get_name() const override { return m_Name; }

		virtual void upload_uniform_int(const Gstring& name, int values) override;
		virtual void upload_uniform_int_array(const Gstring& name, int* values, int count) override;

		virtual void upload_uniform_float_1(const Gstring& name, float values) override;
		virtual void upload_uniform_float_2(const Gstring& name, const glm::vec2& values) override;
		virtual void upload_uniform_float_3(const Gstring& name, const glm::vec3& values) override;
		virtual void upload_uniform_float_4(const Gstring& name, const glm::vec4& values) override;

		virtual void upload_uniform_mat_3(const Gstring& name, const glm::mat3& matrix) override;
		virtual void upload_uniform_mat_4(const Gstring& name, const glm::mat4& matrix) override;

		virtual void execute_compute(uint32_t x, uint32_t y, uint32_t z) override;

	private:
		Gstring read_file(const Gstring& filepath);

		GLenum get_shader_type(const Gstring& source);
		std::vector<std::pair<GLenum, Gstring>> split_shader_types(const Gstring& source);
		void compile(const std::vector<std::pair<GLenum, Gstring>>& shaderSources);

		GLint get_uniform_location(const Gstring& name);

	private:
		uint32_t m_RendererID;
		Gstring m_Name;

		std::unordered_map<Gstring, GLint> m_UniformCache;
		bool hasComputeShader = false;
	};

}
