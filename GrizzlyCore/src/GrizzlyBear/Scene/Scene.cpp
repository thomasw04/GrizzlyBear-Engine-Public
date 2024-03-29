#include <gbpch.h>

#include "Scene.h"
#include "SerializeUtils.h"

#include "GrizzlyBear/Renderer/Renderer2D.h"
#include "GrizzlyBear/Renderer/Renderer3D.h"

#include "GrizzlyBear/Renderer/GuiRenderer.h"

#include "GrizzlyBear/Misc/Utils.h"

#include "GrizzlyBear/Environment/Environment.h" 



namespace GrizzlyBear {

	//Scene----------------------------------------------------------------

	Scene::Scene(const Gstring& filepath, Ref<Window> window)
	{
		init(window);

		m_FilePath = filepath;

		if (filepath != "")
		{
			Gbuffer buffer = Environment::get_fetcher()->get_asset_data(filepath);

			if (!buffer.is_good())
			{
				//If the buffer is damaged do the standard init
				m_Registry = CreateRef<CompManagerType>();
				return;
			}

			Gstring& source = buffer.m_Buffer;

			//Now we are processing the file to islolate the header
			size_t beginHeader = source.find(GCSTokens::BEGIN_HEADER, 0);
			size_t beginBody = source.find(GCSTokens::BEGIN_BODY, 0);

			int add = 1;

			if (source[beginHeader + strlen(GCSTokens::BEGIN_HEADER) + 1] == '\n') add++;

			size_t beginHContent = beginHeader + strlen(GCSTokens::BEGIN_HEADER) + add;
			size_t beginBContent = beginBody + strlen(GCSTokens::BEGIN_BODY) + add;

			//Now process the header
			process_header(source.substr(beginHContent, beginBody - beginHContent));

			//And last process the rest of the file
			m_Registry = CreateRef<CompManagerType>(source.substr(beginBContent));
		}
		else
		{
			m_Registry = CreateRef<CompManagerType>();
		}
	}

	void Scene::process_header(const Gstring& source)
	{
		//Next we check if the gcs version in the file is compatible with our version.
		char major, minor;

		major = source[1];
		minor = source[2];

		GRIZZLY_CORE_ASSERT(major >= GRIZZLYBEAR_VERSION_GCS_MAJOR && minor >= GRIZZLYBEAR_VERSION_GCS_MINOR, "This is an outdated version file");

		//If everything worked fine, retrieve the the list of alive entities and dead entities.
		std::string object = source.substr(3);

 		nlohmann::json json = nlohmann::json::parse(object);

		GrizzlyBear::Reflector::deserialize(m_AliveEntities, json["AliveEntities"]);
		GrizzlyBear::Reflector::deserialize(m_LookupTable, json["LookupTable"]);

		std::set<uint32_t> ids;

		//Insert the dead entities
		std::set_difference(m_DeadEntities.begin(), m_DeadEntities.end(), m_AliveEntities.begin(), m_AliveEntities.end(), std::inserter(ids, ids.end()));
		m_DeadEntities.clear();
		m_DeadEntities = ids;
	}

	bool Scene::serialize(const Gstring& filepath)
	{
		if (filepath != "")
		{
			m_FilePath = filepath;
		}

		std::stringstream sstream;

		//Now write the header
		sstream << GCSTokens::BEGIN_HEADER << '\n';

		//Now write the version
		char major = GRIZZLYBEAR_VERSION_GCS_MAJOR;
		char minor = GRIZZLYBEAR_VERSION_GCS_MINOR;

		sstream << 'W';
		sstream << major;
		sstream << minor;
		sstream << '\n';

		nlohmann::json json;

		//Now write Alive Entities to the json
		GrizzlyBear::Reflector::serialize(m_AliveEntities, json["AliveEntities"]);
		GrizzlyBear::Reflector::serialize(m_LookupTable, json["LookupTable"]);

		sstream << json.dump(GCSTokens::JSON_FORMAT) << '\n';

		//And finally begin the body
		sstream << GCSTokens::BEGIN_BODY << '\n';

		//At last we are writing the whole ecs to the file
		m_Registry->serialize(sstream);

		return Environment::get_fetcher()->save_asset_data(m_FilePath, { sstream.str(), Gbuffer::State::GOOD });
	}

