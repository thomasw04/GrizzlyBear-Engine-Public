#include <gbpch.h>

#include "OpenGLWindow.h"

#include "GrizzlyBear/Core/Event.h"

#include "GrizzlyBear/API/OpenGL/OpenGLContext.h"
#include "GrizzlyBear/API/OpenGL/OpenGLKeyCodes.h"

#include <GLFW/glfw3.h>

namespace GrizzlyBear {

	OpenGLWindow::OpenGLWindow(const Gstring& title, uint32_t width, uint32_t height)
		: m_Data({ title, width, height })
	{
		GRIZZLY_CORE_LOG(LogLevel::Trace, "Creating a new window - Title: {0}, Width: {1}, Height: {2})", m_Data.m_Title.c_str(), m_Data.m_Width, m_Data.m_Height);

		m_Window = glfwCreateWindow((int)m_Data.m_Width, (int)m_Data.m_Height, m_Data.m_Title.c_str(), nullptr, nullptr);

		m_Data.m_Context = std::make_shared<OpenGLContext>(m_Window);
		m_Data.m_KeyCodeConv = std::make_shared<OpenGLKeyCodeConv>();

		set_v_sync(VsyncMode::DISABLED);

		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.m_Width = width;
			data.m_Height = height;

			WindowResizeEvent event(width, height);

			for (auto it = data.m_WindowResizeEvents.rbegin(); it != data.m_WindowResizeEvents.rend(); it++)
			{
				if((*it)(event)) break;
			}
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			for (auto it = data.m_NoDataEvents[NoDataEvent::WINDOW_CLOSE].rbegin(); it != data.m_NoDataEvents[NoDataEvent::WINDOW_CLOSE].rend(); it++)
			{
				if((*it)()) break;
			}
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyCode event(data.m_KeyCodeConv->to_grizzly_key(key));

			switch (action)
			{
				case GLFW_PRESS:
				{
					for (auto it = data.m_KeyEvents[KeyEvent::PRESSED].rbegin(); it != data.m_KeyEvents[KeyEvent::PRESSED].rend(); it++)
					{
						if((*it)(event)) break;
					}
					break;
				}
				case GLFW_RELEASE:
				{
					for (auto it = data.m_KeyEvents[KeyEvent::RELEASED].rbegin(); it != data.m_KeyEvents[KeyEvent::RELEASED].rend(); it++)
					{
						if ((*it)(event)) break;
					}
					break;
				}
				case GLFW_REPEAT:
				{
					event.REPEAT = 1;

					for (auto it = data.m_KeyEvents[KeyEvent::PRESSED].rbegin(); it != data.m_KeyEvents[KeyEvent::PRESSED].rend(); it++)
					{
						if ((*it)(event)) break;
					}
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseCode event(data.m_KeyCodeConv->to_grizzly_mouse(button));

			switch (action)
			{
				case GLFW_PRESS:
				{
					for (auto it = data.m_MouseEvents[MouseEvent::PRESSED].rbegin(); it != data.m_MouseEvents[MouseEvent::PRESSED].rend(); it++)
					{
						if ((*it)(event)) break;
					}
					break;
				}
				case GLFW_RELEASE:
				{
					for (auto it = data.m_MouseEvents[MouseEvent::RELEASED].rbegin(); it != data.m_MouseEvents[MouseEvent::RELEASED].rend(); it++)
					{
						if ((*it)(event)) break;
					}
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyCode event(character, 0);

			for (auto it = data.m_KeyEvents[KeyEvent::TYPED].rbegin(); it != data.m_KeyEvents[KeyEvent::TYPED].rend(); it++)
			{
				if ((*it)(event)) break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);

			for (auto it = data.m_MouseScrolledEvents.rbegin(); it != data.m_MouseScrolledEvents.rend(); it++)
			{
				if((*it)(event)) break;
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);

			for (auto it = data.m_MouseMovedEvents.rbegin(); it != data.m_MouseMovedEvents.rend(); it++)
			{
				if ((*it)(event)) break;
			}
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowMoveEvent event(xpos, ypos);

			for (auto it = data.m_WindowMoveEvents.rbegin(); it != data.m_WindowMoveEvents.rend(); it++)
			{
				if ((*it)(event)) break;
			}
		});
	}

	OpenGLWindow::~OpenGLWindow()
	{
		glfwDestroyWindow(m_Window);
	}

	void OpenGLWindow::on_update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void OpenGLWindow::set_mouse_grabbed(bool grabbed)
	{
		if (grabbed)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	glm::vec2 OpenGLWindow::get_position() const
	{
		int x, y;

		glfwGetWindowPos(m_Window, &x, &y);

		return glm::vec2(x, y);
	}

	void OpenGLWindow::set_v_sync(VsyncMode mode)
	{
		if (mode == VsyncMode::TOGGLE)
		{
			if (m_Data.m_VSync == VsyncMode::ENABLED)
			{
				mode = VsyncMode::DISABLED;
			}
			else
			{
				mode = VsyncMode::ENABLED;
			}
		}

		if (mode == VsyncMode::ENABLED)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.m_VSync = mode;
	}

	VsyncMode OpenGLWindow::get_v_sync() const
	{
		return m_Data.m_VSync;
	}

	void OpenGLWindow::set_window_mode(WindowMode mode)
	{
		if (mode == WindowMode::TOGGLE)
		{
			if (m_Data.m_WindowMode == WindowMode::BORDERLESS || m_Data.m_WindowMode == WindowMode::FULLSCREEN)
			{
				mode = WindowMode::WINDOWED;
			}
			else
			{
				mode = WindowMode::FULLSCREEN;
			}
		}

		if (mode == WindowMode::WINDOWED)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowMonitor(m_Window, nullptr, 0, 30, m_Data.m_Width, m_Data.m_Height, mode->refreshRate);
		}

		if (mode == WindowMode::BORDERLESS)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);

			glfwSetWindowPos(m_Window, 0, 0);
			glfwSetWindowSize(m_Window, mode->width, mode->height);
		}

		if (mode == WindowMode::FULLSCREEN)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}

