project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
    
	targetdir (outputdirTarget .. "/%{prj.name}")
   	objdir (outputdirObject .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
    	}

	includedirs
	{
		"include"
	}
    
	filter "system:windows"
		systemversion "latest"
		toolset "msc"
		
	filter "system:macosx"
		systemversion "latest"
		toolset "clang"

		sysincludedirs { "include" }
	
	filter "system:linux"
		systemversion "latest"
		toolset "gcc"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        
	