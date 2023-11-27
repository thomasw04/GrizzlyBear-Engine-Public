#include "EditorPolygonMode.h"

#include <GrizzlyBear.h>

#include "GrizzlyEditor/EditorWindow/Windows/GameEditorWindow.h"

namespace GrizzlyEditor {

	EditorPolgonMode::EditorPolgonMode()
	{
		//m_Windows.insert({ "Game", std::make_shared<GameEditorWindow>(true) });
	}

	void EditorPolgonMode::on_activate()
	{/*
		m_FrameBuffer = GrizzlyBear::Renderer::get_context()->create_frame_buffer(1920, 1080);
		m_RenderBuffer = GrizzlyBear::Renderer::get_context()->create_render_buffer(1920, 1080, GRIZZLY_INTERNAL_DEPTH24);
		m_SceneTexture = GrizzlyBear::Renderer::get_context()->create_texture_2d(1920, 1080);

		m_FrameBuffer->attach_texture_2d(m_SceneTexture, 0);
		m_FrameBuffer->attach_render_buffer(m_RenderBuffer, 0);

		m_FrameBuffer->unbind(1920, 1080);
		*/
	}

	void EditorPolgonMode::on_deactivate()
	{
	}

	void EditorPolgonMode::on_update(GrizzlyBear::Timestep ts)
	{/*
		m_CamController.on_update(ts);

		GrizzlyBear::Renderer::begin_frame(m_FrameBuffer);

		GrizzlyBear::LineRenderer2D::begin_scene(m_CamController.get_projection(), m_CamController.calc_view());

		GrizzlyBear::LineRenderer2D::draw_line({ 0, 0 }, { 100, 100 }, { 1.0f, 1.0f, 1.0f, 1.0f });

		GrizzlyBear::LineRenderer2D::end_scene();

		GrizzlyBear::Renderer::end_frame();
		*/
	}

	void EditorPolgonMode::on_gui_update(unsigned int dockspaceID)
	{
		get_editor_window<GameEditorWindow>("Game")->get_scene_id() = m_SceneTexture->get_render_id();

		WindowManager::update_docked_window("Game", m_Windows.at("Game"), dockspaceID);
	}

	void EditorPolgonMode::on_dockspace_menu_bar()
	{
	}
}
