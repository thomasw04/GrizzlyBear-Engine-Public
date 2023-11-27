#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

namespace GrizzlyBear
{
	
	class Statistics
	{
	public:

		virtual ~Statistics() = default;
		virtual uint16_t get_max_texture_units() = 0;
		virtual double get_time() = 0;

	protected:

		uint16_t m_MaxTextureUnits = 0;
		double m_LastFrameTime = 0.0;
	};

}
