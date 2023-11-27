#pragma once

#include <GrizzlyEditor/EditorWindow/WindowManager.h>

namespace GrizzlyEditor {

	class ResolutionEditorWindow : public EditorWindow
	{
	public:
		ResolutionEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window);
		ResolutionEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window);
		virtual ~ResolutionEditorWindow();

		int GetXRes() { return m_ResX; }
		int GetYRes() { return m_ResY; }

		virtual void render_window() override;

		virtual void reset_window() override;

	private:

		int m_ResX = 1920;
		int m_ResY = 1080;

		int m_ResXImpl = 1920;
		int m_ResYImpl = 1080;
	};

}
