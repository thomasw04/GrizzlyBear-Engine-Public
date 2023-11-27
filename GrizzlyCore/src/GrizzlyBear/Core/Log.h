#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include <iostream>

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
		static ModuleInfo init();

		static void post_init(const Gstring& appName);

		static void shutdown();
		static void panic();

		static bool is_init() { return s_Init; };

		static void trigger_reload();

		static void set_console_filter(LogLevel level);

		template<typename FormatString, typename... Args>
		static inline void lua_error_log(const FormatString& fmt, const Args &... args)
		{
			std::stringstream sstream;
			sstream << "Module: Lua Scripts, In file: {0}, At line: {1}, Message: " << fmt;
			s_CoreLogger.error(fmt::runtime(sstream.str()), args...);
		}

		static inline void core_log_module_state(Gstring name, ModuleInfo info)
		{
			Gstring text = "Module: ";
			text.append(name).append(", State: ");

			if (s_Info.STATE <= ModuleState::ERROR_USABLE)
			{
				if (info.STATE == ModuleState::GOOD)         s_CoreLogger.info(text.append("Good"));
				if (info.STATE == ModuleState::ERROR_USABLE) s_CoreLogger.warn(text.append("Non-critical error: ").append(info.WHAT));
				if (info.STATE == ModuleState::ERROR_RISKY)  s_CoreLogger.error(text.append("Error: ").append(info.WHAT));
				if (info.STATE == ModuleState::ERROR_FATAL)  s_CoreLogger.critical(text.append("Fatal: ").append(info.WHAT));
			}
			else
			{
				std::cout << "Critical log module error detected... Dumping state:" << std::endl;
				if (info.STATE == ModuleState::GOOD)         std::cout << "     * " << text.append("Good") << std::endl;
				if (info.STATE == ModuleState::ERROR_USABLE) std::cout << "     * " << text.append("Non-critical error: ").append(info.WHAT) << std::endl;
				if (info.STATE == ModuleState::ERROR_RISKY)  std::cout << "     * " << text.append("Error: ").append(info.WHAT) << std::endl;
				if (info.STATE == ModuleState::ERROR_FATAL)  std::cout << "     * " << text.append("Fatal: ").append(info.WHAT) << std::endl;
			}

		}

		template<typename FormatString, typename... Args>
		static inline void core_log(LogLevel level, FormatString fomt, const Args &... args)
		{
			if (level == LogLevel::Trace) s_CoreLogger.trace(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Info) s_CoreLogger.info(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Warn) s_CoreLogger.warn(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Error) s_CoreLogger.error(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Fatal) s_CoreLogger.critical(fmt::runtime(fomt), args...);

			trigger_reload();
		}

		template<typename FormatString, typename... Args>
		static inline void client_log(LogLevel level, FormatString fomt, const Args &... args)
		{
			if (level == LogLevel::Trace) s_ClientLogger.trace(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Info) s_ClientLogger.info(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Warn) s_ClientLogger.warn(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Error) s_ClientLogger.error(fmt::runtime(fomt), args...);
			else if (level == LogLevel::Fatal) s_ClientLogger.critical(fmt::runtime(fomt), args...);

			trigger_reload();
		}

		static void print_core_log(std::ostream& oss, size_t length = 0)
		{
			auto ring = std::dynamic_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(s_CoreLogger.sinks()[0]);

			for (std::string& s : ring->last_formatted(length))
			{
				oss << s << std::endl;
			}
		}

		static void print_client_log(std::ostream& oss, size_t length = 0)
		{
			auto ring = std::dynamic_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(s_ClientLogger.sinks()[0]);

			for (std::string& s : ring->last_formatted(length))
			{
				oss << s << std::endl;
			}
		}

	private:
		static bool s_Init;

		static spdlog::logger s_CoreLogger;
		static spdlog::logger s_ClientLogger;

		static Gstring s_LogDir;
		static ModuleInfo s_Info;
	};

}