#pragma once

#include "BaseTypes.h"

#include <glm/glm.hpp>

namespace GrizzlyBear {

	struct CompTransformation2D : public ComponentNode
	{
		CompTransformation2D(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec2& size = { 0.0f, 0.0f },
							 float rotation = 0.0f)
		: m_Position(position), m_Size(size), m_Rotation(rotation) {}

		GRIZZLY_REFLECT_VARS
		(
			(glm::vec3) m_Position,
			(glm::vec2) m_Size,
			(float) m_Rotation
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompTransformation2D)

	struct CompTransformation3D : public ComponentNode
	{
		CompTransformation3D(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec3& size = {0.0f, 0.0f, 0.0f},
							 const glm::vec3& rotation = {0.0f, 0.0f, 0.0f})
		: m_Position(position), m_Size(size), m_Rotation(rotation) {}

		GRIZZLY_REFLECT_VARS
		(
			(glm::vec3) m_Position,
			(glm::vec3) m_Size,
			(glm::vec3) m_Rotation
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompTransformation3D)
}
