#pragma once

#include <functional>

#include <imgui/imgui.h>

namespace GrizzlyBear {

	//This class is an addon to imgui! Copied code with small changes could be found. See license of imgui at GrizzlyBear/libs/imgui/LICENSE.txt

	namespace Addon {

		using imgui_state_func = std::function<std::pair<bool*, bool*>()>;
		using imgui_callback_func = std::function<void()>;

		//Check Header
		void imgui_cheader(const char* label, imgui_state_func func, imgui_callback_func cfunc);

		bool imgui_checkbox(const char* label, bool* value, const ImVec2& offset = ImVec2(0.0f, 0.0f));

		bool imgui_grey_out(bool greyout, imgui_callback_func cfunc);

		bool imgui_dropdown_button(const char* label, ImVec2 size, imgui_callback_func cfunc);

		//Popups
		void imgui_render_popups();

	}

}
