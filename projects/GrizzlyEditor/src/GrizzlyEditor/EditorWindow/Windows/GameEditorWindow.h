#pragma once

#include <GrizzlyEditor/EditorWindow/WindowManager.h>

namespace GrizzlyEditor {

	class GameEditorWindow : public EditorWindow
	{
	public:
		GameEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		GameEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager);
		virtual ~GameEditorWindow();

		uint32_t& get_scene_id() { return m_SceneID; };

		virtual void render_window() override;

		virtual void reset_window() override;

	private:
		uint32_t m_SceneID = 0;

		GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> m_SceneManager;
	};

}
