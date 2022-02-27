#pragma once

#include "BaseTypes.h"

namespace GrizzlyBear {

	struct CompScript
	{
		CompScript(const Gstring& filepath = "")
			: FILE_PATH(filepath) {}

		GRIZZLY_REFLECT_VARS
		(
			(Gstring) FILE_PATH
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompScript)
}
