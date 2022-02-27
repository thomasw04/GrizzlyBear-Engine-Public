#include "gbpch.h"
#include "RenderUtils.h"

namespace GrizzlyBear {

	Ref<uint32_t[]> RenderUtils::generate_quad_indices(uint32_t objectCount)
	{
		uint32_t offset = 0;

		Ref<uint32_t[]> indices(new uint32_t[objectCount * 6]);

		for (uint32_t i = 0; i < objectCount * 6; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		return indices;
	}

	Ref<uint32_t[]> RenderUtils::generate_triangle_indices(uint32_t objectCount)
	{
		Ref<uint32_t[]> indices(new uint32_t[objectCount * 3]);

		for (uint32_t i = 0; i < 3 * objectCount; i++)
		{
			indices[i] = i;
		}

		return indices;
	}

	Ref<uint32_t[]> RenderUtils::generate_line_indices(uint32_t objectCount)
	{
		Ref<uint32_t[]> indices(new uint32_t[objectCount * 2]);

		for (uint32_t i = 0; i < 2 * objectCount; i++)
		{
			indices[i] = i;
		}

		return indices;
	}

	Ref<uint32_t[]> RenderUtils::generate_point_indices(uint32_t objectCount)
	{
		Ref<uint32_t[]> indices(new uint32_t[objectCount]);

		for (uint32_t i = 0; i < objectCount; i++)
		{
			indices[i] = i;
		}

		return indices;
	}
}


