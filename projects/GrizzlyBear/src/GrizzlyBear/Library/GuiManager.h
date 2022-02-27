#pragma once

#include "GrizzlyBear/Core/Event.h"
#include "GrizzlyBear/API/Window.h"

namespace GrizzlyBear {

	class ImGuiLibrary
	{
	public:
		static void init(Ref<Window> window);
		static void update();
		static void shutdown();
		
		static void begin();
		static void end();

	private:
		static Ref<Window> s_Window;
		static Gstring s_ImGuiConf;
	};

}
