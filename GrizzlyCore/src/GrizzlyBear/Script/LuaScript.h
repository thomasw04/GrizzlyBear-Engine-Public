#pragma once

#include "ScriptManager.h"

#include "GrizzlyBear/Scene/Scene.h"

//Lua include - make sure to use .hpp for c++ compatibility
#include <lua.hpp>

namespace GrizzlyBear {
	
	class LuaScriptingInstance : public ScriptingInstance
	{
	public:
		LuaScriptingInstance(Scene* scene);
		~LuaScriptingInstance();

		virtual void set_framework(const Gstring& filepath) override;

		virtual void load_framework() override;
		virtual void update_framework(Timestep ts) override;
		virtual void shutdown_framework() override;

		virtual void load_script(const Gstring& filepath, uint32_t entityID) override;
		virtual void delete_script(uint32_t entityID) override;

		virtual void on_spawn(uint32_t entityID) override;
		virtual void on_update(Timestep ts, uint32_t entityID) override;
		virtual void on_delete(uint32_t entityID) override;

	private:

		lua_Debug get_invoc_info();
		void register_key_codes();
		void register_enums();

		//Functions
		int LOG_TRACE(lua_State* L);
		int LOG_INFO(lua_State* L);
		int LOG_WARN(lua_State* L);
		int LOG_ERROR(lua_State* L);
		int LOG_FATAL(lua_State* L);

		Gstring GetLog(lua_State* L);

		//Entity
		int set_position(lua_State* L);
		int get_position(lua_State* L);

		int set_rotation(lua_State* L);
		int get_rotation(lua_State* L);

		int set_size(lua_State* L);
		int get_size(lua_State* L);

		int is_key_pressed(lua_State* L);
		int is_mouse_pressed(lua_State* L);

		int play_animation(lua_State* L);
		int load_animation(lua_State* L);

		int get_component(lua_State* L);
		int has_component(lua_State* L);
		int set_component(lua_State* L);

		int get_entity(lua_State* L);

		uint32_t get_entity_index(lua_State* L, int tableIndex);

		//Framework

		//int spawn_entity(lua_State* L);
		//int build_entity(lua_State* L);

	private:
		lua_State* m_L;
	
		Scene* m_Scene;

	public:
		static std::unordered_map<lua_State*, void*> s_ExtraSpace;
	};
}


