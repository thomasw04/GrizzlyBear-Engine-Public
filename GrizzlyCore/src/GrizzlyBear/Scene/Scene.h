#pragma once

#include <set>

#include <GrizzlyBear/Core/BaseDefines.h>
#include "GrizzlyBear/Core/FileIO.h"

#include "EntityManager.h"

#include "GrizzlyBear/Renderer/Systems/Animator2D.h"
#include "GrizzlyBear/Script/ScriptManager.h"

#include "GrizzlyBear/Script/Native/Entity.h"

namespace GrizzlyBear {

	class LookupTable
	{
	public:
		size_t get_id(const Gstring& name) { if (!exist_name(name)) return 0; return m_NameKey[name]; }
		Gstring get_name(size_t id) { if (!exist_id(id))return ""; return m_IDKey[id]; }

		bool exist_name(const Gstring& name) { return m_NameKey.contains(name); }
		bool exist_id(size_t id) { return m_IDKey.contains(id); }

		void create_lookup(const Gstring& name, size_t id) { m_NameKey[name] = id; m_IDKey[id] = name; }
		void delete_lookup(size_t id) { m_NameKey.erase(m_IDKey[id]); m_IDKey.erase(id); }

	private:
		GRIZZLY_REFLECT_VARS
		(
			(std::unordered_map<Gstring, size_t>) m_NameKey,
			(std::unordered_map<size_t, Gstring>) m_IDKey
		)
	};

	GRIZZLY_SERIALIZE_CON_CLASS(LookupTable)

	class Scene
	{

	public:
		Scene(const Gstring& filepath, Ref<Window> window);
		~Scene();

		bool serialize(const Gstring& filepath = "");

		void set_camera(const EntityHandle& entityHandle);
		bool is_entity_alive(const EntityHandle& handle);
		void remove_entity(const EntityHandle& handle);

		EntityHandle get_camera();
		EntityHandle add_entity();
		size_t get_entity_count();

		Ref<Animator2D> get_animator_2d();
		LookupTable& get_lookup_table();
		Ref<Window> get_window();

		void set_native_script(EntityHandle handle, Ref<Entity2D> entity);
		void remove_native_script(EntityHandle handle);

		template<typename Component, typename... InitArgs>
		inline void set_template_ip(const Gstring& name, InitArgs... args)
		{
			m_Registry->set_template_component<Component>(name, {args...});
		}

		template<typename Component>
		inline void set_template_component(const Gstring& name, const Component& comp)
		{
			m_Registry->set_template_component<Component>(name, comp);
		}

		template<typename First, typename... AttachedComponents>
		inline void set_template_entity(const Gstring& entityName, std::initializer_list<std::pair<MemoryType, Gstring>> other)
		{
			m_Registry->set_template_entity<First, AttachedComponents...>(entityName, other, 0);
		}

		inline void set_entity(const EntityHandle& entityHandle, const Gstring& name)
		{
			m_Registry->set_entity(entityHandle, name);
		}

		template<typename Component>
		inline void set_component(const Gstring& name, MemoryType componentType, const EntityHandle& entityHandle)
		{
			m_Registry->set_component<Component>(name, componentType, entityHandle);
		}

		template<typename Component>
		inline bool attach_component(const EntityHandle& entityHandle)
		{
			return m_Registry->attach_component<Component>(entityHandle);
		}

		template<typename Component>
		inline bool detach_component(const EntityHandle& entityHandle)
		{
			return m_Registry->detach_component<Component>(entityHandle);
		}

		template<typename Component>
		inline bool remove_component(const MemoryType componentType, const EntityHandle& entityHandle)
		{
			return m_Registry->remove_component<Component>(componentType, entityHandle);
		}

		template<typename Component>
		inline Component* get_component(const MemoryType componentType, const EntityHandle& entityHandle)
		{
			return m_Registry->get_component<Component>(componentType, entityHandle);
		}

		template<typename Component>
		inline bool has_component(const MemoryType componentType, const EntityHandle& entityHandle)
		{
			return m_Registry->has_component<Component>(componentType, entityHandle);
		}

		template<typename... Components>
		inline GCSIterator<Components...> create_iterator()
		{
			return m_Registry->create_iterator<Components...>();
		}

		inline int get_end()
		{
			return m_Registry->get_end();
		}

		bool on_key(KeyEvent ev, KeyCode code);
		bool on_mouse(MouseEvent ev, MouseCode code);

		void start_simulation(uint32_t entityID);
		void stop_simulation(uint32_t entityID);

		void start_runtime();
		void stop_runtime();

		void do_update(Timestep ts);

		//Validate
		bool is_loaded(bool forceValidation, bool load);

		void unload();

		std::set<uint32_t>::iterator alive_begin();
		std::set<uint32_t>::iterator alive_end();

		//Visual Effects
		AmbientLight get_ambient_light();
		glm::vec4 get_background_color();

		void set_ambient_light(const glm::vec4& color, float intensity);
		void set_background_color(const glm::vec3& color);

		//Intermediate converter
		QuadProps internal_cvt_quad(Timestep ts, EntityHandle handle, CompSprite* mesh);

	private:

		void init(Ref<Window> window);
		void process_header(const Gstring& source);

		//void render_3d_impl(Timestep ts, Ref<FrameBuffer> buffer = nullptr);

	private:
		Ref<Window> m_Window;

		EntityHandle m_CameraHandle = EntityHandle::none();

		bool m_Runtime = false;
		bool m_AssetsLoaded = false;

		Ref<CompManagerType> m_Registry;

		std::set<uint32_t> m_SimulatedEntities;
		std::set<uint32_t> m_AliveEntities;
		std::set<uint32_t> m_DeadEntities;

		LookupTable m_LookupTable;

		std::string m_FilePath;

		//Renderer Systems
		Ref<AssetManager> m_AssetManager;
		Ref<Animator2D> m_Animator2D;

		//Other Systems
		Ref<ScriptingInstance> m_ScriptInstance;

		//Graphic Values
		glm::vec4 m_BackgroundColor;
		AmbientLight m_AmbientLight;
	};
}
