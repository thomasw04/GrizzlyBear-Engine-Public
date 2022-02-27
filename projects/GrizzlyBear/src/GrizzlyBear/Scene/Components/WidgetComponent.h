#pragma once

#include "BaseTypes.h"


namespace GrizzlyBear {

	struct CompWidget
	{
		CompWidget(const MaterialProps2D& props = MaterialProps2D(), const Gstring& texture = "")
			: m_Material(props), m_Texture(texture) {}

		GRIZZLY_REFLECT_VARS
		(
			(MaterialProps2D) m_Material,
			(Gstring) m_Texture
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompWidget)

}
