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

		m_LevelEditor = insert_and_return<LevelEditorWindow>("Level Editor", std::make_shared<LevelEditorWindow>(true, window));
		m_ObjectEditor = insert_and_return<ObjectEditorWindow>("Object Editor", std::make_shared<ObjectEditorWindow>(true, window, m_SceneManager));
		m_GameEditor = insert_and_return<GameEditorWindow>("Game Editor", std::make_shared<GameEditorWindow>(true, window, m_SceneManager));
		m_ResolutionEditor = insert_and_return<ResolutionEditorWindow>("Resolution Editor", std::make_shared<ResolutionEditorWindow>(false, window));
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

		GrizzlyBear::Ref<GrizzlyBear::Texture> m_Texture = GrizzlyBear::AssetManager::wait_for_imm_texture(m_Window->get_context(), "assets/textures/snowflake.png");

		m_ChristmasParticles = std::make_shared<GrizzlyBear::ParticleSystem2D>(m_Renderer2D, m_Texture);

		GrizzlyBear::ParticleProps props;

		props.ColorBegin = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		props.ColorEnd = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		props.LifeTime = 10.0f;
		props.SizeBegin = 2.0f;
		props.SizeEnd = 0.5f;
		props.SizeVariation = 0.3f;
		props.Velocity = glm::vec2(-0.5f, 0.0f);
		props.VelocityVariation = glm::vec2(1.0f, 1.0f);

		m_ChristmasProps = props;

		//m_CamController->get_position() = glm::vec3(50.0f, 50.0f, 0.0f);
		//m_CamController->get_zoom_level() = 20.0f;

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

		if (!m_SceneManager->is_scene_set() || m_SceneManager->get_scene_name() == "Test")
		{
			if (layerCounter >= 1.0f) layerCounter = 0.0f;

			m_ChristmasProps.Position = glm::vec3(GrizzlyBear::Random::rand_float() * 100.0f, GrizzlyBear::Random::rand_float() * 100.0f, layerCounter);
			layerCounter += 0.0001;

			m_CamController->on_update(ts);

			m_Renderer2D->begin_scene(m_CamController->get_projection(), m_CamController->calc_view());

			m_Renderer2D->draw_point_light({ 0.0f, 0.0f, 5.0f }, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

			m_ChristmasParticles->emit(m_ChristmasProps);

			m_ChristmasParticles->on_update(ts);
			m_ChristmasParticles->on_render();

			m_Renderer2D->end_scene();
		}
		else
		{
			m_SceneManager->on_update(ts);
		}

		m_Renderer->end_frame();
	}

	void EditorDefaultMode::on_gui_update(unsigned int dockspaceID)
	{
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

		WindowManager::update_docked_window("Level Editor", m_LevelEditor, dockspaceID);
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

		WindowManager::update_window("Resolution", m_ResolutionEditor, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize, false, resolutionBefore, resolutionAfter);

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
			/*	GrizzlyBear::Gstring filepath;

				if (GrizzlyBear::Platform::save_file(filepath, "Save As...", "", "gcs"))
				{
					GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Adding scene...");

					GrizzlyBear::FilePath path(filepath);

					if (!GrizzlyBear::SceneManager::push_scene(path.get_name_woe(), path.get_path()))
					{
						GRIZZLY_CLIENT_LOG(GrizzlyBear::LogLevel::Error, "Scene {0} already exists! Please use load to load the scene!", path.get_name_woe());
					}
					else
					{
						GrizzlyBear::SceneManager::load_scene();

						GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Info, "Successfully added scene. Saving...");

						GrizzlyBear::SceneManager::serialize();
					}
				}*/
			}

			if (ImGui::MenuItem("Load"))
			{
			/*	GrizzlyBear::Gstring filepath;

				if (GrizzlyBear::Platform::get_file(filepath, "Load a Level", "", "gcs"))
				{
					GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Loading scene...");

					GrizzlyBear::FilePath path(filepath);

					try {
						if (GrizzlyBear::SceneManager::exist_scene(path.get_name_woe()) || GrizzlyBear::SceneManager::push_scene(path.get_name_woe(), filepath))
						{
							GrizzlyBear::SceneManager::set_scene(path.get_name_woe());

							GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Info, "Successfully loaded scene.");
						}
						else
						{
							GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Error while loading scene. Please try again.");
						}
					}
					catch (std::exception& e)
					{
						GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Error while loading scene. Please try again.");
					}
				}*/
			}

			if (ImGui::MenuItem("Save"))
			{
				m_SceneManager->save_scene(m_SceneManager->get_scene_name());
			}

			//if(ImGui::MenuItem("Save")) 
			if (ImGui::MenuItem("Save As..."))
			{
			/*	GrizzlyBear::Gstring filepath;

				if (GrizzlyBear::Platform::save_file(filepath, "Save As...", "", "gcs"))
				{
					GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Saving scene...");

					try {
						if (GrizzlyBear::SceneManager::get_scene()->serialize(filepath))
						{
							GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Info, "Successfully saved scene.");
						}
						else
						{
							GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Error while saving scene. Please try again.");
						}
					}
					catch (std::exception& e)
					{
						GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Error while saving scene. Please try again.");
					}
				}*/
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Graphics"))
		{
			if (ImGui::MenuItem("Change Resolution", "", m_ResolutionEditor->get_window_open())) { m_ResolutionEditor->get_window_open() = true; }

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

	/*bool EditorDefaultMode::on_event(GrizzlyBear::Event& event)
	{
		GrizzlyBear::EventDispatcher dispatcher(event);
		dispatcher.dispatch<GrizzlyBear::MouseButtonPressedEvent>(GRIZZLY_BIND_EVENT_FN(EditorDefaultMode::on_mouse_pressed));
		dispatcher.dispatch<GrizzlyBear::MouseButtonReleasedEvent>(GRIZZLY_BIND_EVENT_FN(EditorDefaultMode::on_mouse_released));
		dispatcher.dispatch<GrizzlyBear::KeyPressedEvent>(GRIZZLY_BIND_EVENT_FN(EditorDefaultMode::on_key_pressed));

		if (m_GameEditor->get_window_focused())
		{
			m_CamController.on_event(event);
		}

		return true;
	}*/

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

		if (e.KEY_CODE == GRIZZLY_KEY_I)
		{
			auto currentScene = m_SceneManager->get_scene();

			currentScene->get_animator_2d()->play_animation("TestAnim", 1);
		}

		if (e.KEY_CODE == GRIZZLY_KEY_R && m_Window->is_key_pressed(GRIZZLY_KEY_LEFT_CONTROL))
		{
			m_CamController->set_position({ 0.0f, 0.0f, 0.0f });
			m_CamController->set_zoom_level(1.0f);
		}

		/*if (e.get_key_code() == GRIZZLY_KEY_I)
		{
			GrizzlyBear::Ref<GrizzlyBear::Scene> defaultScene = GrizzlyBear::SceneManager::get_scene();

			GrizzlyBear::CompMesh2D* mesh = defaultScene->get_component<GrizzlyBear::CompMesh2D>(3);

			mesh->m_AnimatedTex.m_Animation.m_State.m_Paused = false;
		}

		if (e.get_key_code() == GRIZZLY_KEY_J)
		{
			GrizzlyBear::Ref<GrizzlyBear::Scene> defaultScene = GrizzlyBear::SceneManager::get_scene();

			GrizzlyBear::CompMesh2D* mesh = defaultScene->get_component<GrizzlyBear::CompMesh2D>(3);

			mesh->m_AnimatedTex.m_Animation.m_State.m_Paused = true;
		}

		if (e.get_key_code() == GRIZZLY_KEY_R)
		{
			GrizzlyBear::Ref<GrizzlyBear::Scene> defaultScene = GrizzlyBear::SceneManager::get_scene();

			GrizzlyBear::CompMesh2D* mesh = defaultScene->get_component<GrizzlyBear::CompMesh2D>(3);

			mesh->m_AnimatedTex.m_Animation.m_State.m_CurrentFrame = 0;
		}*/

		return true;
	}

}
