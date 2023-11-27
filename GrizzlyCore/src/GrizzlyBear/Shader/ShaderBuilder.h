#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include <GrizzlyBear/API/GGL/Shader.h>
#include "GrizzlyBear/API/GGL/Buffer.h"

namespace GrizzlyBear {


	class ShaderData
	{
		
	};

	class ShaderPrototype
	{
		//TODO No position data hint etc.

		ShaderPrototype();

		void create_slot(const Gstring& name, ShaderDataType type, bool normalized = false);
		void create_uniform(const Gstring& name, ShaderDataType type); 
		
		Ref<ShaderData> create_shader_data();
		Ref<Shader> create_shader();
	};

	class Material
	{
		friend class ShaderBuilder;

	public:

		enum class ShaderType
		{
			Vertex = 0, Geometry, Fragment
		};

		//Material(const std::initializer_list<BufferElement>& elements);

		bool set_uniform(ShaderType shaderType, ShaderDataType dataType, const Gstring& name);
		void remove_uniform(ShaderType shaderType, const Gstring& name);

	private:
		struct UniformElement
		{
			ShaderType m_ShaderType;
			ShaderDataType   m_DataType;
			Gstring    m_Name;
		};

		Gstring m_MaterialName;

		std::vector<UniformElement> m_Uniforms;
		Ref<BufferLayout> m_Layout;

	private:

		
	};

	class ShaderBuilder
	{
		const Gstring& BuildShader(const Ref<Material> material);

	private:

		//TODO: make api agnostic

		static std::string DataTypeToString(ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::None:       break;
				case ShaderDataType::Float:	    return std::string("float");
				case ShaderDataType::Float2:	return std::string("vec2");
				case ShaderDataType::Float3:	return std::string("vec3");
				case ShaderDataType::Float4:	return std::string("vec4");
				case ShaderDataType::Mat3:		return std::string("mat3");
				case ShaderDataType::Mat4:		return std::string("mat4");
				case ShaderDataType::Int:		return std::string("int");
				case ShaderDataType::Int2:		return std::string("ivec2");
				case ShaderDataType::Int3:		return std::string("ivec3");
				case ShaderDataType::Int4:		return std::string("ivec4");
				case ShaderDataType::Bool:		return std::string("bool");
			}

			GRIZZLY_CORE_ASSERT(false, "Unknown data type!");
			return std::string("UNKNOWN");
		}

	private:

		


	};


}
