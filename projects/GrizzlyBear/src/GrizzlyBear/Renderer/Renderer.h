#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/API/Window.h"
#include "GrizzlyBear/API/GGL/GraphicsContext.h"

#include "GrizzlyBear/Asset/AssetManager.h"

namespace GrizzlyBear {

	enum RendererFlags
	{
		RendererHint_None            =      0,
		RendererHint_NoEnvironment   = 1 << 0,
		RendererHint_NoDefaultShader = 1 << 1
	};

	class Renderer
	{
	public:
		Renderer(Ref<Window> window);

		void begin_frame(Ref<FrameBuffer> buffer = nullptr);
		void end_frame();

		Ref<GraphicsContext> get_context();

	private:
		Ref<Window> m_Window;
		Ref<FrameBuffer> m_CurrentFrameBuffer;
	};
}
