#include <gbpch.h>

#include "Overlay.h"

namespace GrizzlyBear {

	//Grizzly Application

	Overlay::Overlay(Ref<Window> window)
		: m_Window(window)
	{
	}

	Overlay::~Overlay()
	{
		if (m_KeyPressed) m_Window->pop_key(KeyEvent::PRESSED);
		if (m_KeyReleased) m_Window->pop_key(KeyEvent::RELEASED);
		if (m_KeyTyped) m_Window->pop_key(KeyEvent::TYPED);

		while (!m_PopFuncs.empty())
		{
			m_PopFuncs.top()(m_Window);
			m_PopFuncs.pop();
		}
	}

	void Overlay::on_mouse_moved(std::function<bool(const MouseMovedEvent&)> func)
	{
		m_PopFuncs.push(m_Window->on_mouse_moved(func));
	}

	void Overlay::on_mouse_scrolled(std::function<bool(const MouseScrolledEvent&)> func)
	{
		m_PopFuncs.push(m_Window->on_mouse_scrolled(func));
	}

	void Overlay::on_window_resize(std::function<bool(const WindowResizeEvent&)> func)
	{
		m_PopFuncs.push(m_Window->on_window_resize(func));
	}

	void Overlay::on_window_move(std::function<bool(const WindowMoveEvent&)> func)
	{
		m_PopFuncs.push(m_Window->on_window_move(func));
	}

	void Overlay::on_key(KeyEvent listen, std::function<bool(const KeyCode&)> func)
	{
		m_Window->on_key(listen, func);

		switch (listen)
		{
		case KeyEvent::PRESSED:
			m_KeyPressed = true;
			break;
		case KeyEvent::RELEASED:
			m_KeyReleased = true;
			break;
		case KeyEvent::TYPED:
			m_KeyTyped = true;
			break;
		}
	}

	void Overlay::on_mouse(MouseEvent listen, std::function<bool(const MouseCode&)> func)
	{
		m_Window->on_mouse(listen, func);

		switch (listen)
		{
		case MouseEvent::PRESSED:
			m_MousePressed = true;
			break;
		case MouseEvent::RELEASED:
			m_MouseReleased = true;
			break;
		}
	}

	void Overlay::on_closed(std::function<bool()> func)
	{
		m_PopFuncs.push(m_Window->on_closed(func));
	}
}

