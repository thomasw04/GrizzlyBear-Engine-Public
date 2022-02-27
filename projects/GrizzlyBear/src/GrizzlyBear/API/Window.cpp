#include <gbpch.h>

#include "Window.h"

#include "GrizzlyBear/API/OpenGL/OpenGLWindow.h"

namespace GrizzlyBear {

	Ref<Window> Window::create(const Gstring& title, uint32_t width, uint32_t height)
	{
		#ifdef GRIZZLY_PLATFORM_WINDOWS
			return CreateRef<OpenGLWindow>(title, width, height);
		#elif defined(GRIZZLY_PLATFORM_LINUX)
			return CreateRef<OpenGLWindow>(title, width, height);	
		#else
			GRIZZLY_CORE_LOG(LogLevel::Fatal, "This platform is currently not supported!");
			return nullptr;	
		#endif
	}

}
