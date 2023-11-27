#pragma once

#include "GrizzlyEditor/EditorApplication.h"

namespace GrizzlyEditor {

	class EditorPolgonMode : public EditorMode
	{
	public:
		EditorPolgonMode();

		virtual void on_activate() override;
		virtual void on_deactivate() override;

		virtual void on_update(GrizzlyBear::Timestep ts) override;

		virtual void on_gui_update(unsigned int dockspaceID) override;

		virtual void on_dockspace_menu_bar() override;

		inline virtual EditorType get_type() override { return EditorType::EXPERIMENTAL_POLYGON; }

	private:
		GrizzlyBear::Ref<GrizzlyBear::FrameBuffer> m_FrameBuffer;
		GrizzlyBear::Ref<GrizzlyBear::RenderBuffer> m_RenderBuffer;
		GrizzlyBear::Ref<GrizzlyBear::Texture2D> m_SceneTexture;

	private:
		//bool on_mouse_pressed(GrizzlyBear::MouseButtonPressedEvent& e);
		//bool on_mouse_released(GrizzlyBear::MouseButtonReleasedEvent& e);

		//bool on_key_pressed(GrizzlyBear::KeyPressedEvent& e);

	};

}
