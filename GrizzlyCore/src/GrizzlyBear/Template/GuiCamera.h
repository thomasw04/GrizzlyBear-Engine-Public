#pragma once

#include "TemplateBase.h"

#include "GrizzlyBear/Core/Event.h"
#include "GrizzlyBear/API/Window.h"

#include <glm/glm.hpp>

namespace GrizzlyBear
{
	namespace Template {

		class GuiCamera : public Camera
		{
		public:
			GuiCamera(Ref<Window> window, float aspectRatio = 0.0f, bool rotation = false, bool linearResize = false);
			~GuiCamera();

			virtual void on_update(Timestep ts) override;

			glm::vec2 get_bounds() const;

			virtual glm::mat4& get_projection() override { return m_Projection; }

			void set_projection(float left, float right, float bottom, float top);

			virtual glm::mat4 calc_view() override;

		public:
			bool on_window_resize(const WindowResizeEvent& e);
		private:
			float m_AspectRatio;

			bool m_LinearResize;

			float m_ZoomLevel = 5.0f;

			glm::mat4 m_Projection;

			Ref<Window> m_Window;
		};

	}
}
