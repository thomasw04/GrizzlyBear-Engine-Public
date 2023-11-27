#pragma once

#include <glm/glm.hpp>

namespace GrizzlyBear {


	namespace Template {


		class Camera
		{
		public:
			virtual void on_update(Timestep ts) = 0;

			virtual glm::mat4& get_projection() = 0;

			virtual glm::mat4 calc_view() = 0;
		};


		class SimpleGraph
		{
		public:
			void update(float delta)
			{
				m_Graph[m_XPos] = delta;
				m_XPos++;
				m_XPos %= 100;
			}

			float* get_graph()
			{
				return m_Graph;
			}

			int get_xpos()
			{
				return m_XPos;
			}

		private:
			int m_XPos = 0;
			float m_Graph[100];
		};
	}

}
