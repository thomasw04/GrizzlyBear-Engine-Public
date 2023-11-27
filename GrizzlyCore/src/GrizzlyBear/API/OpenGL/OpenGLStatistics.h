#pragma once

#include "GrizzlyBear/API/GGL/Statistics.h"

namespace GrizzlyBear {


	class OpenGLStatistics : public Statistics
	{
	public:
		virtual uint16_t get_max_texture_units() override;
		virtual double get_time() override;
	};


}
