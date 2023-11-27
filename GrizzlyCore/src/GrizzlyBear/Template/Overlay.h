#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>
#include <GrizzlyBear/Core/Event.h>
#include <GrizzlyBear/API/Window.h>

#include <stack>

namespace GrizzlyBear {


	class Overlay
	{
	public:

		Overlay(Ref<Window> window);
		~Overlay();

		virtual void on_update(Timestep ts) = 0;

	public:

		void on_mouse_moved(std::function<bool(const MouseMovedEvent&)> func);

		void on_mouse_scrolled(std::function<bool(const MouseScrolledEvent&)> func);

		void on_window_resize(std::function<bool(const WindowResizeEvent&)> func);

		void on_window_move(std::function<bool(const WindowMoveEvent&)> func);

		void on_key(KeyEvent listen, std::function<bool(const KeyCode&)> func);

		void on_mouse(MouseEvent listen, std::function<bool(const MouseCode&)> func);

		void on_closed(std::function<bool()> func);

	private:

		bool m_KeyPressed, m_KeyReleased, m_KeyTyped;
		bool m_MousePressed, m_MouseReleased, m_MouseTyped;

		std::stack<Window::EventPopFunc> m_PopFuncs;

		Ref<Window> m_Window;
	};
}

