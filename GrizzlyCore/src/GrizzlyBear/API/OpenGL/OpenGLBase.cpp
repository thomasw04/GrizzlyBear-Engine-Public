#include <gbpch.h>

#include "OpenGLBase.h"

#include <GrizzlyBear/Core/Log.h>

#include <gl/glew.h>

namespace GrizzlyBear {

	void print_opengl_error(const char* call, const char* file, int line)
	{
		GLenum error = glGetError();

		while (error != GL_NO_ERROR)
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "OpenGL call error: {0}, Code: {1} in {2} at Line {3}", call, error, file, line);
		}
	}
}