#pragma once

#include <glm/glm.hpp>

namespace GrizzlyBear {

	class InputConverter
	{
	public:
		static glm::vec3 screen_to_world(const glm::vec2& screenCoords, const glm::vec2& resolution, const glm::mat4& viewProjection);
	};
}
