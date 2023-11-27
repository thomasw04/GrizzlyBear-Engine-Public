#include "WindowManager.h"

namespace GrizzlyEditor {

	void WindowManager::update_window(const std::string& name, std::shared_ptr<EditorWindow> window, ImGuiWindowFlags flags, bool forceFocus, ImParamFunc before, ImParamFunc after)
	{
		if (!window->get_window_open()) return;

		ImGui::SetNextWindowSize(window->m_Size, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(window->m_Position, ImGuiCond_FirstUseEver);

		if (forceFocus) ImGui::SetNextWindowFocus();

		if (before != nullptr) before();

		ImGui::Begin(name.c_str(), &window->m_Open, flags);

		window->m_Size = ImGui::GetWindowSize();
		window->m_Position = ImGui::GetWindowPos();

		window->m_ContentSize = ImGui::GetWindowContentRegionMax();

		ImVec2 contentPos = ImGui::GetWindowContentRegionMin();

		contentPos.x += window->m_Position.x;
		contentPos.y += window->m_Position.y;

		window->m_ContentPos = contentPos;

		window->render_window();

		window->m_Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow);
		window->m_Hovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootWindow);

		GrizzlyBear::Addon::imgui_render_popups();

		ImGui::End();

		if (after != nullptr) after();
	}

	void WindowManager::update_docked_window(const std::string& name, std::shared_ptr<EditorWindow> window, unsigned int dockspaceID, ImGuiWindowFlags flags, bool forceFocus, ImParamFunc before, ImParamFunc after)
	{
		if (!window->get_window_open()) return;

		ImGui::SetNextWindowSize(window->m_Size, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(window->m_Position, ImGuiCond_FirstUseEver);

		//TODO: Crapy - Need a rework with custom condition
		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);

		if (forceFocus) ImGui::SetNextWindowFocus();

		if (before != nullptr) before();

		ImGui::Begin(name.c_str(), &window->m_Open, flags);

		window->m_Size = ImGui::GetWindowSize();
		window->m_Position = ImGui::GetWindowPos();
		window->m_Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow);
		window->m_Hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow);

		window->m_ContentSize = ImGui::GetWindowContentRegionMax();

		ImVec2 contentPos = ImGui::GetWindowContentRegionMin();

		contentPos.x += window->m_Position.x;
		contentPos.y += window->m_Position.y;

		window->m_ContentPos = contentPos;
		
		window->render_window();

		GrizzlyBear::Addon::imgui_render_popups();

		ImGui::End();

		if (after != nullptr) after();
	}

}