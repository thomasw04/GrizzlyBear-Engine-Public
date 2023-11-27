#include <gbpch.h>

#include <time.h>

#include "Platform.h"

#include "GrizzlyBear/Core/Platform/Windows/WindowsPlatform.h"
#include "GrizzlyBear/Core/Platform/Linux/LinuxPlatform.h"

namespace GrizzlyBear {
	
	Ref<Platform> Platform::m_Platform = nullptr;

	ModuleInfo Platform::init()
	{
		GRIZZLY_CORE_LOG(LogLevel::Trace, "Init platform module...");

		#ifdef GRIZZLY_PLATFORM_WINDOWS
			m_Platform = CreateRef<WindowsPlatform>();
			return m_Platform->get_info();
		#elif defined(GRIZZLY_PLATFORM_LINUX)
			m_Platform = CreateRef<LinuxPlatform>();
			return m_Platform->get_info();
		#else
			return ModuleInfo{ ModuleState::ERROR_FATAL };
		#endif
	}

	Gstring Platform::get_date(const char* format)
	{
		time_t now = time(0);
		char buffer[100];

		if (!std::strftime(buffer, sizeof(buffer), format, std::localtime(&now)))
		{
			return "UNKNOWN-TIME";
		}

		return buffer;
	}

}
