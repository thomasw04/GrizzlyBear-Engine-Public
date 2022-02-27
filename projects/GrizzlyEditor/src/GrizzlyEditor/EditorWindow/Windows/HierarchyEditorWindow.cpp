#include "HierarchyEditorWindow.h"

#include <GrizzlyBear.h>

namespace GrizzlyEditor {

	HierarchyEditorWindow::HierarchyEditorWindow(bool open, std::shared_ptr<ObjectEditorWindow> objectWindow, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, window), m_ObjectWindow(objectWindow)
	{
		m_SceneManager = sceneManager;
	}

	HierarchyEditorWindow::HierarchyEditorWindow(bool open, const ImVec2& position, const ImVec2& size, std::shared_ptr<ObjectEditorWindow> objectWindow, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, position, size, window), m_ObjectWindow(objectWindow)
	{
		m_SceneManager = sceneManager;
	}

	HierarchyEditorWindow::~HierarchyEditorWindow() {}

	void HierarchyEditorWindow::render_window()
	{
		if (!m_SceneManager->is_scene_set()) return;

		GrizzlyBear::Ref<GrizzlyBear::Scene> scene = m_SceneManager->get_scene();

		bool clickedOutside = false;

		if (ImGui::IsMouseClicked(1) && m_Hovered)
		{
			clickedOutside = true;
		}

		ImGui::AlignTextToFramePadding();
		if (ImGui::TreeNode("Root Node"))
		{
			for (auto it = scene->alive_end(); it != scene->alive_begin();)
			{
				--it;

				ImGui::PushID(*it);

				std::string name;

				if (scene->get_lookup_table().exist_id(*it))
				{
					name = scene->get_lookup_table().get_name(*it);
				}
				else
				{
					name = std::to_string(*it);
				}

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

				if (m_ObjectWindow->get_current_entity() == *it)
					flags |= ImGuiTreeNodeFlags_Selected;

				bool dirty = false;

				ImGui::AlignTextToFramePadding();
				if (ImGui::TreeNodeEx(name.c_str(), flags))
				{
					if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
					{
						m_ObjectWindow->set_current_entity(*it);
						clickedOutside = false;
					}

					if (!(dirty = open_action_popup()))
					{
						ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Transform2D", flags);

						if (scene->has_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, *it))
						{
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx("Mesh2D", flags);
						}

						if (scene->has_component<GrizzlyBear::CompCamera>(GrizzlyBear::MemoryType::Dynamic, *it))
						{
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx("Camera", flags);
						}

						if (scene->has_component<GrizzlyBear::CompWidget>(GrizzlyBear::MemoryType::Dynamic, *it))
						{
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx("Widget", flags);
						}
					}

					ImGui::TreePop();
				}
				else
				{
					if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
					{
						m_ObjectWindow->set_current_entity(*it);
						clickedOutside = false;
					}

					dirty = open_action_popup();
				}
				 
				ImGui::PopID();

				if (dirty)
				{
					it = scene->alive_end();
					it--;
				}
			}

			ImGui::TreePop();
		}

		if (clickedOutside)
		{
			ImGui::OpenPopup("Actions##HierarchyWindow");
		}

		bool openEntityCreator = false;

		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);

		if (ImGui::BeginPopup("Actions##HierarchyWindow"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

			ImGui::Text("Actions");

			ImGui::Separator();

			if (ImGui::BeginMenu("Create##HierarchyWindow"))
			{
				if (ImGui::MenuItem("Entity##HierarchyWindow", "SHIFT+C+E"))
				{
					ImGui::CloseCurrentPopup();

					openEntityCreator = true;
				}

				ImGui::EndMenu();
			}

			ImGui::PopStyleVar();

			ImGui::EndPopup();
		}

		ImGui::PopStyleVar();

		if(openEntityCreator) ImGui::OpenPopup("CreateEntity##HierarchyWindow");

		if (ImGui::BeginPopup("CreateEntity##HierarchyWindow"))
		{
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
				ImGui::SetKeyboardFocusHere(0);

			if (ImGui::InputText("##Entity name", &m_InputBuf, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GrizzlyBear::EntityHandle handle = m_SceneManager->get_scene()->add_entity();
				scene->get_lookup_table().create_lookup(m_InputBuf, handle.get_id());
				m_InputBuf = "";
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void HierarchyEditorWindow::reset_window() {}

	bool HierarchyEditorWindow::open_action_popup()
	{
		bool returnValue = false;

		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);

		if (ImGui::BeginPopupContextItem("EntityAction", ImGuiPopupFlags_MouseButtonRight))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

			ImGui::Text("Actions");

			ImGui::Separator();

			if (m_SceneManager->get_scene()->has_component<GrizzlyBear::CompCamera>(GrizzlyBear::MemoryType::Dynamic, m_ObjectWindow->get_current_entity()))
			{
				if (ImGui::Button("Set Camera Entity##Entity"))
				{
					m_SceneManager->get_scene()->set_camera(m_ObjectWindow->get_current_entity());
				}
			}

			if (ImGui::Button("Delete##Entity"))
			{
				m_SceneManager->get_scene()->get_lookup_table().delete_lookup(m_ObjectWindow->get_current_entity().get_id());

				m_SceneManager->get_scene()->remove_entity(m_ObjectWindow->get_current_entity());
				returnValue = true;
			}

			ImGui::PopStyleVar();

			ImGui::EndPopup();
		}

		ImGui::PopStyleVar();

		return returnValue;
	}

}