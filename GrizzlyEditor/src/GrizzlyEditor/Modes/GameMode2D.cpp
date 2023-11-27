#include "GameMode2D.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace GrizzlyEditor {

	GameMode2D::GameMode2D() {}

	void GameMode2D::on_activate()
	{
		m_Particle.ColorBegin = { 100 / 255.0f, 200 / 255.0f, 255 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.2f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 10.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 2.0f, 2.0f };
		m_Particle.Position = { 0.0f, 0.0f, 0.0f};

		memset(m_FrameTimeGraph, 0, sizeof(float) * 100);
	}

	void GameMode2D::on_deactivate()
	{
	}

	void GameMode2D::on_update(GrizzlyBear::Timestep ts)
	{
		//m_ParticleSystem.on_update(ts);
		//m_CamController.on_update(ts);

		//Prepare Screen -----------------------------------------------------------------------------------------------------------------------

		//GrizzlyBear::Renderer::get_context()->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		//GrizzlyBear::Renderer::get_context()->clear();

		//Render to editor window --------------------------------------------------------------------------------------------------------------

		//m_SceneID = GrizzlyBear::ModuleRender::Renderer2D::SetRenderToTexture(1920, 1080);

		//GrizzlyBear::ModuleRender::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		//GrizzlyBear::ModuleRender::RenderCommand::Clear();

		//--------------------------------------------------------------------------------------------------------------------------------------

		//GrizzlyBear::ModuleScene::SceneStack::GetScene()->OnRender2D();

		//GrizzlyBear::Renderer2D::BeginScene(EditorApp::GetSceneStack()->GetScene());

		//RENDER THINGS HERE -------------------------------------------------------------------------------------------------------------------

		//m_ParticleSystem.OnRender();

		//RENDER THINGS HERE -------------------------------------------------------------------------------------------------------------------

		//GrizzlyBear::Renderer2D::EndScene();

		//Render to editor window --------------------------------------------------------------------------------------------------------------

		//TODO: Standalone mode

		//if (!GrizzlyBear::Application::Get().GetStandalone())
		{
			//GrizzlyBear::ModuleRender::Renderer2D::SetRenderToScreen(GrizzlyBear::Core::GetWindow()->GetWidth(), GrizzlyBear::Core::GetWindow()->GetHeight());
		}

		//--------------------------------------------------------------------------------------------------------------------------------------

		/*if (GrizzlyBear::Core::get_window()->is_mouse_pressed(GRIZZLY_MOUSE_BUTTON_1))
		{
			glm::vec2 pos = GrizzlyBear::Core::get_window()->get_mouse_pos();
			auto width = GrizzlyBear::Core::get_window()->get_width();
			auto height = GrizzlyBear::Core::get_window()->get_height();

			//m_Particle.Position = GrizzlyBear::Phys75::ConvertScreenToWorld({ pos.x, pos.y }, { width, height }, m_CamController);

			for (int i = 0; i < 5; i++)
			{
				m_ParticleSystem.emit(m_Particle);
			}
		}*/

		//GrizzlyBear::SceneManager::get_scene()->get_component<GrizzlyBear::CompCamera>(GrizzlyBear::SceneManager::get_scene()->get_camera())->m_ProjectionMatrix = m_CamController.get_projection();

		return;
	}

	void GameMode2D::on_gui_update(unsigned int dockspaceID)
	{
		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_Always);

		//ImGui::SetNextWindowSize(ImVec2(GrizzlyBear::Core::get_window()->get_width(), GrizzlyBear::Core::get_window()->get_height()), ImGuiCond_Always);

		ImGui::Begin("Game Window##PlayMode", (bool*)0);

		ImGui::Image((void*)(uintptr_t)m_SceneID, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();

		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);

		ImGui::Begin("Frame");

		m_FrameTimeGraph[values_offset] = GrizzlyBear::Core::get_timestep().get_millis();
		values_offset = (values_offset + 1) % 100;
		ImGui::PlotLines("##Frametime", m_FrameTimeGraph, 100, values_offset, "Frametime (ms)", 0.0f, 66.6f, ImVec2(0, 100));
		ImGui::Text("Frametime: %.2f", GrizzlyBear::Core::get_timestep().get_millis());

		ImGui::End();
	}

	
	void GameMode2D::on_dockspace_menu_bar()
	{
	}

}


