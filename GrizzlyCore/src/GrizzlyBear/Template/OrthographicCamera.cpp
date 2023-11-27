#include <gbpch.h>

#include "OrthographicCamera.h"

#include "GrizzlyBear/Core/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GrizzlyBear
{

	namespace Template {

		OrthographicCamera::OrthographicCamera(Ref<Window> window, float aspectRatio, bool rotation, bool linearResize)
			: m_AspectRatio(aspectRatio), m_RotationEnabled(rotation), m_LinearResize(linearResize)
		{
			m_Window = window;

			m_Window->on_mouse_scrolled(GRIZZLY_BIND_EVENT(OrthographicCamera::on_mouse_scrolled));
			m_Window->on_window_resize(GRIZZLY_BIND_EVENT(OrthographicCamera::on_window_resize));

			if (m_AspectRatio == 0.0f)
			{
				m_AspectRatio = (float)m_Window->get_width() / (float)m_Window->get_height();
			}

			m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		}

		OrthographicCamera::~OrthographicCamera()
		{
			m_Window->pop_window_resize();
			m_Window->pop_mouse_scrolled();
		}

		void OrthographicCamera::on_update(Timestep ts)
		{
			if (m_Window->is_key_pressed(GRIZZLY_KEY_A))
				m_Position.x -= m_CameraTranslationSpeed * ts;

			else if (m_Window->is_key_pressed(GRIZZLY_KEY_D))
				m_Position.x += m_CameraTranslationSpeed * ts;

			if (m_Window->is_key_pressed(GRIZZLY_KEY_W))
				m_Position.y += m_CameraTranslationSpeed * ts;

			else if (m_Window->is_key_pressed(GRIZZLY_KEY_S))
				m_Position.y -= m_CameraTranslationSpeed * ts;

			if (m_RotationEnabled)
			{
				if (m_Window->is_key_pressed(GRIZZLY_KEY_Q))
					m_Rotation += m_CameraRotationSpeed * ts;

				if (m_Window->is_key_pressed(GRIZZLY_KEY_E))
					m_Rotation -= m_CameraRotationSpeed * ts;
			}

			if (m_Window->is_key_pressed(GRIZZLY_KEY_PAGE_UP) || m_Window->is_key_pressed(GRIZZLY_KEY_KP_ADD))
			{
				m_ZoomLevel -= 0.01f * ts.get_millis();
				m_ZoomLevel = (std::max)(m_ZoomLevel, 0.25f);
				set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			}
			else if (m_Window->is_key_pressed(GRIZZLY_KEY_PAGE_DOWN) || m_Window->is_key_pressed(GRIZZLY_KEY_KP_SUBTRACT))
			{
				m_ZoomLevel += 0.01f * ts.get_millis();
				m_ZoomLevel = (std::min)(m_ZoomLevel, 100.0f);
				set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			}

			m_CameraTranslationSpeed = m_ZoomLevel;
		}

		glm::vec2 OrthographicCamera::get_bounds() const
		{
			glm::vec2 bounds;

			bounds.x = (m_AspectRatio * m_ZoomLevel) + (m_AspectRatio * m_ZoomLevel);
			bounds.y = (m_ZoomLevel + m_ZoomLevel);

			return bounds;
		}

		void OrthographicCamera::set_projection(float left, float right, float bottom, float top)
		{
			m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		void OrthographicCamera::set_zoom_level(float zoomLevel)
		{
			m_ZoomLevel = zoomLevel;
			set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		}

		void OrthographicCamera::set_position(const glm::vec3& position)
		{
			m_Position = position;
		}

		void OrthographicCamera::set_rotation(float rotation)
		{
			m_Rotation = rotation;
		}

		glm::mat4 OrthographicCamera::calc_view()
		{
			return glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		}

		bool OrthographicCamera::on_mouse_scrolled(const MouseScrolledEvent& e)
		{
			m_ZoomLevel -= e.OFFSET_Y;
			m_ZoomLevel = (std::max)(m_ZoomLevel, 0.25f);
			set_projection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
			return false;
		}

		bool OrthographicCamera::on_window_resize(const WindowResizeEvent& e)
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
