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

#ifdef  GRIZZLY_PLATFORM_WINDOWS
	#include <Windows.h> 
	#include <commdlg.h>

#elif defined(GRIZZLY_PLATFORM_X11)
	#include <X11/Xlib.h>

	#undef Bool
	#undef None

#elif defined(GRIZZLY_PLATFORM_WAYLAND)
	#include <wayland-client.h>
#endif //  API

