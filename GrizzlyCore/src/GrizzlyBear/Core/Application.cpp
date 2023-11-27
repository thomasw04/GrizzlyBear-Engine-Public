#include <gbpch.h>

#include "Application.h"

#include "Thread.h"

#include "GrizzlyBear/Renderer/Renderer.h"

#include "GrizzlyBear/Environment/Environment.h"

#include "GrizzlyBear/Misc/Random.h"

#include <sodium.h>

namespace GrizzlyBear {

	Ref<GrizzlyApplication> Core::s_App = nullptr;
	Timestep				Core::s_TimeStep;
	bool					Core::s_Running = true;
	bool					Core::s_Minimized = false;

	void Core::run(const Gstring& projectFile, Ref<GrizzlyApplication> app)
	{
		s_App = app;

		Random::init();
		Guid::init();

		{
			ModuleInfo platformState = Platform::init();
			ModuleInfo threadState = ThreadManager::init();
			ModuleInfo envState = Environment::init(projectFile);
			ModuleInfo logState = Log::init();

			GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Gather module state...");

			Log::core_log_module_state("Log", logState);
			Log::core_log_module_state("Platform", platformState);
			Log::core_log_module_state("Thread", threadState);
			Log::core_log_module_state("Environment", envState);
			
			if (!error_rating({ platformState.STATE, envState.STATE, logState.STATE }))
			{
				panic();
				return;
			}

			/*if (sodium_init() < 0)
			{
				//I am not sure if this is a valid reason to panic the whole engine.
				panic();
			}*/
		}

		{
			GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Trying to load application module...");

			ModuleInfo appState = s_App->on_module_init();

			Log::core_log_module_state("Application", appState);

			if (!error_rating({ appState.STATE }))
			{
				panic();
				return;
			}
		}

		//Register window events
		app->get_main_window()->on_closed([&]()
		{
			s_Running = false;
			return true;
		});

		app->get_main_window()->on_window_resize([&](const WindowResizeEvent& e)
		{
			if (e.WIDTH == 0 || e.HEIGHT == 0)
			{
				s_Minimized = true;
			}
			else
			{
				s_Minimized = false;
			}

			app->get_main_window()->get_context()->set_viewport(0, 0, e.WIDTH, e.HEIGHT);
			return false;
		});

		{
			ImGuiLibrary::init(s_App->get_main_window());
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Info, "Initialization complete. Starting application...");

		

		s_App->on_startup();

		while (s_Running)
		{
			s_TimeStep.update(s_App->get_main_window()->get_context()->get_statistics()->get_time());

			if (!s_Minimized)
			{
				s_App->on_update(s_TimeStep);
			}

			ImGuiLibrary::begin();

			s_App->on_gui_update(s_TimeStep);

			ImGuiLibrary::end();

			s_App->get_main_window()->on_update();
		}

		ImGuiLibrary::shutdown();
		ThreadManager::shutdown();
		
		s_App->on_shutdown();

		Log::shutdown();

		s_App->get_terminal()->suspend();
	}

	void Core::panic()
	{
		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Fatal, "Critical error detected. Panicking...");

		ThreadManager::panic();
		Log::panic();
		return;
	}

	void Core::shutdown()
	{
		s_Running = false;
	}

	bool Core::error_rating(std::initializer_list<ModuleState> states)
	{
		for (auto it = states.begin(); it != states.end(); it++)
		{
			if ((*it) == ModuleState::ERROR_FATAL)
			{
				return false;
			}
		}

		return true;
	}
}
