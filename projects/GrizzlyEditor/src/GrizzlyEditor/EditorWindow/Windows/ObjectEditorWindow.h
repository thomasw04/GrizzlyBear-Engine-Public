#pragma once

#include <GrizzlyEditor/EditorWindow/WindowManager.h>

#include <GrizzlyBear/Scene/Scene.h>

#include <imgui/misc/cpp/imgui_stdlib.h>

namespace GrizzlyEditor {

	class ObjectEditorWindow : public EditorWindow
	{
	public:
		ObjectEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		ObjectEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		virtual ~ObjectEditorWindow();

		GrizzlyBear::EntityHandle get_current_entity() { return m_EntityHandle; }

		void set_current_entity(GrizzlyBear::EntityHandle entityHandle);

		virtual void render_window() override;

		virtual void reset_window() override;

	private:

		GrizzlyBear::EntityHandle m_EntityHandle = GrizzlyBear::EntityHandle::none();

		bool m_ComponentMesh2DEnabled = false;
		bool m_ComponentWidgetEnabled = false;
		bool m_ComponentScriptEnabled = false;
		bool m_ComponentCameraEnabled = true;

		bool m_ComponentMesh2DOpen = false;
		bool m_ComponentWidgetOpen = false;
		bool m_ComponentScriptOpen = false;
		bool m_ComponentCameraOpen = false;

		bool m_Mesh2DAnimationEnabled = false;

		std::string m_Mesh2DTextureFilePath = "";
		std::string m_WidgetTextureFilePath = "";
		std::string m_Mesh2DSpriteSheetFilePath = "";
		std::string m_ScriptFilePath = "";

		std::string m_EntityName = "";

		GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> m_SceneManager;
	};

}
