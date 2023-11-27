#include "LevelEditorWindow.h"

#include <GrizzlyBear.h>

namespace GrizzlyEditor {

	LevelEditorWindow::LevelEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window)
		: EditorWindow(open, window) {}

	LevelEditorWindow::LevelEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window)
		: EditorWindow(open, position, size, window) {}

	LevelEditorWindow::~LevelEditorWindow() {}

	void LevelEditorWindow::render_window()
	{
		//GrizzlyBear::Ref<GrizzlyBear::Scene> scene = GrizzlyBear::SceneManager::get_scene();

		//ImGui::Text("%s", GrizzlyBear::SceneManager::get_scene_name().c_str());
		ImGui::Separator();

		if (ImGui::BeginTabBar("##level_header"))
		{
			if (ImGui::BeginTabItem("Settings"))
			{
				ImGui::Text("Level Script: ");

				ImGui::SameLine();

				if (ImGui::InputText("##level_script_path", &m_LevelScriptFilePath, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
				{
					//TODO = m_LevelScriptFilePath;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AM_PushFile"))
					{
						GrizzlyBear::FilePath path((const char*)payload->Data);

						if (!path.is_directory())
						{
							if (path.get_extension() == "lua")
							{
								//TODO = path.get_path();
								m_LevelScriptFilePath = path.str();
							}
						}
					}
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	void LevelEditorWindow::reset_window()
	{
	}

}