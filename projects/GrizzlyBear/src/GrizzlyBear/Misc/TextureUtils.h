#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "GrizzlyBear/Core/Reflection/Reflector.h"

namespace GrizzlyBear {

	struct AtlasElement
	{
		const glm::vec2& get_min() const { return m_MinPixel; }
		const glm::vec2& get_max() const { return m_MaxPixel; }

		GRIZZLY_REFLECT_VARS
		(
			(glm::vec2) m_MinPixel,
			(glm::vec2) m_MaxPixel
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(AtlasElement)

	class AtlasLayout
	{
	public:
		AtlasLayout()
			: m_Elements() {}

		AtlasLayout(const std::initializer_list<AtlasElement>& elements)
			: m_Elements(elements) {}

		const AtlasElement* get_element(uint32_t index) { return &m_Elements[index]; }

		size_t get_size() { return m_Elements.size(); }

		void get_texture_coordinates(int index, std::array<float, 8>& textureCoords);

		void set_elements(const std::vector<AtlasElement>& elements);

	private:
		GRIZZLY_REFLECT_VARS
		(
			(std::vector<AtlasElement>) m_Elements
		)
	};

}