		m_Data.m_WindowMode = mode;
	}

	WindowMode OpenGLWindow::get_window_mode() const
	{
		return m_Data.m_WindowMode;
	}

	Ref<GraphicsContext> OpenGLWindow::get_context()
	{
		return m_Data.m_Context;
	}

	Ref<KeyCodeConv> OpenGLWindow::get_key_conv()
	{
		return m_Data.m_KeyCodeConv;
	}

	bool OpenGLWindow::is_key_pressed(int keycode) const
	{
		auto state = glfwGetKey(m_Window, m_Data.m_KeyCodeConv->to_platform_key(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool OpenGLWindow::is_mouse_pressed(int button) const
	{
		auto state = glfwGetMouseButton(m_Window, m_Data.m_KeyCodeConv->to_platform_mouse(button));
		return state == GLFW_PRESS;
	}

	glm::vec2 OpenGLWindow::get_mouse_pos() const
	{
		double x, y;

		glfwGetCursorPos(m_Window, &x, &y);

		return glm::vec2(x, y);
	}

	Window::EventPopFunc OpenGLWindow::on_mouse_moved(std::function<bool(const MouseMovedEvent&)> func)
	{
		m_Data.m_MouseMovedEvents.push_back(func);

		return ([](Ref<Window> window) { window->pop_mouse_moved(); });
	}

	void OpenGLWindow::pop_mouse_moved()
	{
		m_Data.m_MouseMovedEvents.pop_back();
	}

	Window::EventPopFunc OpenGLWindow::on_mouse_scrolled(std::function<bool(const MouseScrolledEvent&)> func)
	{
		m_Data.m_MouseScrolledEvents.push_back(func);

		return ([](Ref<Window> window) { window->pop_mouse_scrolled(); });
	}

	void OpenGLWindow::pop_mouse_scrolled()
	{
		m_Data.m_MouseScrolledEvents.pop_back();
	}

	Window::EventPopFunc OpenGLWindow::on_window_resize(std::function<bool(const WindowResizeEvent&)> func)
	{
		m_Data.m_WindowResizeEvents.push_back(func);

		return ([](Ref<Window> window) { window->pop_window_resize(); });
	}

	void OpenGLWindow::pop_window_resize()
	{
		m_Data.m_WindowResizeEvents.pop_back();
	}

	Window::EventPopFunc OpenGLWindow::on_window_move(std::function<bool(const WindowMoveEvent&)> func)
	{
		m_Data.m_WindowMoveEvents.push_back(func);

		return ([](Ref<Window> window) { window->pop_mouse_moved(); });
	}

	void OpenGLWindow::pop_window_move()
	{
		m_Data.m_WindowMoveEvents.pop_back();
	}

	void OpenGLWindow::on_key(KeyEvent listen, std::function<bool(const KeyCode&)> func)
	{
		m_Data.m_KeyEvents[listen].push_back(func);
	}

	void OpenGLWindow::pop_key(KeyEvent listen)
	{
		m_Data.m_KeyEvents[listen].pop_back();
	}

	void OpenGLWindow::on_mouse(MouseEvent listen, std::function<bool(const MouseCode&)> func)
	{
		m_Data.m_MouseEvents[listen].push_back(func);
	}

	void OpenGLWindow::pop_mouse(MouseEvent listen)
	{
		m_Data.m_MouseEvents[listen].pop_back();
	}

	Window::EventPopFunc OpenGLWindow::on_closed(std::function<bool()> func)
	{
		m_Data.m_NoDataEvents[NoDataEvent::WINDOW_CLOSE].push_back(func);

		return ([](Ref<Window> window) { window->pop_window_closed(); });
	}

	void OpenGLWindow::pop_window_closed()
	{
		m_Data.m_NoDataEvents[NoDataEvent::WINDOW_CLOSE].pop_back();
	}


}