	Scene::~Scene()
	{
		m_Registry.reset();
	}

	void Scene::init(Ref<Window> window)
	{
		for (int i = 1; i < TableType::MAX_ENTITIES; i++)
		{
			m_DeadEntities.insert(i);
		}

		m_Window = window;

		m_AssetManager = CreateRef<AssetManager>(window->get_context());
		m_Animator2D = CreateRef<Animator2D>();

		m_ScriptInstance = ScriptingInstance::create_instance(this);
	}

	void Scene::set_camera(const EntityHandle& entityHandle)
	{
		m_CameraHandle = entityHandle.get_id();
	}

	EntityHandle Scene::get_camera()
	{
		return m_CameraHandle;
	}

	EntityHandle Scene::add_entity()
	{
		uint32_t index = *m_DeadEntities.begin();
		EntityHandle handle(index);

		m_AliveEntities.insert(index);

		m_DeadEntities.erase(m_DeadEntities.begin());

		m_Registry->attach_component<CompTransformation>(index, false);

		return handle;
	}

	void Scene::remove_entity(const EntityHandle& handle)
	{
		auto it = m_AliveEntities.find(handle.get_id());

		if (it != m_AliveEntities.end())
		{
			m_DeadEntities.insert(handle.get_id());

			m_AliveEntities.erase(it);

			m_Registry->remove_all_components(handle);
		}
	}

	bool Scene::is_entity_alive(const EntityHandle& handle)
	{
		return m_AliveEntities.find(handle.get_id()) != m_AliveEntities.end();
	}

	size_t Scene::get_entity_count()
	{
		return m_AliveEntities.size();
	}

	Ref<Animator2D> Scene::get_animator_2d()
	{
		return m_Animator2D;
	}

	LookupTable& Scene::get_lookup_table()
	{
		return m_LookupTable;
	}

	Ref<Window> Scene::get_window()
	{
		return m_Window;
	}

	void Scene::set_native_script(EntityHandle handle, Ref<Entity2D> entity)
	{
		m_ScriptInstance->m_Native[handle.get_id()] = entity;
		entity->set_scene(this, handle);

		m_Registry->attach_component<CompScript>(handle, false);
		m_Registry->get_component<CompScript>(MemoryType::Dynamic, handle)->NATIVE = true;
	}

	void Scene::remove_native_script(EntityHandle handle)
	{
		m_Registry->get_component<CompScript>(MemoryType::Dynamic, handle)->NATIVE = false;
	}

	bool Scene::on_key(KeyEvent ev, KeyCode code)
	{
		bool returnB = false;

		for (auto it = create_iterator<CompScript>(); it != m_Registry->get_end(); it++)
		{
			auto comp = it.get<CompScript>();
			if (comp->NATIVE)
			{
				if (m_ScriptInstance->m_Native[it.get_index()]->m_GetEvents)
				{
					if (m_ScriptInstance->m_Native[it.get_index()]->on_key(ev, code))
					{
						returnB = true;
					}
				}
			}
			else
			{
				//TODO
			}
		}

		return returnB;
	}

	bool Scene::on_mouse(MouseEvent ev, MouseCode code)
	{
		bool returnB = false;

		for (auto it = create_iterator<CompScript>(); it != m_Registry->get_end(); it++)
		{
			auto comp = it.get<CompScript>();
			if (comp->NATIVE)
			{
				if (m_ScriptInstance->m_Native[it.get_index()]->m_GetEvents)
				{
					if (m_ScriptInstance->m_Native[it.get_index()]->on_mouse(ev, code))
					{
						returnB = true;
					}
				}
			}
			else
			{
				//TODO
			}
		}

		return returnB;
	}

