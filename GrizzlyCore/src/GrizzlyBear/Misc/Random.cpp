#include <gbpch.h>

#include "Random.h"

namespace GrizzlyBear {

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

	void Random::init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	float Random::rand_float()
	{
		//Supress max macro defined in Windows Kit with parenthesis
		return (float)s_Distribution(s_RandomEngine) / (float)(std::numeric_limits<uint32_t>::max)();
	}

	std::mt19937 Guid::s_RandomEngine;
	std::uniform_int_distribution<uint64_t> Guid::s_Distribution;
	std::unordered_map<uint64_t, bool> Guid::s_Ids;

	void Guid::init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	uint64_t Guid::generate_id()
	{
		bool running = true;
		uint64_t id;

		while (running)
		{
			id = s_Distribution(s_RandomEngine);
			if (!s_Ids[id] && id != 0) running = false;
		}

		s_Ids[id] = true;

		return id;
	}
}
