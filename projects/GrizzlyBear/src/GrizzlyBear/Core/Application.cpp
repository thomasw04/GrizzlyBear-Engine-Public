#include <gbpch.h>

#include "Application.h"

#include "Thread.h"

#include "GrizzlyBear/Renderer/Renderer.h"

#include "GrizzlyBear/Environment/Environment.h"

#include "GrizzlyBear/Misc/Random.h"

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
			ModuleState platformState = Platform::init();

			ModuleState threadState = ThreadManager::init();

			ModuleState envState = Environment::init(projectFile);

			ModuleState logState = Log::init();

			GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Gather module state...");

			Log::core_log_module_state("Platform", platformState);
			Log::core_log_module_state("Thread", threadState);
			Log::core_log_module_state("Environment", envState);
			Log::core_log_module_state("Log", logState);

			if (!error_rating({ platformState, envState, logState }))
			{
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Fatal, "Critical error detected. Shutting down...");

				ThreadManager::shutdown();
				Log::shutdown();
				return;
			}
		}

		//Reset the graphics to standard
		Platform::get_ansi_std().reset_graphics();

		{
			GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Trying to load application module...");

			ModuleState appState = s_App->on_module_init();

			Log::core_log_module_state("Application", appState);

			if (!error_rating({ appState }))
			{
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Fatal, "Critical error detected. Shutting down...");

				ThreadManager::shutdown();
				Log::shutdown();
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

		GrizzlyBear::Ref<GrizzlyBear::Renderer> m_MasterRenderer = GrizzlyBear::CreateRef<GrizzlyBear::Renderer>(app->get_main_window());

		s_App->on_startup();

		while (s_Running)
		{
			s_TimeStep.update(s_App->get_main_window()->get_context()->get_statistics()->get_time());

			if (!s_Minimized)
			{
				m_MasterRenderer->begin_frame();
				s_App->on_update(s_TimeStep);
				m_MasterRenderer->end_frame();
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
