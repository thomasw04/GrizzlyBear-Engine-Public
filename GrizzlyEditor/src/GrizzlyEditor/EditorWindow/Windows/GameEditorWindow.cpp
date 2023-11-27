#include "GameEditorWindow.h"

#include <GrizzlyBear/Core/FileIO.h>
#include <GrizzlyBear/Template/SceneManager.h>

namespace GrizzlyEditor {

	GameEditorWindow::GameEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, window)
	{
		m_SceneManager = sceneManager;
	}

	GameEditorWindow::GameEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, position, size, window)
	{
		m_SceneManager = sceneManager;
	}

	GameEditorWindow::~GameEditorWindow() {}

	void GameEditorWindow::render_window()
	{
		ImGui::BeginChild("Game Window");

		ImGui::GetWindowDrawList()->AddImage((void*)(uintptr_t)m_SceneID, ImVec2(ImGui::GetCursorScreenPos().x - 10, ImGui::GetCursorScreenPos().y - 10),
											 ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x, ImGui::GetCursorScreenPos().y + ImGui::GetWindowSize().y),
											 ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::IsWindowFocused())
		{
			m_Focused = true;
		}
		else
		{
			m_Focused = false;
		}

		if (ImGui::IsWindowHovered())
		{
			m_Hovered = true;
		}
		else
		{
			m_Hovered = false;
		}

		ImGui::EndChild();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AM_PushFile"))
			{
				GrizzlyBear::FilePath path((const char*)payload->Data);

				if (!path.is_directory())
				{
					if (path.get_extension() == "gcs")
					{
						m_SceneManager->unload_scene(m_SceneManager->get_scene_name());

						if (!m_SceneManager->exist_scene(path.get_name_woe()))
						{
							m_SceneManager->load_scene(path.get_name_woe(), path.str());
						}

						m_SceneManager->set_scene(path.get_name_woe());
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
	}

	void GameEditorWindow::reset_window()
	{
		m_SceneID = 0;
	}

}