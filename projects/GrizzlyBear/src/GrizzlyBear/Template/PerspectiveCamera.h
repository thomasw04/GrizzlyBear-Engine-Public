#pragma once

#include "TemplateBase.h"

#include "GrizzlyBear/Core/Event.h"
#include "GrizzlyBear/API/Window.h"

#include <glm/glm.hpp>

namespace GrizzlyBear
{

	namespace Template {

		class PerspectiveCamera : public Camera
		{
		public:
			PerspectiveCamera(Ref<Window> window, float fovy, float aspectRatio, bool enableMouseGrab);
			~PerspectiveCamera();

			virtual void on_update(Timestep ts) override;

			void set_projection(float fovy, float aspectRatio);

			virtual glm::mat4& get_projection() override { return m_Projection; }

			virtual glm::mat4 calc_view() override;

			glm::vec3& get_position() { return m_Position; }
			glm::vec3& get_rotation() { return m_Rotation; }

		private:
			bool on_window_resize(const WindowResizeEvent& e);
		private:
			glm::mat4 m_Projection;

			float m_Fovy;

			float m_AspectRatio;

			bool m_MouseGrab;

			float m_CameraTranslationSpeed = 10.0f;
			float m_CameraRotationSpeed = 0.1f;

			glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };

			glm::vec2 m_LastMousePos = { 0.0f, 0.0f };

			Ref<Window> m_Window;
		};

	}
}
