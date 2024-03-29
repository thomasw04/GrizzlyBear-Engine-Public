#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/API/Window.h"

#include "GrizzlyBear/Core/Event.h"
#include "GrizzlyBear/Core/Platform/Platform.h"

#include "GrizzlyBear/Library/GuiManager.h"

#include "GrizzlyBear/Terminal/Terminal.h"

#include <stack>

#include <vector>
#include <tuple>

namespace GrizzlyBear {

	class GrizzlyApplication
	{
	public:
		GrizzlyApplication()
		{
			m_Terminal = std::make_shared<Executor>(Tag<Terminal>{});
			m_Terminal->start();
		}

		virtual ModuleInfo on_module_init() = 0;

		virtual bool on_startup() = 0;

		virtual void on_update(Timestep ts) = 0;

		virtual void on_gui_update(Timestep ts) = 0;

		virtual bool on_shutdown() = 0;

		virtual Ref<Window> get_main_window() = 0;

		Ref<Executor> get_terminal()
		{
			return m_Terminal;
		}

	private:
		Ref<Executor> m_Terminal;
	};

	class Core
	{
	public:
		static void run(const Gstring& projectFile, Ref<GrizzlyApplication> app);

		static void panic();

		inline static Timestep    get_timestep() { return s_TimeStep; }

		static Ref<GrizzlyApplication> get_app() { return s_App; }

		static void shutdown();

	private:
		static bool error_rating(std::initializer_list<ModuleState> states);

	private:
		static Ref<GrizzlyApplication> s_App;
		static ImGuiLibrary s_ImGuiLib;

		static Timestep s_TimeStep;
		static bool s_Running;
		static bool s_Minimized;
	};

}

