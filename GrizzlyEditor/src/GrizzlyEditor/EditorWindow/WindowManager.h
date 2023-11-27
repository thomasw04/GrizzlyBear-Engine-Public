#pragma once

#include <imgui/imgui.h>
#include <string>
#include <memory>
#include <functional>

#include "GrizzlyBear/Addon/ImGuiAddon.h"
#include "GrizzlyBear/Core/Log.h"

#include "GrizzlyBear/API/Window.h"

#include "GrizzlyBear/Template/SceneManager.h"

namespace GrizzlyEditor {

	using ImParamFunc = std::function<void()>;

	class EditorWindow
	{
		friend class WindowManager;

	public:
		EditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window) : m_Open(open), m_Focused(false), m_Hovered(false), m_Position(ImVec2(0, 0)), m_Size(ImVec2(100, 100)), m_ContentPos(ImVec2(0, 0)), m_ContentSize(ImVec2(0, 0)), m_Window(window) {}
		EditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window) : m_Open(open), m_Focused(false), m_Hovered(false), m_Position(position), m_Size(size),
																																		m_ContentPos(ImVec2(0, 0)), m_ContentSize(ImVec2(0, 0)), m_Window(window) {}
		virtual ~EditorWindow() {};

		bool get_window_focused() { return m_Focused; };
		
		bool get_window_hovered() { return m_Hovered; }

		bool& get_window_open() { return m_Open; };

		ImVec2& get_window_position() { return m_Position; };
		ImVec2& get_window_size() { return m_Size; };

		ImVec2& get_content_position() { return m_ContentPos; }
		ImVec2& get_content_size() { return m_ContentSize; }

		virtual void render_window() = 0;

		virtual void reset_window() = 0;

	protected:
		bool m_Open;
		bool m_Focused;
		bool m_Hovered;

		ImVec2 m_Position;
		ImVec2 m_Size;

		ImVec2 m_ContentPos;
		ImVec2 m_ContentSize;

		GrizzlyBear::Ref<GrizzlyBear::Window> m_Window;
	};

	class WindowManager
	{
	public:

		static void update_window(const std::string& name, std::shared_ptr<EditorWindow> window, ImGuiWindowFlags flags = 0, bool forceFocus = false, ImParamFunc before = nullptr, ImParamFunc after = nullptr);

		static void update_docked_window(const std::string& name, std::shared_ptr<EditorWindow> window, unsigned int dockspaceID, ImGuiWindowFlags flags = 0, bool forceFocus = false, ImParamFunc before = nullptr, ImParamFunc = nullptr);
	};

}
