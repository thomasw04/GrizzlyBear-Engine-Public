#include <gbpch.h>

#include "OpenGLKeyCodes.h"

#include <GLFW/glfw3.h>

namespace GrizzlyBear {

	OpenGLKeyCodeConv::OpenGLKeyCodeConv()
	{
		m_GrizzlyKey.insert({ GRIZZLY_UNKNOWN_KEY, GLFW_KEY_UNKNOWN });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_SPACE, GLFW_KEY_SPACE });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_APOSTROPHE, GLFW_KEY_APOSTROPHE });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_COMMA, GLFW_KEY_COMMA });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_MINUS, GLFW_KEY_MINUS });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_PERIOD, GLFW_KEY_PERIOD });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_SLASH, GLFW_KEY_SLASH });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_0, GLFW_KEY_0 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_1, GLFW_KEY_1 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_2, GLFW_KEY_2 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_3, GLFW_KEY_3 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_4, GLFW_KEY_4 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_5, GLFW_KEY_5 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_6, GLFW_KEY_6 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_7, GLFW_KEY_7 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_8, GLFW_KEY_8 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_9, GLFW_KEY_9 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_SEMICOLON, GLFW_KEY_SEMICOLON });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_EQUAL, GLFW_KEY_EQUAL });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_A, GLFW_KEY_A });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_B, GLFW_KEY_B });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_C, GLFW_KEY_C });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_D, GLFW_KEY_D });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_E, GLFW_KEY_E });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F, GLFW_KEY_F });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_G, GLFW_KEY_G });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_H, GLFW_KEY_H });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_I, GLFW_KEY_I });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_J, GLFW_KEY_J });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_K, GLFW_KEY_K });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_L, GLFW_KEY_L });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_M, GLFW_KEY_M });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_N, GLFW_KEY_N });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_O, GLFW_KEY_O });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_P, GLFW_KEY_P });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_Q, GLFW_KEY_Q });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_R, GLFW_KEY_R });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_S, GLFW_KEY_S });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_T, GLFW_KEY_T });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_U, GLFW_KEY_U });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_V, GLFW_KEY_V });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_W, GLFW_KEY_W });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_X, GLFW_KEY_X });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_Y, GLFW_KEY_Y });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_Z, GLFW_KEY_Z });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_LEFT_BRACKET, GLFW_KEY_LEFT_BRACKET });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_BACKSLASH, GLFW_KEY_BACKSLASH });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_RIGHT_BRACKET, GLFW_KEY_RIGHT_BRACKET });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_GRAVE_ACCENT, GLFW_KEY_GRAVE_ACCENT });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_ESCAPE, GLFW_KEY_ESCAPE });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_ENTER, GLFW_KEY_ENTER });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_TAB, GLFW_KEY_TAB });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_BACKSPACE, GLFW_KEY_BACKSPACE });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_INSERT, GLFW_KEY_INSERT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_DELETE, GLFW_KEY_DELETE });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_RIGHT, GLFW_KEY_RIGHT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_LEFT, GLFW_KEY_LEFT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_DOWN, GLFW_KEY_DOWN });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_UP, GLFW_KEY_UP });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_PAGE_UP, GLFW_KEY_PAGE_UP });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_PAGE_DOWN, GLFW_KEY_PAGE_DOWN });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_HOME, GLFW_KEY_HOME });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_END, GLFW_KEY_END });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_CAPS_LOCK, GLFW_KEY_CAPS_LOCK });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_SCROLL_LOCK, GLFW_KEY_SCROLL_LOCK });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_NUM_LOCK, GLFW_KEY_NUM_LOCK });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_PRINT_SCREEN, GLFW_KEY_PRINT_SCREEN });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_PAUSE, GLFW_KEY_PAUSE });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_F1, GLFW_KEY_F1 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F2, GLFW_KEY_F2 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F3, GLFW_KEY_F3 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F4, GLFW_KEY_F4 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F5, GLFW_KEY_F5 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F6, GLFW_KEY_F6 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F7, GLFW_KEY_F7 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F8, GLFW_KEY_F8 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F9, GLFW_KEY_F9 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F10, GLFW_KEY_F10 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F11, GLFW_KEY_F11 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F12, GLFW_KEY_F12 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F13, GLFW_KEY_F13 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F14, GLFW_KEY_F14 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F15, GLFW_KEY_F15 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F16, GLFW_KEY_F16 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F17, GLFW_KEY_F17 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F18, GLFW_KEY_F18 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F19, GLFW_KEY_F19 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F20, GLFW_KEY_F20 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F21, GLFW_KEY_F21 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F22, GLFW_KEY_F22 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F23, GLFW_KEY_F23 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F24, GLFW_KEY_F24 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_F25, GLFW_KEY_F25 });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_0, GLFW_KEY_KP_0 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_1, GLFW_KEY_KP_1 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_2, GLFW_KEY_KP_2 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_3, GLFW_KEY_KP_3 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_4, GLFW_KEY_KP_4 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_5, GLFW_KEY_KP_5 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_6, GLFW_KEY_KP_6 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_7, GLFW_KEY_KP_7 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_8, GLFW_KEY_KP_8 });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_9, GLFW_KEY_KP_9 });

		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_DECIMAL, GLFW_KEY_KP_DECIMAL });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_DIVIDE, GLFW_KEY_KP_DIVIDE });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_MULTIPLY, GLFW_KEY_KP_MULTIPLY });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_SUBTRACT, GLFW_KEY_KP_SUBTRACT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_ADD, GLFW_KEY_KP_ADD });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_ENTER, GLFW_KEY_KP_ENTER });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_KP_EQUAL, GLFW_KEY_KP_EQUAL });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_CONTROL });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_LEFT_ALT, GLFW_KEY_LEFT_ALT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_LEFT_SUPER, GLFW_KEY_LEFT_SUPER });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_SHIFT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_RIGHT_CONTROL, GLFW_KEY_RIGHT_CONTROL });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_ALT });
		m_GrizzlyKey.insert({ GRIZZLY_KEY_RIGHT_SUPER, GLFW_KEY_RIGHT_SUPER });

		for (auto it = m_GrizzlyKey.begin(); it != m_GrizzlyKey.end(); it++)
		{
			m_GLFWKey[it->second] = it->first;
		}
	}

	int OpenGLKeyCodeConv::to_grizzly_key(int keycode)
	{
		return m_GLFWKey.at(keycode);
	}

	int OpenGLKeyCodeConv::to_platform_key(int keycode)
	{
		return m_GrizzlyKey.at(keycode);
	}

	int OpenGLKeyCodeConv::to_grizzly_mouse(int button)
	{
		return button;
	}

	int OpenGLKeyCodeConv::to_platform_mouse(int button)
	{
		return button;
	}

}