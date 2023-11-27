#pragma once

#include "BaseTypes.h"

#include <glm/glm.hpp>

namespace GrizzlyBear {

	struct CompTransformation : public ComponentNode
	{
		CompTransformation(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, const glm::vec3& size = {0.0f, 0.0f, 0.0f},
							 const glm::vec3& rotation = {0.0f, 0.0f, 0.0f})
		: m_Position(position), m_Size(size), m_Rotation(rotation) {}

		CompTransformation(const glm::vec3& position, const glm::vec2& size,
			float rotation)
			: m_Position(position), m_Size(size.x, size.y, 1.0f), m_Rotation(rotation, 0.0f, 0.0f) {}

		GRIZZLY_REFLECT_VARS
		(
			(glm::vec3) m_Position,
			(glm::vec3) m_Size,
			(glm::vec3) m_Rotation
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompTransformation)
}
