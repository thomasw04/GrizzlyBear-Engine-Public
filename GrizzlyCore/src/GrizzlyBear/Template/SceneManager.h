#pragma once

#include <unordered_map>
#include <functional>
#include <vector>

#include "TemplateBase.h"

#include "GrizzlyBear/Core/BaseDefines.h"
#include "GrizzlyBear/Scene/Scene.h"
#include "GrizzlyBear/Renderer/Renderer2D.h"

namespace GrizzlyBear {

	namespace Template {

		class SceneManager
		{
		public:
			SceneManager(Ref<Window> window);
			~SceneManager();

			void set_custom_camera(Ref<Camera> camera);

			Ref<Scene> load_scene(const Gstring& name, const Gstring& fromFile = "", bool createIfNotExists = true);

			bool unload_scene(const Gstring& name);
			bool save_scene(const Gstring& name, const Gstring& customPath = "");

			void on_update(Timestep ts);

			bool is_scene_set();

			/**Setting the current level, which will be edited by the other methods!*/
			bool set_scene(const Gstring& level);

			/**Returns a shared_ptr to the current level*/
			Ref<Scene> get_scene();

			/**Returns the current level name*/
			Gstring get_scene_name();

			/**Setting the default startup level*/
			void set_default_scene();

			/**Checks whether a level exists*/
			bool exist_scene(const Gstring& level);

		private:
			Ref<Scene> get_scene(const Gstring& level);

		private:
			std::unordered_map<Gstring, Ref<Scene>> m_Scenes;

			Gstring m_CurrentScene;

			Ref<Window> m_Window;
			Ref<Renderer2D> m_Renderer2D;
			Ref<Camera> m_CustomCamera;
		};
	}
}