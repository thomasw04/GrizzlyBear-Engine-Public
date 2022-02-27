#pragma once

#include <random>
#include <unordered_map>

namespace GrizzlyBear {

	class Random
	{
	public:
		static void init();

		static float rand_float();

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

	class Guid
	{
	public:
		static void init();

		static uint64_t generate_id();

	private:
		static std::unordered_map<uint64_t, bool> s_Ids;

		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<uint64_t> s_Distribution;

	};

}