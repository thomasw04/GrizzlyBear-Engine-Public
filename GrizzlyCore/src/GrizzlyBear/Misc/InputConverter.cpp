#include <gbpch.h>

#include "InputConverter.h"

namespace GrizzlyBear {

	glm::vec3 InputConverter::screen_to_world(const glm::vec2& screenCoords, const glm::vec2& resolution, const glm::mat4& viewProjection)
	{
		glm::mat4 vpInverse = glm::inverse(viewProjection);

		glm::vec4 coords;

		coords.x = (2.0f * (screenCoords.x / resolution.x)) - 1.0f;
		coords.y = 1.0f - (2.0f * (screenCoords.y / resolution.y));
		coords.z = 1.0f;
		coords.w = 1.0f;

		coords = coords * vpInverse;

		coords.w = 1.0f / coords.w;

		coords *= glm::vec4(coords.w, coords.w, coords.w, 1.0f);

		return glm::vec3(coords.x, coords.y, coords.z);
	}
}
