#include "AssetManagerEditorWindow.h"

#include <GrizzlyBear/Environment/Environment.h>
#include <GrizzlyBear/Template/SceneManager.h>

#include <imgui/imgui_internal.h>

namespace GrizzlyEditor {

	AssetManagerEditorWindow::AssetManagerEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, window)
	{
		m_FolderIconEmpty = GrizzlyBear::AssetManager::wait_for_imm_texture(m_Window->get_context(), "engine/editor/textures/FolderEmpty.png");
		m_FolderIconFilled = GrizzlyBear::AssetManager::wait_for_imm_texture(m_Window->get_context(), "engine/editor/textures/FolderFilled.png");

		m_FileIconNormal = GrizzlyBear::AssetManager::wait_for_imm_texture(m_Window->get_context(), "engine/editor/textures/FileNormal.png");
		m_FileIconCode = GrizzlyBear::AssetManager::wait_for_imm_texture(m_Window->get_context(), "engine/editor/textures/FileCode.png");

		m_SceneManager = sceneManager;
	}

	AssetManagerEditorWindow::AssetManagerEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, position, size, window)
	{
		m_SceneManager = sceneManager;
	}

	AssetManagerEditorWindow::~AssetManagerEditorWindow() {}

	void AssetManagerEditorWindow::render_window()
	{
		ImGui::SetCursorPos({0.0f, 20.0f});

		ImGui::BeginChild("MenuBar", { EditorWindow::get_content_size().x + 20.0f, 30.0f }, true);

		ImGui::SetCursorPos({ 6.0f, 2.5f });

		//TODO Create Button

		ImGui::SetCursorPos({ 200.0f, 2.5f });

		if (ImGui::Button("Back", { 60.0f, 25.0f }))
		{
			m_CurrentPath.back(1);
		}

		ImGui::EndChild();

		ImGui::SetCursorPos({ 1.0f, 49.0f });

		ImGui::BeginChild("FoldersSmall", { 200.0f, EditorWindow::get_content_size().y - 49.0f }, true);

		ImGuiTreeNodeFlags smallFolderFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;

		ImGui::AlignTextToFramePadding();

		ImGui::TreeNodeEx("assets", smallFolderFlags);

		if (ImGui::IsItemClicked(0))
		{
			m_CurrentPath = GrizzlyBear::FilePath("assets/");
		}

		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("cache", smallFolderFlags);

		if (ImGui::IsItemClicked(0))
		{
			m_CurrentPath = GrizzlyBear::FilePath("cache/");
		}

		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("config", smallFolderFlags);
		
		if (ImGui::IsItemClicked(0))
		{
			m_CurrentPath = GrizzlyBear::FilePath("config/");
		}

		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("content", smallFolderFlags);

		if (ImGui::IsItemClicked(0))
		{
			m_CurrentPath = GrizzlyBear::FilePath("content/");
		}

		ImGui::AlignTextToFramePadding();
		ImGui::TreeNodeEx("mods", smallFolderFlags);

		if (ImGui::IsItemClicked(0))
		{
			m_CurrentPath = GrizzlyBear::FilePath("mods/");
		}

		ImGui::EndChild();

		ImGui::SetCursorPos({ 200.0f, 49.0f });

		ImGui::BeginChild("FoldersBig", { EditorWindow::get_content_size().x - 200.0f, EditorWindow::get_content_size().y - 49.0f }, true);

		std::vector<GrizzlyBear::RelativePath> files = GrizzlyBear::Environment::get_fetcher()->find_entries(m_CurrentPath.str());

		ImVec2 nextPos = {10.0f, 10.0f};

		int id = 0;

		bool clicked = false;

		if (ImGui::IsMouseClicked(1))
		{
			clicked = true;
		}

		for (GrizzlyBear::RelativePath path : files)
		{
			GrizzlyBear::File file = GrizzlyBear::FileIO::open(path.m_FullPath);

			if ((ImGui::GetWindowSize().x - 10.0f) < nextPos.x + 120.0f)
			{
				nextPos.x = 10.0f;
				nextPos.y += 150.0f;
			}

			ImGui::SetCursorPos(nextPos);

			ImGui::BeginChild((std::string("FileOrFolder#") + std::to_string(id)).c_str(), ImVec2(110, 140));

			if (ImGui::BeginDragDropSource())
			{
				ImGui::Text("Draging %s", path.m_FullPath.get_name().c_str());

				ImGui::SetDragDropPayload("AM_PushFile", (m_CurrentPath.str() + path.m_FullPath.get_name()).c_str(), std::strlen((m_CurrentPath.str() + path.m_FullPath.get_name()).c_str()) + 1);

				ImGui::EndDragDropSource();
			}

			if (file.is_directory())
			{
				ImGui::Image((void*)(uintptr_t)m_FolderIconEmpty->get_render_id(), ImVec2(100, 81), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				ImGui::Image((void*)(uintptr_t)m_FileIconNormal->get_render_id(), ImVec2(86, 110), ImVec2(0, 1), ImVec2(1, 0));
			}
			
			ImGui::SetCursorPosX(5.0f);

			ImGui::TextWrapped(path.m_FullPath.get_name().c_str());

			ImGui::EndChild();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && !ImGui::IsMouseDragging(0))
			{
				if (path.m_FullPath.is_directory())
				{
					m_CurrentPath.add(path.m_Relative + std::string("/"));
				}
				else
				{
					if (path.m_FullPath.get_extension() == "gcs")
					{
						m_SceneManager->unload_scene(m_SceneManager->get_scene_name());

						if (!m_SceneManager->exist_scene(path.m_FullPath.get_name_woe()))
						{
							m_SceneManager->load_scene(path.m_FullPath.get_name_woe(), m_CurrentPath.str() + path.m_FullPath.get_name());
						}
						
						m_SceneManager->set_scene(path.m_FullPath.get_name_woe());
					}
				}
			}

			if (ImGui::IsMouseClicked(1))
			{
				if (ImGui::IsItemHovered())
				{
					ImGui::OpenPopup((std::string("AssetActions#") + std::to_string(id)).c_str());
					clicked = false;
				}
			}

			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);

			if (ImGui::BeginPopup((std::string("AssetActions#") + std::to_string(id)).c_str()))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

				ImGui::Text("Actions");

				ImGui::Separator();

				if (ImGui::MenuItem((std::string("Delete##Asset_id") + std::to_string(id)).c_str(), "SHIFT+X"))
				{
					ImGui::CloseCurrentPopup();

					file.destroy();
				}

				ImGui::PopStyleVar();

				ImGui::EndPopup();
			}

			ImGui::PopStyleVar();

			nextPos.x += 120.0f;
			id++;
		}

		if (clicked && (m_Hovered || ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows)))
		{
			ImGui::OpenPopup("Actions##AssetManager");
		}

		bool open = false;

		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);

		if (ImGui::BeginPopup("Actions##AssetManager"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
			
			ImGui::Text("Actions");

			ImGui::Separator();

			if (ImGui::BeginMenu("Create##AssetManager"))
			{
				if (ImGui::MenuItem("Level##AssetManager", "SHIFT+C+L"))
				{
					ImGui::CloseCurrentPopup();
					open = true;
				}

				if (ImGui::MenuItem("Animations##AssetManager", "SHIFT+C+A"))
				{
					ImGui::CloseCurrentPopup();
					open = true;
				}

				ImGui::EndMenu();
			}

			ImGui::PopStyleVar();

			ImGui::EndPopup();
		}

		ImGui::PopStyleVar();

		if(open) ImGui::OpenPopup("CreateLevel##AssetManager");

		if (ImGui::BeginPopup("CreateLevel##AssetManager"))
		{
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
				ImGui::SetKeyboardFocusHere(0);

			if (ImGui::InputText("##Level name", &m_InputBuf, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_SceneManager->unload_scene(m_SceneManager->get_scene_name());
				m_SceneManager->load_scene(m_InputBuf, m_CurrentPath.str() + std::string(m_InputBuf) + std::string(".gcs"));
				m_SceneManager->set_scene(std::string(m_InputBuf));

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::EndChild();
	}

	void AssetManagerEditorWindow::reset_window()
	{

	}

	void AssetManagerEditorWindow::open_level_create_dialog()
	{
	}

}