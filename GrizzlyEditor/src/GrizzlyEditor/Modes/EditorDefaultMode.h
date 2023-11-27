#pragma once

#include <GrizzlyBear.h>

#include <GrizzlyEditor/EditorApplication.h>

#include <GrizzlyEditor/EditorWindow/WindowManager.h>

#include <GrizzlyEditor/EditorWindow/Windows/LevelEditorWindow.h>
#include <GrizzlyEditor/EditorWindow/Windows/ObjectEditorWindow.h>
#include <GrizzlyEditor/EditorWindow/Windows/GameEditorWindow.h>
#include <GrizzlyEditor/EditorWindow/Windows/ResolutionEditorWindow.h>
#include <GrizzlyEditor/EditorWindow/Windows/AssetManagerEditorWindow.h>
#include <GrizzlyEditor/EditorWindow/Windows/HierarchyEditorWindow.h>

namespace GrizzlyEditor {

	class EditorDefaultMode : public EditorMode
	{
	public:
		EditorDefaultMode(GrizzlyBear::Ref<GrizzlyBear::Window> window);

		virtual void on_activate() override;
		virtual void on_deactivate() override;

		virtual void on_update(GrizzlyBear::Timestep ts) override;

		virtual void on_gui_update(unsigned int dockspaceID) override;

		virtual void on_dockspace_menu_bar() override;

		inline virtual EditorType get_type() override { return EditorType::STABLE_DEFAULT; }

	private:
		std::shared_ptr<LevelEditorWindow> m_LevelEditor;
		std::shared_ptr<ObjectEditorWindow> m_ObjectEditor;
		std::shared_ptr<GameEditorWindow> m_GameEditor;
		std::shared_ptr<ResolutionEditorWindow> m_ResolutionEditor;
		std::shared_ptr<AssetManagerEditorWindow> m_AssetManagerEditor;
		std::shared_ptr<HierarchyEditorWindow> m_HierarchyEditor;


		GrizzlyBear::Ref<GrizzlyBear::Window> m_Window;
		GrizzlyBear::Ref<GrizzlyBear::Renderer> m_Renderer;
		GrizzlyBear::Ref<GrizzlyBear::Renderer2D> m_Renderer2D;

		GrizzlyBear::Ref<GrizzlyBear::FrameBuffer> m_FrameBuffer;
		GrizzlyBear::Ref<GrizzlyBear::RenderBuffer> m_RenderBuffer;
		GrizzlyBear::Ref<GrizzlyBear::Texture2D> m_SceneTexture;

		GrizzlyBear::Ref<GrizzlyBear::Template::OrthographicCamera> m_CamController;
		GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> m_SceneManager;

		GrizzlyBear::ParticleProps m_ChristmasProps;
		GrizzlyBear::Ref<GrizzlyBear::ParticleSystem2D> m_ChristmasParticles;

		float layerCounter = 0.0f;

		bool m_WireMode = false;

		float m_FrameTimeGraph[100] = { 0.0f };
		int values_offset = 0;

		bool up = true;
	private:
		bool on_mouse_pressed(const GrizzlyBear::MouseCode& e);
		bool on_mouse_released(const GrizzlyBear::MouseCode& e);
		bool on_key_pressed(const GrizzlyBear::KeyCode& e);

	};

}
