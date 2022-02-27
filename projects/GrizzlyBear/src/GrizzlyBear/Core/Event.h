#pragma once

namespace GrizzlyBear {

	enum class NoDataEvent
	{
		WINDOW_CLOSE
	};

	struct WindowResizeEvent
	{
		uint32_t WIDTH, HEIGHT;
	};

	struct WindowMoveEvent
	{
		uint32_t POS_X, POS_Y;
	};
	
	struct WindowCloseEvent
	{
	public:
		WindowCloseEvent() {}
	};

	//Key
	enum class KeyEvent
	{
		PRESSED, RELEASED, TYPED
	};

	struct KeyCode
	{
		uint32_t KEY_CODE, REPEAT;
	};

	//Mouse

	enum class MouseEvent
	{
		PRESSED, RELEASED
	};

	struct MouseCode
	{
		uint32_t MOUSE_CODE;
	};

	struct MouseMovedEvent
	{
		float MOUSE_X, MOUSE_Y;
	};

	struct MouseScrolledEvent
	{
		float OFFSET_X, OFFSET_Y;
	};
}
