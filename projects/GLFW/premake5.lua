project "GLFW"
    kind "StaticLib"
    language "C"
	staticruntime "on"
    
	targetdir (outputdirTarget .. "/%{prj.name}")
    objdir (outputdirObject .. "/%{prj.name}")

	files
	{
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c"
    }
    
	filter "system:windows"
        systemversion "latest"
		toolset "msc"
        
        files
        {
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_time.c",
            "src/win32_thread.c",
            "src/win32_window.c",
            "src/wgl_context.c",
            "src/egl_context.c",
            "src/osmesa_context.c"
        }

		defines 
		{ 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}
		
	filter "system:macosx"
		systemversion "latest"
		toolset "clang"

		files
		{
			"src/egl_context.c",
    		"src/osmesa_context.c",
			"src/nsgl_context.m",
			"src/cocoa_init.m",
			"src/cocoa_joystick.m",
			"src/cocoa_monitor.m",
			"src/cocoa_time.c",
			"src/posix_thread.c",
			"src/cocoa_window.m"
		}

		defines
		{
			"_GLFW_COCOA",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:linux"
		systemversion "latest"
		toolset "gcc"

		files { "src/linux_joystick.h", "src/linux_joystick.c" }

	filter {"system:linux", "options:linuxWindowSys=x11"}
		files
		{
			"src/x11_platform.h",
			"src/xkb_unicode.h",
			"src/posix_time.h",
			"src/posix_thread.h",
			"src/glx_context.h",
			"src/egl_context.h",
			"src/osmesa_context.h",
			
			"src/x11_init.c",
			"src/x11_monitor.c",
			"src/x11_window.c",
			"src/xkb_unicode.c",
			"src/posix_time.c",
			"src/posix_thread.c",
			"src/glx_context.c",
			"src/egl_context.c",
			"src/osmesa_context.c"
		}
		defines
		{
			"_GLFW_X11",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter {"system:linux", "options:linuxWindowSys=wayland"}
		files
		{
			"src/wl_platform.h",
			"src/posix_time.h",
			"src/posix_thread.h",
			"src/xkb_unicode.h",
			"src/egl_context.h",
			"src/wl_init.c",
			"src/wl_monitor.c",
			"src/wl_window.c",
			"src/posix_time.c",
			"src/posix_thread.c",
			"src/xkb_unicode.c",
			"src/egl_context.c",
			"src/osmesa_context.c"
		}
		defines
		{
			"_GLFW_WAYLAND",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"