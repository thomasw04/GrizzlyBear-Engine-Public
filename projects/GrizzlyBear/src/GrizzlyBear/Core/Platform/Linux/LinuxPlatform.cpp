#include <gbpch.h>

#ifdef GRIZZLY_PLATFORM_LINUX

#include "LinuxPlatform.h"

#include "GrizzlyBear/Core/Application.h"

#include <linux/limits.h>
#include <unistd.h>

#include <GLFW/glfw3.h>

namespace GrizzlyBear {

	static void GLFWErrorCallback(int error, const char* description)
	{
		GRIZZLY_CORE_LOG(LogLevel::Error, "Module: GLFW, Message: ({0}): {1}", error, description);
	}

	LinuxPlatform::LinuxPlatform()
	{
		int success = glfwInit();
		GRIZZLY_CORE_ASSERT(success, "Could not initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);

		#ifdef GRIZZLY_PLATFORM_X11
			Display* d = XOpenDisplay(NULL);
			Screen* s = DefaultScreenOfDisplay(d);
			m_Width = s->width;
			m_Height = s->height;
		#elif defined(GRIZZLY_PLATFORM_WAYLAND)
			#error "I have no clue how wayland is working"
		#endif
	}

	LinuxPlatform::~LinuxPlatform()
	{

	}

	bool LinuxPlatform::get_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt)
	{
		GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Platform, Message: Unable to run action. (Not implemented)");
		return false;
	}

	bool LinuxPlatform::save_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt)
	{
		GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Platform, Message: Unable to run action. (Not implemented)");
		return false;
	}

	int LinuxPlatform::get_display_width_impl() const
	{
		return m_Width;
	}

	int LinuxPlatform::get_display_height_impl() const
	{
		return m_Height;
	}

	Gstring LinuxPlatform::get_executable_path_impl() const
	{
		char result[PATH_MAX];
		size_t count = readlink("/proc/self/exe", result, PATH_MAX);
		Gstring s = Gstring(result, count);
		return s;
	}

	bool LinuxPlatform::is_absolute_filepath_impl(const Gstring& path) const
	{
		if (path[0] == '/')
		{
			return true;
		}

		return false;
	}

	bool LinuxPlatform::is_valid_filepath_impl(const Gstring& path) const
	{
		if (path.empty()) return false;

		for (int i = 0; i < path.size(); i++)
		{
			//Filter linux reserved characters, 
			if (path[i] == '\0')
			{
				return false;
			}
		}

		return true;
	}
}

#endif
