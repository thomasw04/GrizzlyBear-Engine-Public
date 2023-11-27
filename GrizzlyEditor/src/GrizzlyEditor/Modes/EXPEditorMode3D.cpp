#include "EXPEditorMode3D.h"

#include <imgui/imgui.h>

namespace GrizzlyEditor {

	EXPEditorMode3D::EXPEditorMode3D()
	{
		//m_Windows.insert({ "Game", std::make_shared<GameEditorWindow>(true) });
	}

	/*

	void EXPEditorMode3D::OnActivate()
	{
		m_FrameBuffer = ResourceManager::AllocateResource(FrameBuffer, 1920, 1080);

		m_FrameBuffer->AttachTexture2D(0);
		m_FrameBuffer->AttachRenderBuffer(0);

		m_FrameBuffer->Unbind();

		Engine::Window::SetViewport(1920, 1080);

		if(SceneManager::PushScene("3DEXPLevel"))
		{
			auto curr_scene = SceneManager::GetScene();

			uin32_t cameraHandle = curr_scene->AddEntity();

			curr_scene->SetComponent<Component::Camera>(cameraHandle, m_CamController.GetProjection());

			uint32_t cube = curr_scene->AddEntity();
			uint32_t cube2 = curr_scene->AddEntity();

			curr_scene->SetComponent<Component::Transformation3D>(cube, glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 40.0f, 5.0f, 40.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ));
			curr_scene->SetComponent<Component::Transformation3D>(cube2, glm::vec3(10.0f, 10.0f, -1.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(45.0f, 45.0f, 45.0f));

			curr_scene->SetCamera(cameraHandle);

			AssetManager::RequestAsset(GAMenum::Texture2D, "assets/textures/Chessboard.png");
		}
	}


	*/

	void EXPEditorMode3D::on_activate()
	{
		//m_FrameBuffer = GrizzlyBear::Renderer::get_context()->create_frame_buffer(1920, 1080);
		//m_RenderBuffer = GrizzlyBear::Renderer::get_context()->create_render_buffer(1920, 1080, GRIZZLY_INTERNAL_DEPTH24);
		//m_SceneTexture = GrizzlyBear::Renderer::get_context()->create_texture_2d(1920, 1080);
		
		/*
		m_FrameBuffer->attach_texture_2d(m_SceneTexture, 0);
		m_FrameBuffer->attach_render_buffer(m_RenderBuffer, 0);

		m_FrameBuffer->unbind(1920, 1080);

		if (!GrizzlyBear::SceneManager::exist_scene("3DEXPLevel"))
		{
			GrizzlyBear::SceneManager::push_scene("3DEXPLevel", "");

			auto curr_scene = GrizzlyBear::SceneManager::get_scene();

			GrizzlyBear::EntityHandle cameraHandle = curr_scene->add_entity();

			//curr_scene->set_component<GrizzlyBear::CompCamera>(cameraHandle, m_CamController.get_projection());

			GrizzlyBear::EntityHandle cube = curr_scene->add_entity();
			GrizzlyBear::EntityHandle cube2 = curr_scene->add_entity();

			//curr_scene->set_component<GrizzlyBear::CompTransformation3D>(cube, glm::vec3( 1.0f, 1.0f, -1.0f ), glm::vec3( 40.0f, 5.0f, 40.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ));
			//curr_scene->set_component<GrizzlyBear::CompTransformation3D>(cube2, glm::vec3(10.0f, 10.0f, -1.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(45.0f, 45.0f, 45.0f));

			curr_scene->set_camera(cameraHandle);

			//GrizzlyBear::AssetManager::request_asset(GrizzlyBear::Asset::Texture2D, "textures/Chessboard.png");
		}
		else
		{
			GrizzlyBear::SceneManager::set_scene("3DEXPLevel");
		}

		memset(m_FrameTimeGraph, 0, sizeof(float) * 100);

		*/

		//GrizzlyBear::Core::get_window()->on_mouse(GrizzlyBear::MouseEvent::PRESSED, GRIZZLY_BIND_EVENT(EXPEditorMode3D::on_mouse_pressed));
		//GrizzlyBear::Core::get_window()->on_mouse(GrizzlyBear::MouseEvent::RELEASED, GRIZZLY_BIND_EVENT(EXPEditorMode3D::on_mouse_released));
	}

	void EXPEditorMode3D::on_deactivate()
	{
		//GrizzlyBear::Core::get_window()->pop_mouse(GrizzlyBear::MouseEvent::PRESSED);
		//GrizzlyBear::Core::get_window()->pop_mouse(GrizzlyBear::MouseEvent::RELEASED);
	}

	void EXPEditorMode3D::on_update(GrizzlyBear::Timestep ts)
	{
		//m_CamController.on_update(ts);

	//	auto curr_scene = GrizzlyBear::SceneManager::get_scene();

		//curr_scene->do_render(ts, GrizzlyBear::Dimensions::THREE, m_CamController.get_projection(), m_CamController.calc_view(), m_FrameBuffer);
	}

	void EXPEditorMode3D::on_gui_update(unsigned int dockspaceID)
	{
		get_editor_window<GameEditorWindow>("Game")->get_scene_id() = m_SceneTexture->get_render_id();

		WindowManager::update_docked_window("Game", m_Windows.at("Game"), dockspaceID);

		ImGui::Begin("Frame");

		m_FrameTimeGraph[values_offset] = GrizzlyBear::Core::get_timestep().get_millis();
		values_offset = (values_offset + 1) % 100;
		ImGui::PlotLines("##Frametime", m_FrameTimeGraph, 100, values_offset, "Frametime (ms)", 0.0f, 66.6f, ImVec2(0, 100));
		ImGui::Text("Frametime: %.2f", GrizzlyBear::Core::get_timestep().get_millis());

		ImGui::End();
	}

	void EXPEditorMode3D::on_dockspace_menu_bar()
	{
	}

	bool EXPEditorMode3D::on_mouse_pressed(const GrizzlyBear::MouseCode& e)
	{
		if (e.MOUSE_CODE == GRIZZLY_MOUSE_BUTTON_2)
		{
			//GrizzlyBear::Core::get_window()->set_mouse_grabbed(true);
			return true;
		}

		return false;
	}
	bool EXPEditorMode3D::on_mouse_released(const GrizzlyBear::MouseCode& e)
	{
		if (e.MOUSE_CODE == GRIZZLY_MOUSE_BUTTON_2)
		{
		//	GrizzlyBear::Core::get_window()->set_mouse_grabbed(false);
			return true;
		}

		return false;
	}
}
