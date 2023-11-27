#pragma once

#include <glm/glm.hpp>

namespace GrizzlyBear {


	inline void conv_uint64_to_char(unsigned char c[8], uint64_t number)
	{
		c[7] = (unsigned char)(number);
		c[6] = (unsigned char)(number >> 8);
		c[5] = (unsigned char)(number >> 16);
		c[4] = (unsigned char)(number >> 24);

		c[3] = (unsigned char)(number >> 32);
		c[2] = (unsigned char)(number >> 40);
		c[1] = (unsigned char)(number >> 48);
		c[0] = (unsigned char)(number >> 56);
	}

	inline uint64_t conv_char_to_uint64(unsigned char c[8])
	{
		uint64_t returnInt = 0;

		returnInt |= c[0];
		returnInt = returnInt << 8 | c[1];
		returnInt = returnInt << 8 | c[2];
		returnInt = returnInt << 8 | c[3];

		returnInt = returnInt << 8 | c[4];
		returnInt = returnInt << 8 | c[5];
		returnInt = returnInt << 8 | c[6];
		returnInt = returnInt << 8 | c[7];

		return returnInt;
	}

	inline void conv_uint32_to_char(unsigned char c[4], uint32_t number)
	{
		c[0] = number;
		c[1] = number >> 8;
		c[2] = number >> 16;
		c[3] = number >> 24;
	}

	inline uint32_t conv_char_to_uint32(unsigned char c[4])
	{
		uint32_t returnInt = 0;

		returnInt |= c[0];
		returnInt = returnInt << 8 | c[1];
		returnInt = returnInt << 8 | c[2];
		returnInt = returnInt << 8 | c[3];

		return returnInt;
	}

	template<typename... Other>
	inline bool is_any_str(const Gstring& comp, const Gstring& first, Other... other)
	{
		return is_any(comp, first, std::forward((other)...));
	}

	template<typename Comp, typename First, typename... Other> 
	inline bool is_any(Comp compare, First first, Other... other)
	{
		if (compare == first) return true;

		return is_any(std::forward<const Gstring>((other)...));
	}


}
