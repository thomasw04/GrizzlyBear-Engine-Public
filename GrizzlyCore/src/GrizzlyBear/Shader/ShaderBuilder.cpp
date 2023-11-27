#include <gbpch.h>

#include "ShaderBuilder.h"

namespace GrizzlyBear {

	/*bool Material::add_uniform(ShaderType shaderType, ShaderDataType dataType, const Gstring& name)
	{
		for (const UniformElement& element : m_Uniforms)
		{
			if (element.m_Name == name)
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Material_{0}: The uniform {1} is already registered", m_MaterialName, name);
				return false;
			}
		}

		m_Uniforms.push_back({ shaderType, dataType, name });
		return true;
	}

	const Gstring& ShaderBuilder::BuildShader(const Ref<Material> material)
	{
		std::string source;*/

		/*Append header
		 * #type vertex
		 * #version 330 core
		 * 
		*/
		/*source.append("#type vertex\n#version330 core\n\n");

		int counter = 0;
		//Append buffer inputs
		for (auto it = material->m_Layout->begin(); it != material->m_Layout->end(); it++)
		{
			source.append(std::string("layout(location = ") + std::to_string(counter) + std::string(") in ") + DataTypeToString(it->m_Type) + std::string(" ") + it->m_Name + std::string(";\n"));
			counter++;
		}

		for (const Material::UniformElement& uniform : material->m_Uniforms)
		{
			source.append(std::string("uniform ") + DataTypeToString(uniform.m_DataType) + std::string(" ") + uniform.m_Name + std::string(";\n"));
		}

		return std::string();
	}*/

	/*
	* bufferLayout = CreateRef<BufferLayout>(std::initializer_list<BufferElement>{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TextureID" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Float, "a_Highlight" }
		});

		Material material(bufferLayout);

		//Prebuild uniforms
		material.add_uniform(ShaderType::Vertex, DataType::mat4, "u_ViewProjection");
		material.add_uniform(ShaderType::Fragment, DataType::int_array, "u_Textures[32]");
		
		//Custom uniforms
		material.add_uniform(ShaderType::Fragment, DataType::float, "u_Roughness");

		//Prebuild shader functions
		material.set_color(...);
		material.set_texture(...);
		material.set_roughness(...);
		
		...

		//Custom shader functions
		material.set_snippet(ShaderType::Fragment, const std::string& snippet);
	

		Shader shader = ShaderBuilder::Build(material);
	*/

}



