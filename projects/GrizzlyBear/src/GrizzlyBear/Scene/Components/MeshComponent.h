#pragma once

#include "BaseTypes.h"

#include "GrizzlyBear/Misc/RenderUtils.h"

namespace GrizzlyBear {

	struct SubAnimatedTexture
	{
		SubAnimatedTexture()
			: m_Animation(), m_Attached(false) {}

		SubAnimatedTexture(Animation2D animation)
			: m_Animation(animation), m_Attached(true) {}

		GRIZZLY_REFLECT_VARS
		(
			(Animation2D) m_Animation,
			(bool) m_Attached
		)
	};

	struct CompMesh2D
	{
		CompMesh2D() {}
		CompMesh2D(const MaterialProps2D& props) : m_Material(props) {}

		GRIZZLY_REFLECT_VARS
		(
			(MaterialProps2D) m_Material
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompMesh2D)

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
