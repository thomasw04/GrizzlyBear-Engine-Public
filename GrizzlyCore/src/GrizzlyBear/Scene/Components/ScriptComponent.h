#pragma once

#include "BaseTypes.h"

namespace GrizzlyBear {

	struct CompScript
	{
		CompScript(bool native = false, const Gstring& filepath = "")
			: NATIVE(native), FILE_PATH(filepath) {}

		GRIZZLY_REFLECT_VARS
		(
			(bool) NATIVE,
			(Gstring) FILE_PATH
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(CompScript)
}
