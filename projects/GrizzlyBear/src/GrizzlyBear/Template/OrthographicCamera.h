#pragma once

#include "TemplateBase.h"

#include "GrizzlyBear/Core/Event.h"
#include "GrizzlyBear/API/Window.h"

#include <glm/glm.hpp>

namespace GrizzlyBear
{
	namespace Template {

		class OrthographicCamera : public Camera
		{
		public:
			OrthographicCamera(Ref<Window> window, float aspectRatio = 0.0f, bool rotation = false, bool linearResize = false);
			~OrthographicCamera();

			virtual void on_update(Timestep ts) override;

			glm::vec2 get_bounds() const;

			float get_translation_speed() { return m_CameraTranslationSpeed; }
			float get_rotation_speed() { return m_CameraRotationSpeed; }

			glm::vec3 get_position() { return m_Position; };
			float get_rotation() { return m_Rotation; }
			float get_zoom_level() { return m_ZoomLevel; }

			void set_zoom_level(float zoomLevel);
			void set_position(const glm::vec3& position);
			void set_rotation(float rotation);

			virtual glm::mat4& get_projection() override { return m_Projection; }

			void set_projection(float left, float right, float bottom, float top);
			

			virtual glm::mat4 calc_view() override;

			

		public:
			bool on_mouse_scrolled(const MouseScrolledEvent& e);
			bool on_window_resize(const WindowResizeEvent& e);
		private:
			float m_AspectRatio;

			bool m_RotationEnabled;

			bool m_LinearResize;

			float m_ZoomLevel = 1.0f;

			glm::mat4 m_Projection;

			glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
			float m_Rotation = 0.0f;

			float m_CameraTranslationSpeed = 10.0f;
			float m_CameraRotationSpeed = 180.0f;

			Ref<Window> m_Window;
		};

	}
}
