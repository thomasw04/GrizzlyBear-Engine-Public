#include <gbpch.h>

#include "Log.h"

#include <GrizzlyBear/Environment/Environment.h>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ringbuffer_sink.h"

namespace GrizzlyBear {

	spdlog::logger Log::s_CoreLogger = spdlog::logger("Empty");
	spdlog::logger Log::s_ClientLogger = spdlog::logger("Empty");

	bool Log::s_Init = false;
	Gstring Log::s_LogDir;

	ModuleState Log::init()
	{
		if (Environment::has_environment())
		{
			s_LogDir = Environment::get_fetcher()->get_project_dir() + Environment::get_fetcher()->get_config_value("InBuild", "LogDir");
		}
		else
		{
			s_LogDir = "logs/";
		}

		Gstring reset = s_LogDir;

		auto core_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		core_console_sink->set_level(spdlog::level::trace);
		core_console_sink->set_pattern("%^[%T] [%n]: %v%$");

		auto core_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(s_LogDir.append("latest_core.log"), true);
		core_file_sink->set_level(spdlog::level::trace);
		core_file_sink->set_pattern("%^[%T] [%n]: %v%$");

		s_CoreLogger = spdlog::logger("GrizzlyBear", {core_console_sink, core_file_sink});
		s_CoreLogger.set_level(spdlog::level::trace);

		auto app_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		app_console_sink->set_level(spdlog::level::trace);
		app_console_sink->set_pattern("%^[%T] [%n]: %v%$");

		s_LogDir = reset;

		auto app_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(s_LogDir.append("latest_app.log"), true);
		app_file_sink->set_level(spdlog::level::trace);
		app_file_sink->set_pattern("%^[%T] [%n]: %v%$");

		s_ClientLogger = spdlog::logger("Application", {app_console_sink, app_file_sink});
		s_ClientLogger.set_level(spdlog::level::trace);

		s_Init = true;

		s_LogDir = reset;

		return ModuleState::GOOD;
	}

	void Log::post_init(const Gstring& appName)
	{
	}

	void Log::shutdown()
	{
		s_CoreLogger.flush();
		s_ClientLogger.flush();

		Gstring reset = s_LogDir;

		File core = FileIO::open(s_LogDir.append("latest_core.log"));
		s_LogDir = reset;
		core.copy(s_LogDir.append("[CORE] ").append(Platform::get_date("%Y-%m-%d-%H-%M-%S")).append(".log"), true);

		s_LogDir = reset;

		File app = FileIO::open(s_LogDir.append("latest_app.log"));
		s_LogDir = reset;
		app.copy(s_LogDir.append("[APP] ").append(Platform::get_date("%Y-%m-%d-%H-%M-%S")).append(".log"), true);

		s_LogDir = reset;
	}

	void Log::set_console_filter(LogLevel level)
	{
		if (level == LogLevel::Trace)
		{
			s_CoreLogger.sinks()[0]->set_level(spdlog::level::trace);
			s_ClientLogger.sinks()[0]->set_level(spdlog::level::trace);
		}
		else if (level == LogLevel::Info)
		{
			s_CoreLogger.sinks()[0]->set_level(spdlog::level::info);
			s_ClientLogger.sinks()[0]->set_level(spdlog::level::info);
		}
		else if (level == LogLevel::Warn)
		{
			s_CoreLogger.sinks()[0]->set_level(spdlog::level::warn);
			s_ClientLogger.sinks()[0]->set_level(spdlog::level::warn);
		}
		else if (level == LogLevel::Error)
		{
			s_CoreLogger.sinks()[0]->set_level(spdlog::level::err);
			s_ClientLogger.sinks()[0]->set_level(spdlog::level::err);
		}
		else if (level == LogLevel::Fatal)
		{
			s_CoreLogger.sinks()[0]->set_level(spdlog::level::critical);
			s_ClientLogger.sinks()[0]->set_level(spdlog::level::critical);
		}
	}
}
