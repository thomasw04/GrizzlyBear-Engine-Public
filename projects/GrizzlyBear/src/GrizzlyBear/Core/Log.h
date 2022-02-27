#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/ringbuffer_sink.h"
#pragma warning(pop)

namespace GrizzlyBear {

	#define GRIZZLY_CORE_LOG(loglevel, ...) ::GrizzlyBear::Log::core_log(loglevel, __VA_ARGS__)
	#define GRIZZLY_CLIENT_LOG(loglevel, ...) ::GrizzlyBear::Log::client_log(loglevel, __VA_ARGS__)

	#ifdef GRIZZLY_ENABLE_ASSERTS
		#define GRIZZLY_CORE_ASSERT(x, ...) { if(!(x)) { GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Assertion Failed: {0}", __VA_ARGS__); GRIZZLY_BREAKPOINT(); } }
	#else
		#define GRIZZLY_CORE_ASSERT(x, ...)
	#endif

	enum class LogLevel
	{
		Trace = 0, Info, Warn, Error, Fatal
	};

	class Log
	{
	public:
		static ModuleState init();

		static void post_init(const Gstring& appName);

		static void shutdown();

		static bool is_init() { return s_Init; };

		static void set_console_filter(LogLevel level);

		template<typename FormatString, typename... Args>
		static inline void lua_error_log(const FormatString& fmt, const Args &... args)
		{
			std::stringstream sstream;
			sstream << "Module: Lua Scripts, In file: {0}, At line: {1}, Message: " << fmt;
			s_CoreLogger.error(fmt::runtime(sstream.str()), args...);
		}

		static inline void core_log_module_state(Gstring name, ModuleState state)
		{
			Gstring text = "Module: ";
			text.append(name).append(", State: ");

			if (state == ModuleState::GOOD) s_CoreLogger.info(text.append("Good"));
			else if (state == ModuleState::ERROR_USABLE) s_CoreLogger.warn(text.append("Non-critical error"));
			else if (state == ModuleState::ERROR_RISKY) s_CoreLogger.error(text.append("Error"));
			else if (state == ModuleState::ERROR_FATAL) s_CoreLogger.critical(text.append("Fatal"));
		}

		template<typename FormatString, typename... Args>
		static inline void core_log(LogLevel level, FormatString fomt, const Args &... args)
		{
			if (level == LogLevel::Trace) s_CoreLogger.trace(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Info) s_CoreLogger.info(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Warn) s_CoreLogger.warn(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Error) s_CoreLogger.error(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Fatal) s_CoreLogger.critical(fmt::runtime(fomt), args...);
		}

		template<typename FormatString, typename... Args>
		static inline void client_log(LogLevel level, FormatString fomt, const Args &... args)
		{
			if (level == LogLevel::Trace) s_ClientLogger.trace(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Info) s_ClientLogger.info(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Warn) s_ClientLogger.warn(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Error) s_ClientLogger.error(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Fatal) s_ClientLogger.critical(fmt::runtime(fomt), args...);
		}

	private:
		static bool s_Init;

		static spdlog::logger s_CoreLogger;
		static spdlog::logger s_ClientLogger;

		static Gstring s_LogDir;
	};

}