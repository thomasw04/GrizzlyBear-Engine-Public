#pragma once

#include <GrizzlyEditor/EditorWindow/WindowManager.h>

#include "ObjectEditorWindow.h"

#include <imgui/imgui.h>

namespace GrizzlyEditor {

	class HierarchyEditorWindow : public EditorWindow
	{
	public:
		HierarchyEditorWindow(bool open, std::shared_ptr<ObjectEditorWindow> objectWindow, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		HierarchyEditorWindow(bool open, const ImVec2& position, const ImVec2& size, std::shared_ptr<ObjectEditorWindow> objectWindow, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		virtual ~HierarchyEditorWindow();

		virtual void render_window() override;

		virtual void reset_window() override;

	private:
		bool open_action_popup();

	private:
		std::shared_ptr<ObjectEditorWindow> m_ObjectWindow;

		std::string m_InputBuf;

		GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> m_SceneManager;
	};
}