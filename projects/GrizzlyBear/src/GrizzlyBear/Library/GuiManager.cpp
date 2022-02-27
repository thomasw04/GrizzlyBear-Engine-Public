#include <gbpch.h>

#include "GuiManager.h"

#include <imgui/imgui.h>

//Compile ImGui example
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/backends/imgui_impl_glfw.cpp>

#include "GrizzlyBear/Core/Application.h"

#include "GrizzlyBear/Environment/Environment.h"

namespace GrizzlyBear {

	Ref<Window> ImGuiLibrary::s_Window = nullptr;

	Gstring ImGuiLibrary::s_ImGuiConf;

	void ImGuiLibrary::init(Ref<Window> window)
	{
		GRIZZLY_CORE_LOG(LogLevel::Trace, "Init gui module...");

		s_Window = window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		Gstring imgui_conf = Environment::get_fetcher()->get_project_dir() + Environment::get_fetcher()->get_config_value("InBuild", "CacheDir");
		imgui_conf += "layout.ini";

		s_ImGuiConf = imgui_conf;

		io.IniFilename = s_ImGuiConf.c_str();

		ImGui::StyleColorsDark();

		GLFWwindow* glfwwindow = static_cast<GLFWwindow*>(window->get_native_window());

		ImGui_ImplGlfw_InitForOpenGL(glfwwindow, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		io.Fonts->AddFontDefault();

		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowRounding = 6.0f;
		style.ChildRounding = 6.0f;
		style.FrameRounding = 3.0f;
		style.PopupRounding = 6.0f;
		style.ScrollbarRounding = 6.0f;
		style.TabRounding = 3.0f;
		style.GrabRounding = 6.0f;
		style.ScrollbarRounding = 5.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing.y = 6.5f;

		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ColorButtonPosition = ImGuiDir_Right;

		style.Colors[ImGuiCol_Text] = { 0.85f, 0.85f, 0.85f, 1.00f };
		style.Colors[ImGuiCol_TextDisabled] = { 0.34f, 0.34f, 0.34f, 1.00f };
		style.Colors[ImGuiCol_WindowBg] = { 0.23f, 0.24f, 0.25f, 0.94f };
		style.Colors[ImGuiCol_ChildBg] = { 0.23f, 0.24f, 0.25f, 0.00f };
		style.Colors[ImGuiCol_PopupBg] = { 0.23f, 0.24f, 0.25f, 0.94f };
		style.Colors[ImGuiCol_Border] = { 0.37f, 0.37f, 0.37f, 0.50f };
		style.Colors[ImGuiCol_BorderShadow] = { 0.37f, 0.37f, 0.37f, 0.0f };
		style.Colors[ImGuiCol_FrameBg] = { 0.16f, 0.16f, 0.16f, 0.54f };
		style.Colors[ImGuiCol_FrameBgHovered] = { 0.45f, 0.67f, 0.99f, 0.67f };
		style.Colors[ImGuiCol_FrameBgActive] = { 0.35f, 0.35f, 0.35f, 0.67f };
		style.Colors[ImGuiCol_TitleBg] = { 0.16f, 0.16f, 0.16f, 1.00f };
		style.Colors[ImGuiCol_TitleBgCollapsed] = { 0.16f, 0.29f, 0.48f, 1.00f };
		style.Colors[ImGuiCol_TitleBgActive] = { 0.12f, 0.12f, 0.12f, 0.51f };
		style.Colors[ImGuiCol_MenuBarBg] = { 0.02f, 0.02f, 0.02f, 0.80f };
		style.Colors[ImGuiCol_ScrollbarBg] = { 0.27f, 0.28f, 0.29f, 0.60f };
		style.Colors[ImGuiCol_ScrollbarGrab] = { 0.21f, 0.30f, 0.41f, 0.51f };
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.21f, 0.30f, 0.41f, 1.00f };
		style.Colors[ImGuiCol_ScrollbarGrabActive] = { 0.13f, 0.19f, 0.26f, 0.91f };
			 
		style.Colors[ImGuiCol_Tab] = { 0.16f, 0.19f, 0.30f, 0.0f };
		style.Colors[ImGuiCol_TabActive] = { 0.25f, 0.40f, 0.70f, 1.0f };
		style.Colors[ImGuiCol_TabUnfocused] = { 0.16f, 0.19f, 0.30f, 0.0f };
		style.Colors[ImGuiCol_TabUnfocusedActive] = { 0.16f, 0.19f, 0.30f, 0.0f };
			 
		style.Colors[ImGuiCol_CheckMark] = { 0.90f, 0.90f, 0.90f, 0.83f };
		style.Colors[ImGuiCol_SliderGrab] = { 0.70f, 0.70f, 0.70f, 0.62f };
		style.Colors[ImGuiCol_SliderGrabActive] = { 0.30f, 0.30f, 0.30f, 0.84f };
		style.Colors[ImGuiCol_Button] = { 0.33f, 0.35f, 0.36f, 0.49f };
		style.Colors[ImGuiCol_ButtonHovered] = { 0.21f, 0.30f, 0.41f, 1.00f };
		style.Colors[ImGuiCol_ButtonActive] = { 0.13f, 0.19f, 0.26f, 1.00f };
		style.Colors[ImGuiCol_Header] = { 0.33f, 0.35f, 0.36f, 0.53f };
		style.Colors[ImGuiCol_HeaderHovered] = { 0.45f, 0.67f, 0.99f, 0.67f };
		style.Colors[ImGuiCol_HeaderActive] = { 0.47f, 0.47f, 0.47f, 0.67f };
		style.Colors[ImGuiCol_Separator] = { 0.31f, 0.31f, 0.31f, 1.00f };
		style.Colors[ImGuiCol_SeparatorHovered] = { 0.31f, 0.31f, 0.31f, 1.00f };
		style.Colors[ImGuiCol_SeparatorActive] = { 0.31f, 0.31f, 0.31f, 1.00f };
		style.Colors[ImGuiCol_ResizeGrip] = { 1.00f, 1.00f, 1.00f, 0.85f };
		style.Colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.00f, 1.00f, 0.60f };
		style.Colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.00f, 0.90f };
		style.Colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
		style.Colors[ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
		style.Colors[ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
		style.Colors[ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
		style.Colors[ImGuiCol_TextSelectedBg] = { 0.18f, 0.39f, 0.79f, 0.90f };
	}

	void ImGuiLibrary::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLibrary::begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		update();
	}

	void ImGuiLibrary::end()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)s_Window->get_width(), (float)s_Window->get_height());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLibrary::update() {}

}
