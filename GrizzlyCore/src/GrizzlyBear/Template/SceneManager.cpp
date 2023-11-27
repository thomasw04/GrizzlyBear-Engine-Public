#include <gbpch.h>

#include "SceneManager.h"

#include "GrizzlyBear/Environment/Environment.h"

namespace GrizzlyBear {

	namespace Template {

		SceneManager::SceneManager(Ref<Window> window)
		{
			m_Window = window;

			m_Renderer2D = CreateRef<Renderer2D>(window->get_context(), RendererHint_None);

			m_Window->on_key(KeyEvent::PRESSED, [&](KeyCode code) -> bool
			{
				return get_scene()->on_key(KeyEvent::PRESSED, code);
			});

			m_Window->on_key(KeyEvent::RELEASED, [&](KeyCode code) -> bool
			{
				return get_scene()->on_key(KeyEvent::RELEASED, code);
			});

			m_Window->on_mouse(MouseEvent::PRESSED, [&](MouseCode code) -> bool
			{
				return get_scene()->on_mouse(MouseEvent::PRESSED, code);
			});

			m_Window->on_mouse(MouseEvent::RELEASED, [&](MouseCode code) -> bool
			{
				return get_scene()->on_mouse(MouseEvent::RELEASED, code);
			});
		}

		SceneManager::~SceneManager()
		{
			m_Window->pop_mouse(MouseEvent::RELEASED);
			m_Window->pop_mouse(MouseEvent::PRESSED);

			m_Window->pop_key(KeyEvent::RELEASED);
			m_Window->pop_key(KeyEvent::PRESSED);
		}

		void SceneManager::set_custom_camera(Ref<Camera> camera)
		{
			m_CustomCamera = camera;
		}

		Ref<Scene> SceneManager::load_scene(const Gstring& name, const Gstring& fromFile, bool createIfNotExists)
		{
			if (m_Scenes.find(name) == m_Scenes.end())
			{
				if (!FileIO::exists(Environment::get_fetcher()->get_asset_path(fromFile)) && createIfNotExists)
				{
					m_Scenes.insert({ name, CreateRef<Scene>("", m_Window) });
					m_Scenes[name]->serialize(fromFile);
				}
				else
				{
					m_Scenes.insert({ name, CreateRef<Scene>(fromFile, m_Window) });
				}

				auto scene = m_Scenes[name];

				scene->is_loaded(true, true);

				GRIZZLY_CORE_LOG(LogLevel::Info, "Successfully added Scene {0}", name.c_str());
				return scene;
			}

			GRIZZLY_CORE_LOG(LogLevel::Error, "The Scene {0} does already exist", name.c_str());
			return nullptr;
		}

		bool SceneManager::unload_scene(const Gstring& name)
		{
			auto it = m_Scenes.find(name);

			if (it == m_Scenes.end())
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "The Scene {0} does not exist", name.c_str());
				return false;
			}

			if (m_CurrentScene == name)
			{
				set_default_scene();
			}

			it->second->unload();

			m_Scenes.erase(it);

			GRIZZLY_CORE_LOG(LogLevel::Info, "Successfully deleted Scene {0}", name.c_str());

			return true;
		}

		bool SceneManager::save_scene(const Gstring& name, const Gstring& customPath)
		{
			bool success = get_scene(name)->serialize(customPath);

			if (success)
			{
				GRIZZLY_CORE_LOG(LogLevel::Info, "Successfully saved scene: {0}", name.c_str());
			}
			else
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Error while saving scene: {0}", name.c_str());
			}

			return success;
		}

		void SceneManager::on_update(Timestep ts)
		{
			get_scene()->do_update(ts);

			if (m_CustomCamera != nullptr)
			{
				m_CustomCamera->on_update(ts);
				m_Renderer2D->render_scene(ts, get_scene(), { m_CustomCamera->get_projection(), m_CustomCamera->calc_view(), get_scene()->get_ambient_light()});
			}
			else
			{
				m_Renderer2D->render_scene(ts, get_scene());
			}
		}

		bool SceneManager::is_scene_set()
		{
			if (m_CurrentScene == "NONE")
			{
				return false;
			}

			return true;
		}

		bool SceneManager::set_scene(const Gstring& scene)
		{
			if (!exist_scene(scene))
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "The Scene {0} does not exist!", scene.c_str());
				return false;
			}

			m_CurrentScene = scene;

			//TODO: GarbageCollector();
			return true;
		}

		void SceneManager::set_default_scene()
		{
			m_CurrentScene = "default";
		}

		bool SceneManager::exist_scene(const Gstring& scene)
		{
			auto it = m_Scenes.find(scene);

			if (it != m_Scenes.end())
			{
				return true;
			}

			return false;
		}

		Ref<Scene> SceneManager::get_scene()
		{
			GRIZZLY_CORE_ASSERT(m_CurrentScene != "NONE", "No scene!");

			return get_scene(m_CurrentScene);
		}

		Gstring SceneManager::get_scene_name()
		{
			return m_CurrentScene;
		}

		Ref<Scene> SceneManager::get_scene(const Gstring& scene)
		{
			if (!exist_scene(scene))
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Scene: {0} not found!", scene.c_str());
				return nullptr;
			}

			return m_Scenes.at(scene);
		}
	}
}
