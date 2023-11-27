#pragma once

#include <GrizzlyBear.h>

#include <GrizzlyEditor/EditorApplication.h>

namespace GrizzlyEditor {

	class GameMode2D : public EditorMode
	{
	public:
		GameMode2D();

		virtual void on_activate() override;
		virtual void on_deactivate() override;

		virtual void on_update(GrizzlyBear::Timestep ts) override;

		virtual void on_gui_update(unsigned int dockspaceID) override;

		virtual void on_dockspace_menu_bar() override;

		virtual EditorType get_type() override { return EditorType::STABLE_PLAY; }

	private:
		//GrizzlyBear::ParticleSystem2D m_ParticleSystem;
		GrizzlyBear::ParticleProps m_Particle;

		float m_FrameTimeGraph[100];
		int values_offset = 0;

		int m_SceneID;

		bool on_mouse_pressed(GrizzlyBear::MouseCode& e);

	};

}
