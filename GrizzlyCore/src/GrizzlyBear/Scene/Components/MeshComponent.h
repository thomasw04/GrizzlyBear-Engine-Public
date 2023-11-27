#pragma once

#include "BaseTypes.h"

#include "GrizzlyBear/Misc/RenderUtils.h"

namespace GrizzlyBear {

	struct CompSprite
	{
		CompSprite() : HUD(false) {}
		CompSprite(const MaterialProps2D& props, bool hud = false) : MATERIAL(props), HUD(hud) {}

		GRIZZLY_REFLECT_VARS
		(
			(MaterialProps2D) MATERIAL,
			(bool) HUD
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompSprite)

	struct CompMesh3D
	{
		CompMesh3D()
			: m_Vertices(), m_Indices() {}

		CompMesh3D(std::vector<float>& vertices, std::vector<int>& indices)
			: m_Vertices(vertices), m_Indices(indices) {}
	
		GRIZZLY_REFLECT_VARS
		(
			(std::vector<float>) m_Vertices,
			(std::vector<int>) m_Indices
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompMesh3D)
}
