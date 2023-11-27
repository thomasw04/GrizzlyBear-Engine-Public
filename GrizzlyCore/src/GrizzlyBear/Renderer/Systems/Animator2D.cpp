#include <gbpch.h>

#include "Animator2D.h"

#include <nlohmann/json.hpp>

#include "GrizzlyBear/Environment/Environment.h"

namespace GrizzlyBear {

	std::unordered_map<size_t, Animation2D> Animator2D::s_AllAnimations;
	std::hash<std::string> Animator2D::s_Hasher;

	void Animator2D::play_animation(const Gstring& animation, size_t objectID)
	{
		if (!s_AllAnimations.contains(s_Hasher(animation)))
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Renderer, Message: Unknown animation: {0}", animation.c_str());
			return;
		}

		m_CurrentAnimations[objectID] = s_AllAnimations[s_Hasher(animation)];
		m_CurrentAnimations[objectID].ACTIVE = true;
	}

	void Animator2D::play_animation(const Animation2D& animation, size_t objectID)
	{
		m_CurrentAnimations[objectID] = animation;
		m_CurrentAnimations[objectID].ACTIVE = true;

	}

	Animation2D& Animator2D::get_animation_instance(size_t objectID)
	{
		return m_CurrentAnimations[objectID];
	}

	Animation2D& Animator2D::get_animation(const Gstring& animation)
	{
		return s_AllAnimations[s_Hasher(animation)];
	}

	void Animator2D::pause_animation(size_t objectID)
	{
		m_CurrentAnimations[objectID].ACTIVE = false;
	}

	void Animator2D::resume_animation(size_t objectID)
	{
		m_CurrentAnimations[objectID].ACTIVE = true;
	}

	bool Animator2D::is_active(size_t objectID)
	{
		return m_CurrentAnimations[objectID].ACTIVE;
	}

	bool Animator2D::update_animation(size_t id, std::array<float, 8>& textureCoords, Gstring& textureFilepath, Timestep ts)
	{
		Animation2D& anim = m_CurrentAnimations[id];
		auto& cache = m_Recovery[id];

		//Check if an animation is active
		if (!anim.ACTIVE) return false;

		if (!cache.is_stored())
		{
			cache.store({ textureCoords, textureFilepath });
		}

		anim.LAST_FRAME_TIME += ts.get_seconds();

		float frameTime = anim.PLAYTHROUGH_TIME / anim.END_FRAME;

		if (anim.LAST_FRAME_TIME >= frameTime)
		{
			if (anim.CURRENT_FRAME == anim.END_FRAME)
			{
				if (anim.LOOP)
				{
					anim.CURRENT_FRAME = 0;
					textureFilepath = anim.TEXTURE_FILEPATH;
					anim.TEXTURE_LAYOUT.get_texture_coordinates(anim.CURRENT_FRAME, textureCoords);
					return true;

				}
				else
				{
					anim.ACTIVE = false;
					std::tie(textureCoords, textureFilepath) = cache.load();
					cache.reset();
					return false;
				}
			}
			else
			{
				anim.CURRENT_FRAME++;
			}

			anim.LAST_FRAME_TIME = 0;
		}

		textureFilepath = anim.TEXTURE_FILEPATH;
		anim.TEXTURE_LAYOUT.get_texture_coordinates(anim.CURRENT_FRAME, textureCoords);
		return true;
	}

	void Animator2D::load_animation(const Gstring& filepath)
	{
		size_t id = s_Hasher(filepath);

		if (s_AllAnimations.contains(id))
		{
			GRIZZLY_CORE_LOG(LogLevel::Warn, "Trying to load animation {0} - already loaded", filepath.c_str());
			return;
		}

		Gbuffer file = Environment::get_fetcher()->get_asset_data(filepath);

		if (file.is_good())
		{
			Gstring& source = file.m_Buffer;

			const char* endHeaderToken = "#$EH75$#";

			size_t endHeader = source.find(endHeaderToken, 0);

			nlohmann::json object = nlohmann::json::parse(source.substr(0, endHeader));

			Animation2D newAnim;

			Reflector::deserialize(newAnim, object);

			//TODO Picture in same file

			set_animation(filepath, newAnim);
		}
	}

	void Animator2D::save_animation(const Gstring& name, const Gstring& filepath)
	{
		size_t id = s_Hasher(name);

		std::stringstream sstream;

		nlohmann::json object;

		Reflector::serialize(s_AllAnimations[id], object);

		const char* endHeaderToken = "#$EH75$#";

		sstream << object.dump();
		sstream << endHeaderToken;

		Environment::get_fetcher()->save_asset_data(filepath, { sstream.str(), Gbuffer::State::GOOD });
	}

	bool Animator2D::exist_animation(const Gstring& name)
	{
		return s_AllAnimations.contains(s_Hasher(name));
	}

	void Animator2D::set_animation(const Gstring& name, const Animation2D& anim)
	{
		size_t id = s_Hasher(name);

		s_AllAnimations[id] = anim;
	}
}


