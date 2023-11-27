#include <gbpch.h>

#include "ParticleSystem2D.h"

#include "GrizzlyBear/Misc/Random.h"

#include "glm/gtc/constants.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

namespace GrizzlyBear {

	ParticleSystem2D::ParticleSystem2D(Ref<Renderer2D> renderer, Ref<Texture> texture)
	{
		m_ParticlePool.resize(m_ParticlePoolSize);

		m_Texture = texture;
		m_Renderer = renderer;
	}

	void ParticleSystem2D::on_update(GrizzlyBear::Timestep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.m_Active) continue;

			if (particle.m_LifeRemaining <= 0.0f)
			{
				particle.m_Active = false;
				continue;
			}

			particle.m_LifeRemaining -= ts;
			particle.m_Position += glm::vec3(particle.m_Velocity.x * (float)ts, particle.m_Velocity.y * (float)ts, 0.0f);
			particle.m_Rotation += 0.02f * ts;
		}
	}

	void ParticleSystem2D::on_render()
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.m_Active) continue;

			//TODO Make changeable
			float life = particle.m_LifeRemaining / particle.m_LifeTime;
			glm::vec4 color = glm::lerp(particle.m_ColorEnd, particle.m_ColorBegin, life);
			color.a = color.a * life;

			float size = glm::lerp(particle.m_SizeEnd, particle.m_SizeBegin, life);

			QuadProps data;

			data.TINT = color;
			data.TEXTURE = m_Texture;

			m_Renderer->draw_quad(particle.m_Position, particle.m_Rotation, { size, size }, data);
		}
	}

	void ParticleSystem2D::emit(const ParticleProps& props)
	{
		Particle& particle = m_ParticlePool[m_ParticlePoolIndex];
		particle.m_Active = true;
		particle.m_Position = props.Position;
		particle.m_Rotation = Random::rand_float() * 2.0f * glm::pi<float>();

		particle.m_Velocity = props.Velocity;
		particle.m_Velocity.x += props.VelocityVariation.x * (Random::rand_float() - 0.5f);
		particle.m_Velocity.y += props.VelocityVariation.y * (Random::rand_float() - 0.5f);

		particle.m_ColorBegin = props.ColorBegin;
		particle.m_ColorEnd = props.ColorEnd;

		particle.m_LifeTime = props.LifeTime;
		particle.m_LifeRemaining = props.LifeTime;
		particle.m_SizeBegin = props.SizeBegin + props.SizeVariation * (Random::rand_float() - 0.5f);
		particle.m_SizeEnd = props.SizeEnd;

		m_ParticlePoolIndex = --m_ParticlePoolIndex % m_ParticlePool.size();
	}

}
