#pragma once

#include "GrizzlyBear/API/Window.h"

#include "GrizzlyBear/API/GGL/GraphicsContext.h"

struct GLFWwindow;

namespace GrizzlyBear {

	
	class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const Gstring& title, uint32_t width, uint32_t height);

		virtual ~OpenGLWindow();

		void on_update() override;

		uint32_t get_width() const override { return m_Data.m_Width; }
		uint32_t get_height() const override { return m_Data.m_Height; }

		virtual void set_mouse_grabbed(bool grabbed) override;

		glm::vec2 get_position() const override;

		virtual void set_v_sync(VsyncMode mode) override;
		virtual VsyncMode get_v_sync() const override;

		virtual void set_window_mode(WindowMode mode) override;
		virtual WindowMode get_window_mode() const override;

		virtual Ref<GraphicsContext> get_context() override;
		virtual Ref<KeyCodeConv> get_key_conv() override;

		virtual bool is_key_pressed(int keycode) const override;
		virtual bool is_mouse_pressed(int button) const override;

		virtual glm::vec2 get_mouse_pos() const override;

		inline virtual void* get_native_window() const override { return m_Window; }

		virtual Window::EventPopFunc on_mouse_moved(std::function<bool(const MouseMovedEvent&)> func) override;
		virtual void pop_mouse_moved() override;

		virtual Window::EventPopFunc on_mouse_scrolled(std::function<bool(const MouseScrolledEvent&)> func) override;
		virtual void pop_mouse_scrolled() override;

		virtual Window::EventPopFunc on_window_resize(std::function<bool(const WindowResizeEvent&)> func) override;
		virtual void pop_window_resize() override;

		virtual Window::EventPopFunc on_window_move(std::function<bool(const WindowMoveEvent&)> func) override;
		virtual void pop_window_move() override;

		virtual void on_key(KeyEvent listen, std::function<bool(const KeyCode&)> func) override;
		virtual void pop_key(KeyEvent listen) override;

		virtual void on_mouse(MouseEvent listen, std::function<bool(const MouseCode&)> func) override;
		virtual void pop_mouse(MouseEvent listen) override;

		virtual Window::EventPopFunc on_closed(std::function<bool()> func) override;
		virtual void pop_window_closed() override;

	private:

		struct WindowData
		{
			Gstring m_Title = "Grizzly Bear Engine";
			uint32_t m_Width = 1280;
			uint32_t m_Height = 720;

			std::vector<std::function<bool(const MouseMovedEvent&)>> m_MouseMovedEvents;
			std::vector<std::function<bool(const MouseScrolledEvent&)>> m_MouseScrolledEvents;
			std::vector<std::function<bool(const WindowResizeEvent&)>> m_WindowResizeEvents;
			std::vector<std::function<bool(const WindowMoveEvent&)>> m_WindowMoveEvents;

			std::unordered_map<NoDataEvent, std::vector<std::function<bool()>>> m_NoDataEvents;
			std::unordered_map<KeyEvent, std::vector<std::function<bool(KeyCode)>>> m_KeyEvents;
			std::unordered_map<MouseEvent, std::vector < std::function<bool(MouseCode)>>> m_MouseEvents;

			WindowMode m_WindowMode = WindowMode::WINDOWED;

			Ref<GraphicsContext> m_Context;
			Ref<KeyCodeConv> m_KeyCodeConv;

			VsyncMode m_VSync = VsyncMode::ENABLED;
		};

	private:
		GLFWwindow* m_Window;

		WindowData m_Data;
	};
}
