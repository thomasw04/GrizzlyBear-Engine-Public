#pragma once

#include <array>
#include <unordered_map>

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/Misc/RenderUtils.h"
#include "GrizzlyBear/Scene/Components/BaseTypes.h"

namespace GrizzlyBear {
	
	class Animator2D
	{
	public:
		void play_animation(const Gstring& animation, size_t objectID);
		void play_animation(const Animation2D& animation, size_t objectID);

		Animation2D& get_animation_instance(size_t objectID);
		Animation2D& get_animation(const Gstring& animation);

		void pause_animation(size_t objectID);
		void resume_animation(size_t objectID);

		bool is_active(size_t objectID);

		/*
		* \brief 
		* Sets the right texture parameters for the current frame
		* \param id The quad id
		* \param textureCoords Please enter your MaterialProps2D.TEXTURE_COORDS - value will be set auto
		* \param textureFilepath value will be set auto
		* \param ts Enter the timestep of the current frame
		* \return Whether there is a animation or not
		*/
		bool update_animation(size_t id, std::array<float, 8>& textureCoords, Gstring& textureFilepath, Timestep ts);

		static void set_animation(const Gstring& name, const Animation2D& anim);

		static void load_animation(const Gstring& filepath);
		static void save_animation(const Gstring& name, const Gstring& filepath);
		static bool exist_animation(const Gstring& name);

	private:
		std::unordered_map<size_t, Animation2D> m_CurrentAnimations;
		std::unordered_map <size_t, PersistentCache<std::pair<std::array<float, 8>, Gstring>>> m_Recovery;

	private:
		static std::unordered_map<size_t, Animation2D> s_AllAnimations;
		static std::hash<std::string> s_Hasher;
	};
}
