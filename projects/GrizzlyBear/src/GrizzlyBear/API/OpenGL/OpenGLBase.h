#pragma once

namespace GrizzlyBear {

	void print_opengl_error(const char* call, const char* file, int line);

}

#ifdef GRIZZLY_DEBUG
	#define GL_VALIDATE(x) \
		x;                 \
		print_opengl_error(#x, __FILE__, __LINE__)
#else
	#define GL_VALIDATE(x) x
#endif
