#include <gbpch.h>

#include "TextureUtils.h"

namespace GrizzlyBear {

	void AtlasLayout::get_texture_coordinates(int index, std::array<float, 8>& textureCoords)
	{
		const AtlasElement& frame = m_Elements[index];

		/* Change coords from 0,0 = upper left and 1, 1 = lower right to 0, 0 = lower left and 1, 1 = upper right
		 *
		 * Vertices are supplied in following order:
		 *
		 *	4 - 3
		 *  |   |
		 *  1 - 2
		*/

		glm::vec2 rMin = { frame.get_min().x, 1 - frame.get_min().y };
		glm::vec2 rMax = { frame.get_max().x, 1 - frame.get_max().y };

		textureCoords[0] = rMin.x;
		textureCoords[1] = rMax.y;

		textureCoords[2] = rMax.x;
		textureCoords[3] = rMax.y;

		textureCoords[4] = rMax.x;
		textureCoords[5] = rMin.y;

		textureCoords[6] = rMin.x;
		textureCoords[7] = rMin.y;
	}

	void AtlasLayout::set_elements(const std::vector<AtlasElement>& elements)
	{
		m_Elements = elements;
	}

}


