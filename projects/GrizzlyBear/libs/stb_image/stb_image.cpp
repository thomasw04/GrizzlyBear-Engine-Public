#include <gbpch.h>

#define STB_IMAGE_IMPLEMENTATION

#ifdef GRIZZLY_DEBUG
#define STBI_NO_SIMD
#endif

#include "stb_image.h"