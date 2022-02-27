#include <gbpch.h>

#include "GuiCamera.h"

#include "GrizzlyBear/Core/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GrizzlyBear
{

	namespace Template {

		GuiCamera::GuiCamera(Ref<Window> window, float aspectRatio, bool rotation, bool linearResize)
			: m_AspectRatio(aspectRatio), m_LinearResize(linearResize)
		{
			m_Window = window;

			m_Window->on_window_resize(GRIZZLY_BIND_EVENT(GuiCamera::on_window_resize));

			if (m_AspectRatio == 0.0f)
			{
				m_AspectRatio = (float)m_Window->get_width() / (float)m_Window->get_height();
			}

			m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		}

		GuiCamera::~GuiCamera()
		{
			m_Window->pop_window_resize();
		}

		void GuiCamera::on_update(Timestep ts) {}

		glm::vec2 GuiCamera::get_bounds() const
		{
			glm::vec2 bounds;

			bounds.x = (m_AspectRatio * m_ZoomLevel) + (m_AspectRatio * m_ZoomLevel);
			bounds.y = (m_ZoomLevel + m_ZoomLevel);

			return bounds;
		}

		void GuiCamera::set_projection(float left, float right, float bottom, float top)
		{
			m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		glm::mat4 GuiCamera::calc_view()
		{
			return glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f}) * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));
		}

		bool GuiCamera::on_window_resize(const WindowResizeEvent& e)
		{
			if (!m_LinearResize)
			{
				if (m_AspectRatio == ((float)e.WIDTH / (float)e.HEIGHT)) return false;

				m_AspectRatio = (float)e.WIDTH / (float)e.HEIGHT;
				set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			}

			return false;
		}

	}
}
