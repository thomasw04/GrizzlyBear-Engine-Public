#pragma once

namespace GrizzlyBear {

	struct GCSTokens
	{
		static constexpr const char* TOKEN_PREFIX = "#";
		static constexpr const char* BEGIN_HEADER = "#HEADER_SECTION";
		static constexpr const char* BEGIN_BODY = "#BODY_SECTION";

		static constexpr const char* BEGIN_TEMPLATE_SECTION = "#TEMPLATE_SUB_SECTION";
		static constexpr const char* BEGIN_STATE_SECTION = "#STATE_SUB_SECTION";
		static constexpr const char* BEGIN_COMPONENT_SECTION = "#COMPONENT_SUB_SECTION";

		static constexpr int JSON_FORMAT = 2;
	};
}
