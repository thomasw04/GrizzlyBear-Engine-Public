#include <gbpch.h>

#include "Renderer.h"

#include "GrizzlyBear/Core/Application.h"

#include "GrizzlyBear/Renderer/Renderer2D.h"
#include "GrizzlyBear/Renderer/Renderer3D.h"
#include "GrizzlyBear/Renderer/GuiRenderer.h"

namespace GrizzlyBear {


	Renderer::Renderer(Ref<Window> window)
	{
		m_Window = window;
	}

	void Renderer::begin_frame(Ref<FrameBuffer> buffer)
	{
		m_CurrentFrameBuffer = nullptr;

		if (buffer != nullptr)
		{
			m_CurrentFrameBuffer = buffer;
			m_CurrentFrameBuffer->bind();
		}

		m_Window->get_context()->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		m_Window->get_context()->clear();
	}

	void Renderer::end_frame()
	{
		if (m_CurrentFrameBuffer != nullptr)
		{
			m_CurrentFrameBuffer->unbind(m_Window->get_width(), m_Window->get_height());

			m_Window->get_context()->set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			m_Window->get_context()->clear();
		}
	}

	Ref<GraphicsContext> Renderer::get_context()
	{
		return m_Window->get_context();
	}

}


