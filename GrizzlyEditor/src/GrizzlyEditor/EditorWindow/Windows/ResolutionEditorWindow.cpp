#include "ResolutionEditorWindow.h"


namespace GrizzlyEditor {

	ResolutionEditorWindow::ResolutionEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window)
		: EditorWindow(open, window) {}

	ResolutionEditorWindow::ResolutionEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window)
		: EditorWindow(open, position, size, window) {}

	ResolutionEditorWindow::~ResolutionEditorWindow() {}

	void ResolutionEditorWindow::render_window()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

		ImGui::InputInt("Width in Pixels", &m_ResXImpl, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputInt("Height in Pixels", &m_ResYImpl, ImGuiInputTextFlags_CharsDecimal);

		ImGui::Separator();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((float)ImGui::GetWindowSize().x / 2) - ((float)ImGui::GetWindowSize().x * 0.3f / 2));

		if (ImGui::Button("Apply", ImVec2((float)ImGui::GetWindowSize().x * 0.3f, (float)ImGui::GetWindowSize().y * 0.2f)))
		{
			m_ResX = m_ResXImpl;
			m_ResY = m_ResYImpl;

			m_Open = false;
		}

		ImGui::PopStyleVar();
	}

	void ResolutionEditorWindow::reset_window() {}
}