#pragma once

#include <vector>

#include "GrizzlyBear/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace GrizzlyBear {

	struct ParticleProps
	{
		glm::vec3 Position = {0.0f, 0.0f, 0.0f};
		glm::vec2 Velocity = { 0.1f, 0.1f }, VelocityVariation = { 0.1f, 0.1f};
		glm::vec4 ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f }, ColorEnd = {0.0f, 0.0f, 0.0f, 1.0f};
		float SizeBegin = 1.0f, SizeEnd = 0.5f, SizeVariation = 0.1f;
		float LifeTime = 1.0f;
	};


	class ParticleSystem2D
	{
	public:
		ParticleSystem2D(Ref<Renderer2D> renderer, Ref<Texture> texture);

		void on_update(GrizzlyBear::Timestep ts);
		void on_render();

		void emit(const ParticleProps& props);

	private:
		struct Particle
		{
			glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
			glm::vec2 m_Velocity = {0.0f, 0.0f};
			glm::vec4 m_ColorBegin = { 0.0f, 0.0f, 0.0f, 0.0f };
			glm::vec4 m_ColorEnd = {1.0f, 1.0f, 1.0f, 1.0f};

			float m_Rotation = 0.0f;
			float m_SizeBegin = 0.0f;
			float m_SizeEnd = 1.0f;

			float m_LifeTime = 1.0f;
			float m_LifeRemaining = 0.0f;

			bool m_Active = false;
		};

		uint32_t m_ParticlePoolSize = 100000;
		uint32_t m_ParticlePoolIndex = 99999;

		std::vector<Particle> m_ParticlePool;

		Ref<Renderer2D> m_Renderer;

		Ref<Texture> m_Texture;
	};

}


