#include "EditorDefaultMode.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace GrizzlyEditor {

	EditorDefaultMode::EditorDefaultMode(GrizzlyBear::Ref<GrizzlyBear::Window> window)
		: m_Window(window)
	{
		m_Renderer = GrizzlyBear::CreateRef<GrizzlyBear::Renderer>(window);
		m_Renderer2D = GrizzlyBear::CreateRef<GrizzlyBear::Renderer2D>(window->get_context(), GrizzlyBear::RendererHint_None);
		m_CamController = GrizzlyBear::CreateRef<GrizzlyBear::Template::OrthographicCamera>(window, 1280.0f / 720.0f, false);
		m_SceneManager = GrizzlyBear::CreateRef<GrizzlyBear::Template::SceneManager>(m_Window);

		m_ObjectEditor = insert_and_return<ObjectEditorWindow>("Object Editor", std::make_shared<ObjectEditorWindow>(true, window, m_SceneManager));
		m_GameEditor = insert_and_return<GameEditorWindow>("Game Editor", std::make_shared<GameEditorWindow>(true, window, m_SceneManager));
		m_AssetManagerEditor = insert_and_return<AssetManagerEditorWindow>("Asset Manager", std::make_shared<AssetManagerEditorWindow>(true, window, m_SceneManager));
		m_HierarchyEditor = insert_and_return<HierarchyEditorWindow>("Hierarchy Editor", std::make_shared<HierarchyEditorWindow>(true, m_ObjectEditor, window, m_SceneManager));
	}

	void EditorDefaultMode::on_activate()
	{
		m_FrameBuffer = m_Window->get_context()->create_frame_buffer(1920, 1080);
		m_RenderBuffer = m_Window->get_context()->create_render_buffer(1920, 1080, GRIZZLY_INTERNAL_DEPTH24);
		m_SceneTexture = m_Window->get_context()->create_texture_2d(1920, 1080);

		m_FrameBuffer->attach_texture_2d(m_SceneTexture, 0);
		m_FrameBuffer->attach_render_buffer(m_RenderBuffer, 0);

		m_FrameBuffer->unbind(1920, 1080);

		auto scene = m_SceneManager->load_scene("Test");
		m_SceneManager->set_scene("Test");

		GrizzlyBear::AtlasLayout testAnimLayout(std::initializer_list<GrizzlyBear::AtlasElement>
		{
			{ {0.0f, 0.0f}, { 0.25f, 0.25f } },
			{ {0.25f, 0.0f}, { 0.5f, 0.25f } },
			{ {0.5f, 0.0f}, { 0.75f, 0.25f } },
			{ {0.75f, 0.0f}, { 1.0f, 0.25f } },

			{ {0.0f, 0.25f}, { 0.25f, 0.5f } },
			{ {0.25f, 0.25f}, { 0.5f, 0.5f } },
			{ {0.5f, 0.25f}, { 0.75f, 0.5f } },
			{ {0.75f, 0.25f}, { 1.0f, 0.5f } },

			{ {0.0f, 0.5f}, { 0.25f, 0.75f } }});

		GrizzlyBear::Animation2D testAnim;

		testAnim.ACTIVE = true;
		testAnim.END_FRAME = 8;
		testAnim.LOOP = false;
		testAnim.PLAYTHROUGH_TIME = 5.0f;
		testAnim.TEXTURE_FILEPATH = "textures/SpriteSheet.png";
		testAnim.TEXTURE_LAYOUT = testAnimLayout;

		GrizzlyBear::Animator2D::set_animation("TestAnim", testAnim);
		GrizzlyBear::Animator2D::save_animation("TestAnim", "anims/TestAnim.ganim");

		memset(m_FrameTimeGraph, 0, sizeof(float) * 100);
		 
		m_Window->on_mouse(GrizzlyBear::MouseEvent::PRESSED, GRIZZLY_BIND_EVENT(EditorDefaultMode::on_mouse_pressed));
		m_Window->on_mouse(GrizzlyBear::MouseEvent::RELEASED, GRIZZLY_BIND_EVENT(EditorDefaultMode::on_mouse_released));
		m_Window->on_key(GrizzlyBear::KeyEvent::PRESSED, GRIZZLY_BIND_EVENT(EditorDefaultMode::on_key_pressed));

		m_SceneManager->set_custom_camera(m_CamController);
	}

	void EditorDefaultMode::on_deactivate()
	{
		m_SceneManager->save_scene(m_SceneManager->get_scene_name());

		m_Window->pop_mouse(GrizzlyBear::MouseEvent::PRESSED);
		m_Window->pop_mouse(GrizzlyBear::MouseEvent::RELEASED);
		m_Window->pop_key(GrizzlyBear::KeyEvent::PRESSED);
	}

	void EditorDefaultMode::on_update(GrizzlyBear::Timestep ts)
	{
		m_Renderer->begin_frame(m_FrameBuffer);

		m_GameEditor->get_scene_id() = m_SceneTexture->get_render_id();

		if (m_SceneManager->is_scene_set() && m_SceneManager->get_scene_name() != "Test")
		{
			m_SceneManager->on_update(ts);
		}

		m_Renderer->end_frame();
	}

	void EditorDefaultMode::on_gui_update(unsigned int dockspaceID)
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

		WindowManager::update_docked_window("Object Editor", m_ObjectEditor, dockspaceID);
		WindowManager::update_docked_window("Asset Manager", m_AssetManagerEditor, dockspaceID);
		WindowManager::update_docked_window("Level Hierarchy", m_HierarchyEditor, dockspaceID);

		WindowManager::update_docked_window("Game Window", m_GameEditor, dockspaceID);

		GrizzlyBear::WindowResizeEvent revent = GrizzlyBear::WindowResizeEvent((unsigned int)m_GameEditor->get_window_size().x, (unsigned int)m_GameEditor->get_window_size().y);

		m_CamController->on_window_resize(revent);

		ImParamFunc resolutionBefore = [&]()
		{
			ImGui::SetNextWindowSize(ImVec2(413, 104), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2((GrizzlyBear::Platform::get_display_width() / 2.0f) - (m_Window->get_width() / 2.0f), (GrizzlyBear::Platform::get_display_height() / 2.0f) - (m_Window->get_height() / 2.0f)), ImGuiCond_Appearing);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
		};

		ImParamFunc resolutionAfter = [&]()
		{
			ImGui::PopStyleVar();
		};

		ImGui::Begin("Frame");

		m_FrameTimeGraph[values_offset] = GrizzlyBear::Core::get_timestep().get_millis();
		values_offset = (values_offset + 1) % 100;
		ImGui::PlotLines("##Frametime", m_FrameTimeGraph, 100, values_offset, "Frametime (ms)", 0.0f, 66.6f, ImVec2(0, 100));
		ImGui::Text("Frametime: %.2f", GrizzlyBear::Core::get_timestep().get_millis());

		ImGui::End();

		ImGui::PopFont();
	}

	void EditorDefaultMode::on_dockspace_menu_bar()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create"))
			{
			}

			if (ImGui::MenuItem("Load"))
			{
			}

			if (ImGui::MenuItem("Save"))
			{
				m_SceneManager->save_scene(m_SceneManager->get_scene_name());
			}

			if (ImGui::MenuItem("Save As..."))
			{
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Graphics"))
		{
			if (ImGui::MenuItem("Wire Mode", "", m_WireMode))
			{
				if (!m_WireMode) m_Window->get_context()->enable_wire_mode(5.0f);
				if (m_WireMode) m_Window->get_context()->disable_wire_mode();

				m_WireMode = !m_WireMode;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Play"))
		{

			if (ImGui::MenuItem("Simulate Game"))
			{
				m_SceneManager->get_scene()->start_runtime();
			}

			if (ImGui::MenuItem("Stop Simulation"))
			{
				m_SceneManager->get_scene()->stop_runtime();
			}

			ImGui::EndMenu();
		}
	}

	bool EditorDefaultMode::on_mouse_pressed(const GrizzlyBear::MouseCode& e)
	{
		if (!m_SceneManager->is_scene_set()) return false;

		if (e.MOUSE_CODE == GRIZZLY_MOUSE_BUTTON_2 && m_GameEditor->get_window_focused())
		{
			m_Window->set_mouse_grabbed(true);
			return true;
		}

		if (e.MOUSE_CODE == GRIZZLY_MOUSE_BUTTON_1)
		{
			glm::vec2 rawPos = m_Window->get_mouse_pos();

			rawPos += m_Window->get_position();

			if (rawPos.x > m_GameEditor->get_content_position().x && rawPos.x < m_GameEditor->get_content_position().x + m_GameEditor->get_content_size().x && rawPos.y > m_GameEditor->get_content_position().y && rawPos.y < m_GameEditor->get_content_position().y + m_GameEditor->get_content_size().y)
			{
				glm::vec2 pos = GrizzlyBear::Phys75::screen_to_world({ rawPos.x - m_GameEditor->get_content_position().x, rawPos.y - m_GameEditor->get_content_position().y }, { m_GameEditor->get_content_size().x, m_GameEditor->get_content_size().y }, m_CamController->get_bounds(), m_CamController->get_position());

				auto scene = m_SceneManager->get_scene();

				for (auto it = scene->create_iterator<GrizzlyBear::CompMesh2D>(); it != scene->get_end(); it++)
				{
					it.get<GrizzlyBear::CompMesh2D>()->m_Material.HIGHLIGHT = false;
				}

				float topdepth = -1.0f;
				int   topindex = -1;

				for (auto it = scene->create_iterator<GrizzlyBear::CompMesh2D, GrizzlyBear::CompTransformation2D>(); it != scene->get_end(); it++)
				{
					auto mesh = it.get<GrizzlyBear::CompMesh2D>();
					auto transform = it.get<GrizzlyBear::CompTransformation2D>();

					float hsizeX = transform->m_Size.x / 2.0f;
					float hSizeY = transform->m_Size.y / 2.0f;

					if (pos.x > transform->m_Position.x - hsizeX  && pos.x < transform->m_Position.x + hsizeX && pos.y > transform->m_Position.y - hSizeY && pos.y < transform->m_Position.y + hSizeY)
					{
						if (transform->m_Position.z > topdepth)
						{
							topdepth = transform->m_Position.z;
							topindex = it.get_index();
						}
					}
				}

				if (topindex != -1)
				{
					scene->get_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, topindex)->m_Material.HIGHLIGHT = true;
					get_editor_window<ObjectEditorWindow>("Object Editor")->set_current_entity(topindex);
					return true;
				}
				else
				{
					get_editor_window<ObjectEditorWindow>("Object Editor")->set_current_entity(GrizzlyBear::EntityHandle::none());
					return false;
				}
			}

			return false;
		}

		return false;
	}

	bool EditorDefaultMode::on_mouse_released(const GrizzlyBear::MouseCode& e)
	{
		if (e.MOUSE_CODE == GRIZZLY_MOUSE_BUTTON_2)
		{
			m_Window->set_mouse_grabbed(false);
			return true;
		}

		return false;
	}

	bool EditorDefaultMode::on_key_pressed(const GrizzlyBear::KeyCode& e)
	{
		if (e.KEY_CODE == GRIZZLY_KEY_S)
		{
			if (m_Window->is_key_pressed(GRIZZLY_KEY_LEFT_CONTROL) || m_Window->is_key_pressed(GRIZZLY_KEY_RIGHT_CONTROL))
			{
				m_SceneManager->save_scene(m_SceneManager->get_scene_name());
			}
		}

		if (e.KEY_CODE == GRIZZLY_KEY_R && m_Window->is_key_pressed(GRIZZLY_KEY_LEFT_CONTROL))
		{
			m_CamController->set_position({ 0.0f, 0.0f, 0.0f });
			m_CamController->set_zoom_level(1.0f);
		}

		return true;
	}

}
