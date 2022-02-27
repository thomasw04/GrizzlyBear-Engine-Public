#pragma once

#include "gbpch.h"

#include <GrizzlyBear/Core/BaseDefines.h>
#include "GrizzlyBear/Core/Event.h"

#include "GrizzlyBear/API/GGL/GraphicsContext.h"
#include "GrizzlyBear/API/GGL/KeyCodes.h"

#include <glm/glm.hpp>

namespace GrizzlyBear {

	enum class WindowMode
	{
		WINDOWED, BORDERLESS, FULLSCREEN, TOGGLE
	};

	enum class VsyncMode
	{
		ENABLED, DISABLED, TOGGLE 
	};

	class Window
	{
	public:
		typedef void (*EventPopFunc)(Ref<Window>);

		virtual ~Window() {}

		virtual void on_update() = 0;

		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;

		virtual void set_mouse_grabbed(bool grabbed) = 0;

		virtual glm::vec2 get_position() const = 0;

		virtual void set_v_sync(VsyncMode mode) = 0;
		virtual VsyncMode get_v_sync() const = 0;

		virtual void set_window_mode(WindowMode mode) = 0;
		virtual WindowMode get_window_mode() const = 0;

		virtual Ref<GraphicsContext> get_context() = 0;
		virtual Ref<KeyCodeConv> get_key_conv() = 0;

		virtual bool is_key_pressed(int keycode) const = 0;
		virtual bool is_mouse_pressed(int button) const = 0;

		virtual glm::vec2 get_mouse_pos() const = 0;

		virtual void* get_native_window() const = 0;

	public:

		virtual EventPopFunc on_mouse_moved(std::function<bool(const MouseMovedEvent&)> func) = 0;
		virtual void pop_mouse_moved() = 0;

		virtual EventPopFunc on_mouse_scrolled(std::function<bool(const MouseScrolledEvent&)> func) = 0;
		virtual void pop_mouse_scrolled() = 0;

		virtual EventPopFunc on_window_resize(std::function<bool(const WindowResizeEvent&)> func) = 0;
		virtual void pop_window_resize() = 0;

		virtual EventPopFunc on_window_move(std::function<bool(const WindowMoveEvent&)> func) = 0;
		virtual void pop_window_move() = 0;

		virtual void on_key(KeyEvent listen, std::function<bool(const KeyCode&)> func) = 0;
		virtual void pop_key(KeyEvent listen) = 0;

		virtual void on_mouse(MouseEvent listen, std::function<bool(const MouseCode&)> func) = 0;
		virtual void pop_mouse(MouseEvent listen) = 0;

		virtual EventPopFunc on_closed(std::function<bool()> func) = 0;
		virtual void pop_window_closed() = 0;

	public:
		static Ref<Window> create(const Gstring& title, uint32_t width, uint32_t height);
	};
}