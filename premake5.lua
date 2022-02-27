newoption {
	trigger       = "AppProject",
	value         = "APP",
	description   = "Choose the name of the application project (e.g. \"GrizzlyEditor\" for the default editor)",
	default = "GrizzlyEditor"
}

newoption {
	trigger       = "AppProjectLoc",
	value         = "LOC",
	description   = "Choose the location of the application project (e.g. \"projects/\" for the default location)",
	default       = "projects/"
}

newoption {
	trigger = "linuxWindowSys",
	value = "WINDOWSYSTEM",
	description = "Choose a linux window system for the engine (x11, wayland)",
	allowed = {
		{ "x11", "X11"},
		{ "wayland", "Wayland"}
	},
	default = "x11"
}

workspace "GrizzlyBear"
	architecture "x64"

	if _OPTIONS["generateEditor"] == "yes" then
		startproject "GrizzlyEditor"
	end

	configurations
	{
		"Debug",
		"Release"
	}

	outputdirTarget = "bin/%{cfg.buildcfg}/%{cfg.system}"
    outputdirObject = "bin-int/%{cfg.buildcfg}/%{cfg.system}"

	ProjectDir = {}
	ProjectDir["GLFW"] = "projects/GLFW/include"
	ProjectDir["Glad"] = "projects/Glad/include"
	ProjectDir["ImGui"] = "projects/ImGui"
	ProjectDir["LuaJit"] = "projects/LuaJit"

	HeaderDir = {}
	HeaderDir["stb_image"] = "projects/GrizzlyBear/libs/stb_image"

	SourceDir = {}
	SourceDir["stb_image"] = "projects/GrizzlyBear/libs/stb_image"

	include "projects/GLFW"
	include "projects/Glad"
	include "projects/ImGui/imgui"

