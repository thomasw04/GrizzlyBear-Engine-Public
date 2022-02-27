#pragma once

#include "BaseTypes.h"

namespace GrizzlyBear {

	struct CompCamera
	{
		CompCamera(const glm::mat4& projection = glm::mat4(1.0f))
			: m_ProjectionMatrix(projection) {}

		GRIZZLY_REFLECT_VARS
		(
			(glm::mat4) m_ProjectionMatrix
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompCamera)
}
