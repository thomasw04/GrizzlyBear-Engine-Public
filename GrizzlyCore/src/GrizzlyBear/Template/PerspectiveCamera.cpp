#include <gbpch.h>

#include "PerspectiveCamera.h"

#include "GrizzlyBear/Core/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GrizzlyBear
{

	namespace Template {

		PerspectiveCamera::PerspectiveCamera(Ref<Window> window, float fovy, float aspectRatio, bool enableMouseGrab)
			: m_Projection(glm::perspective(fovy, aspectRatio, 0.1f, 100.0f)), m_Fovy(fovy), m_AspectRatio(aspectRatio), m_MouseGrab(enableMouseGrab)
		{
			m_Window = window;
		}

		PerspectiveCamera::~PerspectiveCamera() {}

		void PerspectiveCamera::on_update(Timestep ts)
		{
			float x = m_Window->get_mouse_pos().x;
			float y = m_Window->get_mouse_pos().y;

			if (m_MouseGrab && !m_Window->is_mouse_pressed(GRIZZLY_MOUSE_BUTTON_2))
			{
				m_LastMousePos = { x, y };
				return;
			}

			//Move into script
			glm::vec2 deltaMouse = { x - m_LastMousePos.x, y - m_LastMousePos.y };

			//Rework needed
			double decay = std::exp(std::log(0.5) * 1.2 * ts.get_millis());

			m_Rotation.y -= (float)(deltaMouse.x * decay * m_CameraRotationSpeed * ts * 1000);
			m_Rotation.x -= (float)(deltaMouse.y * decay * m_CameraRotationSpeed * ts * 1000);

			m_LastMousePos = { x, y };

			float zForward = glm::cos(glm::radians(m_Rotation.y));
			float xForward = glm::sin(glm::radians(m_Rotation.y));

			if (m_Window->is_key_pressed(GRIZZLY_KEY_A))
			{
				m_Position.x -= m_CameraTranslationSpeed * ts * zForward;
				m_Position.z += m_CameraTranslationSpeed * ts * xForward;
			}

			else if (m_Window->is_key_pressed(GRIZZLY_KEY_D))
			{
				m_Position.x += m_CameraTranslationSpeed * ts * zForward;
				m_Position.z -= m_CameraTranslationSpeed * ts * xForward;
			}

			if (m_Window->is_key_pressed(GRIZZLY_KEY_W))
			{
				m_Position.z -= m_CameraTranslationSpeed * ts * zForward;
				m_Position.x -= m_CameraTranslationSpeed * ts * xForward;
			}

			else if (m_Window->is_key_pressed(GRIZZLY_KEY_S))
			{
				m_Position.z += m_CameraTranslationSpeed * ts * zForward;
				m_Position.x += m_CameraTranslationSpeed * ts * xForward;
			}

			if (m_Window->is_key_pressed(GRIZZLY_KEY_SPACE))
			{
				m_Position.y += m_CameraTranslationSpeed * ts;
			}

			else if (m_Window->is_key_pressed(GRIZZLY_KEY_LEFT_SHIFT))
			{
				m_Position.y -= m_CameraTranslationSpeed * ts;
			}

			if (m_Window->is_key_pressed(GRIZZLY_KEY_Q))
			{
				m_Rotation.x += m_CameraRotationSpeed * ts;
			}

			if (m_Window->is_key_pressed(GRIZZLY_KEY_E))
			{
				m_Rotation.x -= m_CameraRotationSpeed * ts;
			}
		}

		void PerspectiveCamera::set_projection(float fovy, float aspectRatio)
		{
			m_Projection = glm::perspective(fovy, aspectRatio, 0.1f, 100.0f);
		}

		glm::mat4 PerspectiveCamera::calc_view()
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1, 0, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));

			//MARK: Inverse maybe

			return transform;
		}

		bool PerspectiveCamera::on_window_resize(const WindowResizeEvent& e)
		{
			if (m_AspectRatio == ((float)e.WIDTH / (float)e.HEIGHT)) return false;

			m_AspectRatio = (float)e.WIDTH / (float)e.HEIGHT;

			set_projection(m_Fovy, m_AspectRatio);

			return false;
		}

	}

}