project "GrizzlyBear"
	location "projects/%{prj.name}"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir (outputdirTarget .. "/%{prj.name}")
	objdir (outputdirObject .. "/%{prj.name}")

	files
	{
		"projects/%{prj.name}/src/**.h",
		"projects/%{prj.name}/src/**.cpp",

		"%{HeaderDir.stb_image}/**.h",

		"%{SourceDir.stb_image}/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	links
	{
			"GLFW",
			"Glad",
			"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		toolset "msc"

		pchheader "gbpch.h"
		pchsource "projects/GrizzlyBear/src/gbpch.cpp"

		includedirs
		{
			"projects/%{prj.name}/src",

			"%{ProjectDir.GLFW}",
			"%{ProjectDir.Glad}",
			"%{ProjectDir.ImGui}",
			"%{ProjectDir.ImGui}/imgui",
			"%{ProjectDir.LuaJit}",
			
			"%{HeaderDir.stb_image}",
			
			"libs/vcpkg/installed/x64-windows-static/include"
		}

		defines
		{
			"GLFW_INCLUDE_NONE",
			"GRIZZLY_PLATFORM_WINDOWS",
			"NOMINMAX"
		}

	filter "system:macosx"
		systemversion "latest"
		toolset "clang"

		pchheader "gbpch.h"
		pchsource "projects/GrizzlyBear/src/gbpch.cpp"
		
		sysincludedirs
		{
			"projects/%{prj.name}/src",

			"%{ProjectDir.GLFW}",
			"%{ProjectDir.Glad}",
			"%{ProjectDir.ImGui}",
			"%{ProjectDir.ImGui}/imgui",
			"%{ProjectDir.Lua}",
			
			"%{HeaderDir.stb_image}",

			"libs/vcpkg/installed/x64-macos-static/include"
		}

		defines
		{
		  "GLFW_INCLUDE_NONE",
		  "GRIZZLY_PLATFORM_MACOS"
		}

	filter "system:linux"
		systemversion "latest"
		toolset "gcc"

		pchheader "gbpch.h"
		pchsource "projects/GrizzlyBear/src/gbpch.cpp"

		includedirs
		{
			"projects/%{prj.name}/src",

			"%{ProjectDir.GLFW}",
			"%{ProjectDir.Glad}",
			"%{ProjectDir.ImGui}",
			"%{ProjectDir.ImGui}/imgui",
			"%{ProjectDir.LuaJit}",
			
			"%{HeaderDir.stb_image}",

			"libs/vcpkg/installed/x64-linux/include"
		}


		buildoptions
		{
			"-fPIC",
			"-fcoroutines"
		}

	filter {"system:linux", "options:linuxWindowSys=x11"}
		defines
		{
		  "GLFW_INCLUDE_NONE",
		  "GRIZZLY_PLATFORM_LINUX",
		  "GRIZZLY_PLATFORM_X11"
		}

	filter {"system:linux", "options:linuxWindowSys=wayland"}
		defines
		{
			"GLFW_INCLUDE_NONE",
			"GRIZZLY_PLATFORM_LINUX",
			"GRIZZLY_PLATFORM_WAYLAND"
		}

	filter "configurations:Debug"
		defines "GRIZZLY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GRIZZLY_RELEASE"
		runtime "Release"
		optimize "on"

project (_OPTIONS["AppProject"])
	location (_OPTIONS["AppProjectLoc"] .. "%{prj.name}")
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	targetdir (outputdirTarget .. "/%{prj.name}")
	objdir (outputdirObject .. "/%{prj.name}")

	files
	{
		"projects/%{prj.name}/src/**.h",
		"projects/%{prj.name}/src/**.cpp"
	}

	links
	{
		"GrizzlyBear",
		"GLFW",
		"Glad",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		toolset "msc"

		prebuildcommands
		{
			"copy \"%{wks.location}projects\\LuaJit\\luajit\\src\\lua51.dll\" \"%{wks.location}bin\\%{cfg.buildcfg}\\%{cfg.system}\\%{prj.name}\""
		}

		links {"opengl32.lib", "lua51.lib"}

		includedirs
		{
			"projects/GrizzlyBear/src",
			"projects/%{prj.name}/src",
			"%{ProjectDir.ImGui}",
			"%{ProjectDir.LuaJit}",
			"%{HeaderDir.stb_image}",
			"libs/vcpkg/installed/x64-windows-static/include"
		}
		
		libdirs {"projects/LuaJit/luajit/src"}

		defines
		{
		}

	filter {"system:windows", "configurations:Release"}
		libdirs { "libs/vcpkg/installed/x64-windows-static/lib/", "libs/vcpkg/installed/x64-windows-static/lib/manual-link/"}
		links {"libs/vcpkg/installed/x64-windows-static/lib/**.lib"}

	filter {"system:windows", "configurations:Debug"}
		libdirs { "libs/vcpkg/installed/x64-windows-static/debug/lib/", "libs/vcpkg/installed/x64-windows-static/debug/lib/manual-link/"}
		links {"libs/vcpkg/installed/x64-windows-static/debug/lib/**.lib"}

	filter "system:macosx"
		systemversion "latest"
		toolset "clang"
		links {"Cocoa.framework", "OpenGL.framework", "IOKit.framework", "CoreFoundation.framework", "CoreVideo.framework"}

		sysincludedirs
		{
			"projects/GrizzlyBear/src",
			"projects/%{prj.name}/src",
			"%{ProjectDir.ImGui}",
			"%{ProjectDir.LuaJit}",
			"%{HeaderDir.stb_image}",
			"libs/vcpkg/installed/x64-macos/include"
		}

		libdirs {"projects/LuaJit/luajit/src"}

		defines
		{
		}

	filter {"system:macosx", "configurations:Release"}
		libdirs { "libs/vcpkg/installed/x64-macos/lib/", "libs/vcpkg/installed/x64-macos/lib/manual-link/"}

	filter {"system:macosx", "configurations:Debug"}
		libdirs { "libs/vcpkg/installed/x64-macos/debug/lib/", "libs/vcpkg/installed/x64-macos/debug/lib/manual-link/"}

	filter "system:linux"
		systemversion "latest"
		toolset "gcc"

		prebuildcommands
		{
			"ln -f -s $$(readlink -f \"%{wks.location}/projects/LuaJit/luajit/build/lib/libluajit-5.1.so\") \"%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.system}/%{prj.name}/libluajit-5.1.so\"",
			"ln -f -s $$(readlink -f \"%{wks.location}/projects/LuaJit/luajit/build/lib/libluajit-5.1.so.2\") \"%{wks.location}/bin/%{cfg.buildcfg}/%{cfg.system}/%{prj.name}/libluajit-5.1.so.2\""
		}

		links
		{
			"dl",
			"X11", 
			"pthread",
			"GL",
			"boost_atomic", 
			"boost_chrono", 
			"boost_container",
			"boost_date_time",
			"boost_exception",
			"boost_system",
			"boost_filesystem",
			"boost_regex",
			"boost_thread",
			"boost_unit_test_framework",
			
			"luajit"
		}

		includedirs
		{
			"projects/GrizzlyBear/src",
			"projects/%{prj.name}/src",
			"%{ProjectDir.ImGui}",
			"%{ProjectDir.LuaJit}",
			"%{HeaderDir.stb_image}",
			"libs/vcpkg/installed/x64-linux/include"
		}
		
		libdirs {"projects/LuaJit/luajit/src"}
		
		buildoptions
		{
			"-fcoroutines"
		}

		defines
		{
		}

		linkoptions {"-Wl,-rpath='$$ORIGIN'"}

	filter {"system:linux", "configurations:Release"}
		libdirs { "libs/vcpkg/installed/x64-linux/lib/", "libs/vcpkg/installed/x64-linux/lib/manual-link/"}
		links { "fmt", "spdlog" }
	
	filter {"system:linux", "configurations:Debug"}
		libdirs { "libs/vcpkg/installed/x64-linux/debug/lib/", "libs/vcpkg/installed/x64-linux/debug/lib/manual-link/"}
		links { "fmtd", "spdlogd" }

	filter "configurations:Debug"
		defines "GRIZZLY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GRIZZLY_RELEASE"
		runtime "Release"
		optimize "on"
