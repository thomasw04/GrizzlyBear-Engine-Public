#pragma once

#include "glm/glm.hpp"

namespace GrizzlyBear {


	class Phys75
	{
	public:

		//For orthogonal Matrices
		static glm::vec2 screen_to_world(const glm::vec2& mousePos, const glm::vec2& screenBounds, const glm::vec2& cameraPosition, float aspectRatio, float zoomLevel);
		static glm::vec2 screen_to_world(const glm::vec2& mousePos, const glm::vec2& screenBounds, const glm::vec2& cameraBounds, const glm::vec2& cameraPosition);


	private:

	};

}
