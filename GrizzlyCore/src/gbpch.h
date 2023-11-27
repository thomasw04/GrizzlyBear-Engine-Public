#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdio.h>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <future>
#include <tuple>
#include <type_traits>

#include "GrizzlyBear/Core/Log.h"

#include <glm/glm.hpp>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	#define GRIZZLY_PLATFORM_WINDOWS	
#elif defined(__linux__) || defined(linux) || defined(__linux)
	#define GRIZZLY_PLATFORM_X11
#endif


#ifdef  GRIZZLY_PLATFORM_WINDOWS
	#include <Windows.h> 
	#include <commdlg.h>

	#undef INFINITE

#elif defined(GRIZZLY_PLATFORM_X11)
	#include <X11/Xlib.h>

	#undef Bool
	#undef None

#elif defined(GRIZZLY_PLATFORM_WAYLAND)
	#include <wayland-client.h>
#endif //  API

