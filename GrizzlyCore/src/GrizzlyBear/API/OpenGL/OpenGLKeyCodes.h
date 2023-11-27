#pragma once

#include "GrizzlyBear/API/GGL/KeyCodes.h"

#include <unordered_map>

namespace GrizzlyBear {


	class OpenGLKeyCodeConv : public KeyCodeConv
	{
	public:
		OpenGLKeyCodeConv();

		virtual int to_grizzly_key(int keycode) override;
		virtual int to_platform_key(int keycode) override;

		virtual int to_grizzly_mouse(int button) override;
		virtual int to_platform_mouse(int button) override;

	private:
		std::unordered_map<int, int> m_GrizzlyKey;
		std::unordered_map<int, int> m_GLFWKey;
	};
}
