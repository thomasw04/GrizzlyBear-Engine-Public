#pragma once

#include <GrizzlyEditor/EditorWindow/WindowManager.h>

#include <imgui/misc/cpp/imgui_stdlib.h>

namespace GrizzlyEditor {

	class LevelEditorWindow : public EditorWindow
	{
	public:
		LevelEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window);
		LevelEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window);
		virtual ~LevelEditorWindow();

		virtual void render_window() override;

		virtual void reset_window() override;

	private:

		std::string m_LevelScriptFilePath = "";
	};
}