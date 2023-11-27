#pragma once

#include <GrizzlyBear.h>

#include <GrizzlyEditor/EditorApplication.h>
#include <GrizzlyEditor/EditorWindow/WindowManager.h>
#include <GrizzlyEditor/EditorWindow/Windows/GameEditorWindow.h>

namespace GrizzlyEditor {

	class EXPEditorMode3D : public EditorMode
	{
	public:
		EXPEditorMode3D();

		virtual void on_activate() override;
		virtual void on_deactivate() override;

		virtual void on_update(GrizzlyBear::Timestep ts) override;

		virtual void on_gui_update(unsigned int dockspaceID) override;

		virtual void on_dockspace_menu_bar() override;

		virtual EditorType get_type() override { return EditorType::EXPERIMENTAL_3D_VWA; }

	private:
		bool on_mouse_pressed(const GrizzlyBear::MouseCode& e);
		bool on_mouse_released(const GrizzlyBear::MouseCode& e);
	private:
		GrizzlyBear::Ref<GrizzlyBear::FrameBuffer> m_FrameBuffer;
		GrizzlyBear::Ref<GrizzlyBear::RenderBuffer> m_RenderBuffer;
		GrizzlyBear::Ref<GrizzlyBear::Texture2D> m_SceneTexture;

		float m_FrameTimeGraph[100] = { 0.0f };
		int values_offset = 0;
	};


}
