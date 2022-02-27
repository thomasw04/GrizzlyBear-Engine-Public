#include <gbpch.h>

#include "Phys75.h"

namespace GrizzlyBear {

	glm::vec2 Phys75::screen_to_world(const glm::vec2& mousePos, const glm::vec2& screenBounds, const glm::vec2& cameraPosition, float aspectRatio, float zoomLevel)
	{
		float x = mousePos.x;
		float y = mousePos.y;

		glm::vec2 bounds;

		bounds.x = (aspectRatio * zoomLevel) + (aspectRatio * zoomLevel);
		bounds.y = (zoomLevel + zoomLevel);

		x = bounds.x * (x / screenBounds.x) - 0.5f * bounds.x;
		y = bounds.y * 0.5f - (y / screenBounds.y) * bounds.y;

		return { x + cameraPosition.x, y + cameraPosition.y };
	}

	glm::vec2 Phys75::screen_to_world(const glm::vec2& mousePos, const glm::vec2& screenBounds, const glm::vec2& cameraBounds, const glm::vec2& cameraPosition)
	{
		float x = mousePos.x;
		float y = mousePos.y;

		x = cameraBounds.x * (x / screenBounds.x) - 0.5f * cameraBounds.x;
		y = cameraBounds.y * 0.5f - (y / screenBounds.y) * cameraBounds.y;

		return { x + cameraPosition.x, y + cameraPosition.y };
	}

}
