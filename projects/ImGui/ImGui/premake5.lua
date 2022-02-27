project "ImGui"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir (outputdirTarget .. "/%{prj.name}")
	objdir (outputdirObject .. "/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
        "imgui_tables.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp",
		"misc/cpp/imgui_stdlib.h",
		"misc/cpp/imgui_stdlib.cpp"
	}

	includedirs
	{
		"%{prj.location}"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		toolset "msc"

	filter "system:macosx"
		systemversion "latest"
		cppdialect "C++17"
		toolset "clang"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		toolset "gcc"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "Off"

	filter "configurations:Release"
		runtime "Release"
		optimize "Off"
