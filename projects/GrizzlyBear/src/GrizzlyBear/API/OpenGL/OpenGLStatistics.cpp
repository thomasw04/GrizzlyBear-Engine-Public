#include <gbpch.h>
#include "OpenGLStatistics.h"

#include "OpenGLBase.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace GrizzlyBear {


	uint16_t OpenGLStatistics::get_max_texture_units()
	{
		if (Statistics::m_MaxTextureUnits != 0)
		{
			return Statistics::m_MaxTextureUnits;
		}

		GLint returnValue;
		GL_VALIDATE(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &returnValue));

		return (Statistics::m_MaxTextureUnits = returnValue);
	}

	double OpenGLStatistics::get_time()
	{
		return glfwGetTime();
	}

}