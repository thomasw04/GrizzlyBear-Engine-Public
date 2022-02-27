#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GrizzlyBear/Core/BaseDefines.h>
#include <GrizzlyBear/API/GGL/Shader.h>
#include <GrizzlyBear/Core/Reflection/Reflector.h>

namespace GrizzlyBear {

	struct ComponentNode
	{
		uint32_t m_Children = 0;
	};

	//Module Render: Probably just some transfer classes to group large set of custom parameters
	struct MaterialProps2D
	{
		MaterialProps2D()
			: TINT({ 1.0f, 1.0f, 1.0f, 1.0f }), TILING_FACTOR(1.0f), HIGHLIGHT(0.0f),
			TEXTURE_COORDINATES({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }), STREAMABLE(false) {}

		GRIZZLY_REFLECT_VARS
		(
			(glm::vec4) TINT,
			(float) TILING_FACTOR,
			(float) HIGHLIGHT,
			(std::array<float, 8>) TEXTURE_COORDINATES,
			(Gstring) TEXTURE_FILEPATH,
			(bool) STREAMABLE
		)
	};

	struct Material3D
	{
		Material3D(Ref<Shader> shader)
			: m_Shader(shader)
		{
			shader->bind();
		}

		void SetTexture2D(Gstring texture)
		{
			m_Texture2D = texture;
		}

		void SetTint(const glm::vec4& tint)
		{
			m_Tint = tint;
		}

		
	private:
		Ref<Shader> m_Shader;

		Gstring m_Texture2D = "";
		glm::vec4 m_Tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct Material3DInstance
	{
		Material3DInstance(const Ref<Material3D> material) {}

	private:

	};


	//Module Entity: List of attachable components. (Make sure to specify them in EntityManager.h)
	//TODO: Make an seperate system for static objects with limited functionality.


	//Component guidlines:
	//Make sure to use pointers to "asset" objects or equivalent to minimize component size, otherwise it will destroy the advantage from cache friendliness.
	//Every component needs a default constructor.

	//Every component needs a serialization method with the signature: std::string serialize();
	//Every component needs a deserialization method with the signature : void deserialize(const std::string& object);

	//Sub components are only used in other components and are not registered in the ECS

}