	void Scene::start_simulation(uint32_t entityID)
	{
		if (has_component<CompScript>(MemoryType::Dynamic, entityID))
		{
			if (!m_SimulatedEntities.contains(entityID))
			{
				auto comp = get_component<CompScript>(MemoryType::Dynamic, entityID);

				if (comp->NATIVE)
				{
					auto& sc = m_ScriptInstance->m_Native[entityID];
					sc->_fetch();
					sc->on_spawn();
					sc->_sync();
				}
				else
				{
					m_ScriptInstance->load_script(comp->FILE_PATH, entityID);
					m_ScriptInstance->on_spawn(entityID);
				}

				m_SimulatedEntities.insert(entityID);
			}
		}
		else
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Scene, Message: This is not a scriptable entity.");
		}
	}

	void Scene::stop_simulation(uint32_t entityID)
	{
		auto it = m_SimulatedEntities.find(entityID);

		if (it != m_SimulatedEntities.end())
		{
			if (has_component<CompScript>(MemoryType::Dynamic, entityID))
			{
				auto comp = get_component<CompScript>(MemoryType::Dynamic, entityID);

				if (comp->NATIVE)
				{
					auto& sc = m_ScriptInstance->m_Native[entityID];
					sc->on_delete();
				}
				else
				{
					m_ScriptInstance->on_delete(entityID);
					m_ScriptInstance->delete_script(entityID);
				}
			}

			m_SimulatedEntities.erase(entityID);
		}
		else
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Scene, Message: This entity is currently not simulated.");
		}
	}

	void Scene::start_runtime()
	{
		m_Runtime = true;

		for (auto it = create_iterator<CompScript>(); it != m_Registry->get_end(); it++)
		{
			auto comp = it.get<CompScript>();
			if (comp->NATIVE)
			{
				m_ScriptInstance->m_Native[it.get_index()]->_fetch();
				m_ScriptInstance->m_Native[it.get_index()]->on_spawn();
				m_ScriptInstance->m_Native[it.get_index()]->_sync();
			}
			else
			{
				m_ScriptInstance->load_script(comp->FILE_PATH, it.get_index());
				m_ScriptInstance->on_spawn(it.get_index());
			}	
		}
	}

	void Scene::stop_runtime()
	{
		m_Runtime = false;

		for (auto it = create_iterator<CompScript>(); it != m_Registry->get_end(); it++)
		{
			auto comp = it.get<CompScript>();

			if (comp->NATIVE)
			{
				m_ScriptInstance->m_Native[it.get_index()]->on_delete();
			}
			else
			{
				m_ScriptInstance->on_delete(it.get_index());
				m_ScriptInstance->delete_script(it.get_index());
			}
		}
	}

	void Scene::do_update(Timestep ts)
	{
		m_AssetManager->update();

		if (m_Runtime)
		{
			for (auto it = create_iterator<CompScript>(); it != m_Registry->get_end(); it++)
			{
				auto comp = it.get<CompScript>();

				if (comp->NATIVE)
				{
					m_ScriptInstance->m_Native[it.get_index()]->on_update(ts);
					m_ScriptInstance->m_Native[it.get_index()]->_sync();
				}
				else
				{
					m_ScriptInstance->on_update(ts, it.get_index());
				}
			}
		}
		else if (!m_SimulatedEntities.empty())
		{
			for (uint32_t index : m_SimulatedEntities)
			{
				if (get_component<CompScript>(MemoryType::Dynamic, index)->NATIVE)
				{
					m_ScriptInstance->m_Native[index]->on_update(ts);
					m_ScriptInstance->m_Native[index]->_sync();
				}
				else
				{
					m_ScriptInstance->on_update(ts, index);
				}

			}
		}
	}

	bool Scene::is_loaded(bool forceValidation, bool load)
	{
		if (!forceValidation && m_AssetsLoaded && !load) return true;

		for (auto it = create_iterator<CompSprite>(); it != m_Registry->get_end(); it++)
		{
			auto mesh = it.get<CompSprite>();

			if (!m_AssetManager->is_available(Asset::Texture2D, mesh->MATERIAL.TEXTURE_FILEPATH))
			{
				if (load)
				{
					m_AssetManager->request_asset(Asset::Texture2D, mesh->MATERIAL.TEXTURE_FILEPATH, mesh->MATERIAL.STREAMABLE);

					if (!m_AssetManager->is_available(Asset::Texture2D, mesh->MATERIAL.TEXTURE_FILEPATH))
					{
						return false;
					}
				}

				return false;
			}
		}

		m_AssetsLoaded = true;
		return true;
	}

	void Scene::unload()
	{
	}

	/*void Scene::render_3d_impl(Timestep ts, Ref<FrameBuffer> buffer)
	{
		if (!has_component<CompCamera>(MemoryType::Dynamic, m_CameraHandle)) return;

		CompCamera* camera = get_component<CompCamera>(MemoryType::Dynamic, m_CameraHandle);
		CompTransformation3D* camTransform = get_component<CompTransformation3D>(MemoryType::Dynamic, m_CameraHandle);

		//y before x cause pitch (x) is based on yaw (y).
		glm::mat4 camTransformationMatrix = glm::translate(glm::mat4(1.0f), camTransform->m_Position) 
		* glm::rotate(glm::mat4(1.0f), glm::radians(camTransform->m_Rotation.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(camTransform->m_Rotation.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(camTransform->m_Rotation.z), glm::vec3(0, 0, 1));

		Renderer::begin_frame(buffer);

		Renderer3D::begin_scene(camera->m_ProjectionMatrix, camTransformationMatrix);

		for (auto it = create_iterator<CompTransformation3D>(); it != m_Registry->get_end(); it++)
		{
			auto transform = it.get<CompTransformation3D>();

			Renderer3D::submit(transform->m_Position, transform->m_Rotation, transform->m_Size, AssetManager::get_texture("textures/Chessboard.png"));
		}

		Renderer3D::end_scene();

		Renderer::end_frame();
	}*/

	std::set<uint32_t>::iterator Scene::alive_begin()
	{
		return m_AliveEntities.begin();
	}

	std::set<uint32_t>::iterator Scene::alive_end()
	{
		return m_AliveEntities.end();
	}

	AmbientLight Scene::get_ambient_light()
	{
		return m_AmbientLight;
	}

	glm::vec4 Scene::get_background_color()
	{
		return m_BackgroundColor;
	}

	void Scene::set_ambient_light(const glm::vec4& color, float intensity)
	{
		m_AmbientLight.COLOR = color;
		m_AmbientLight.INTENSITY = intensity;
	}

	void Scene::set_background_color(const glm::vec3& color)
	{
		m_BackgroundColor.r = color.r;
		m_BackgroundColor.g = color.g;
		m_BackgroundColor.b = color.b;
		m_BackgroundColor.a = 1.0f;
	}

	QuadProps Scene::internal_cvt_quad(Timestep ts, EntityHandle handle, CompSprite* mesh)
	{
		//Update Systems
		m_Animator2D->update_animation(handle.get_id(), mesh->MATERIAL.TEXTURE_COORDINATES, mesh->MATERIAL.TEXTURE_FILEPATH, ts);

		QuadProps returnProps;

		returnProps.TINT = mesh->MATERIAL.TINT;
		returnProps.TILING_FACTOR = mesh->MATERIAL.TILING_FACTOR;
		returnProps.HIGHLIGHT = mesh->MATERIAL.HIGHLIGHT;
		returnProps.TEXTURE_COORDINATES = mesh->MATERIAL.TEXTURE_COORDINATES;
		returnProps.TEXTURE = m_AssetManager->get_texture(mesh->MATERIAL.TEXTURE_FILEPATH);

		return returnProps;
	}
}