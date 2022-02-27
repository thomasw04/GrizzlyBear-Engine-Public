#include <gbpch.h>

#include "LuaScript.h"

#include "GrizzlyBear/Environment/Environment.h"

namespace GrizzlyBear {


	int LuaScriptingInstance::LOG_TRACE(lua_State* L)
	{
		GRIZZLY_CLIENT_LOG(LogLevel::Trace, GetLog(L));

		return 0;
	}

	int LuaScriptingInstance::LOG_INFO(lua_State* L)
	{
		GRIZZLY_CLIENT_LOG(LogLevel::Info, GetLog(L));

		return 0;
	}

	int LuaScriptingInstance::LOG_WARN(lua_State* L)
	{
		GRIZZLY_CLIENT_LOG(LogLevel::Warn, GetLog(L));

		return 0;
	}

	int LuaScriptingInstance::LOG_ERROR(lua_State* L)
	{
		GRIZZLY_CLIENT_LOG(LogLevel::Error, GetLog(L));

		return 0;
	}

	int LuaScriptingInstance::LOG_FATAL(lua_State* L)
	{
		GRIZZLY_CLIENT_LOG(LogLevel::Fatal, GetLog(L));

		return 0;
	}

	Gstring LuaScriptingInstance::GetLog(lua_State* L)
	{
		int args = lua_gettop(L);

		Gstring string = "";

		for (int i = args; i > 0; i--)
		{
			if (lua_isstring(L, -i))
			{
				string += lua_tostring(L, -i);
			}
		}

		return string;
	}

	int LuaScriptingInstance::set_position(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 3 && args != 4)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 3 or 4. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		glm::vec3 position;

		if (args == 4)
		{
			if (lua_isnumber(L, -1) && lua_isnumber(L, -2) && lua_isnumber(L, -3))
			{
				position.x = (float)lua_tonumber(L, -3);
				position.y = (float)lua_tonumber(L, -2);
				position.z = (float)lua_tonumber(L, -1);
			}
			else
			{
				lua_Debug d = get_invoc_info();
				Log::lua_error_log("Invalid argument type. Expected: number.", d.source, d.currentline);
			}
		}
		else
		{
			if (lua_isnumber(L, -1) && lua_isnumber(L, -2))
			{
				position.x = (float)lua_tonumber(L, -2);
				position.y = (float)lua_tonumber(L, -1);
				position.z = 0;
			}
			else
			{
				lua_Debug d = get_invoc_info();
				Log::lua_error_log("Invalid argument type. Expected: number.", d.source, d.currentline);
				return 0;
			}
		}

		int entityIndex;

		if (args == 4) entityIndex = get_entity_index(L, -4);
		if (args == 3) entityIndex = get_entity_index(L, -3);

		if (entityIndex != 0)
		{
			CompTransformation2D* transform = m_Scene->get_component<CompTransformation2D>(MemoryType::Dynamic, entityIndex);

			if (args == 3)
				transform->m_Position = position;

			if (args == 2)
			{
				transform->m_Position.x = position.x;
				transform->m_Position.y = position.y;
			}
		}

		return 0;
	}

	int LuaScriptingInstance::get_position(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 1)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		int entityIndex = get_entity_index(L, -1);

		if (entityIndex != 0)
		{
			CompTransformation2D* transform = m_Scene->get_component<CompTransformation2D>(MemoryType::Dynamic, entityIndex);

			lua_newtable(L);

			lua_pushstring(L, "x");
			lua_pushnumber(L, transform->m_Position.x);
			lua_settable(L, -3);

			lua_pushstring(L, "y");
			lua_pushnumber(L, transform->m_Position.y);
			lua_settable(L, -3);

			lua_pushstring(L, "z");
			lua_pushnumber(L, transform->m_Position.z);
			lua_settable(L, -3);
			return 1;
		}

		return 0;
	}

	int LuaScriptingInstance::set_rotation(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 2)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 2. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		float rotation;

		if (lua_isnumber(L, -1))
		{
			rotation = (float)lua_tonumber(L, -1);
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid argument type. Expected: number.", d.source, d.currentline);
			return 0;
		}

		int entityIndex = get_entity_index(L, -2);

		if (entityIndex != 0)
		{
			CompTransformation2D* transform = m_Scene->get_component<CompTransformation2D>(MemoryType::Dynamic, entityIndex);

			transform->m_Rotation = rotation;
		}

		return 0;
	}

	int LuaScriptingInstance::get_rotation(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 1)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		int entityIndex = get_entity_index(L, -1);

		if (entityIndex != 0)
		{
			CompTransformation2D* transform = m_Scene->get_component<CompTransformation2D>(MemoryType::Dynamic, entityIndex);

			lua_pushnumber(L, transform->m_Rotation);
			return 1;
		}

		return 0;
	}

	int LuaScriptingInstance::set_size(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 3)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 3. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		glm::vec2 size;

		if (lua_isnumber(L, -1) && lua_isnumber(L, -2))
		{
			size.x = (float)lua_tonumber(L, -2);
			size.y = (float)lua_tonumber(L, -1);
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid argument type. Expected: number.", d.source, d.currentline);
			return 0;
		}

		int entityIndex = get_entity_index(L, -3);

		if (entityIndex != 0)
		{
			CompTransformation2D* transform = m_Scene->get_component<CompTransformation2D>(MemoryType::Dynamic, entityIndex);

			transform->m_Size = size;
		}

		return 0;
	}

	int LuaScriptingInstance::get_size(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 1)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		int entityIndex = get_entity_index(L, -1);

		if (entityIndex != 0)
		{
			CompTransformation2D* transform = m_Scene->get_component<CompTransformation2D>(MemoryType::Dynamic, entityIndex);

			lua_newtable(L);

			lua_pushstring(L, "x");
			lua_pushnumber(L, transform->m_Size.x);
			lua_settable(L, -3);

			lua_pushstring(L, "y");
			lua_pushnumber(L, transform->m_Size.y);
			lua_settable(L, -3);

			return 1;
		}

		return 0;
	}

	int LuaScriptingInstance::is_key_pressed(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 1)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		if (lua_isnumber(L, -1) || lua_isstring(L, -1))
		{
			bool pressed = m_Scene->get_window()->is_key_pressed((int)lua_tointeger(L, -1));
			lua_pushboolean(L, pressed);
			return 1;
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid argument type. Expected: number.", d.source, d.currentline);
			return 0;
		}
	}

	int LuaScriptingInstance::is_mouse_pressed(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 1)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		if (lua_isnumber(L, -1) || lua_isstring(L, -1))
		{
			bool pressed = m_Scene->get_window()->is_mouse_pressed((int)lua_tointeger(L, -1));
			lua_pushboolean(L, pressed);
			return 1;
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid argument type. Expected: number.", d.source, d.currentline);
			return 0;
		}
	}

	int LuaScriptingInstance::play_animation(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 2)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 2. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		if (lua_isstring(L, -1))
		{
			m_Scene->get_animator_2d()->play_animation(lua_tostring(L, -1), get_entity_index(L, -2));
			return 0;
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Inalid argument type. Expected: string.", d.source, d.currentline);
			return 0;
		}
	}

	int LuaScriptingInstance::load_animation(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 1)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		if (lua_isstring(L, -1))
		{
			if (!m_Scene->get_animator_2d()->exist_animation(lua_tostring(L, -1)))
			{
				m_Scene->get_animator_2d()->load_animation(lua_tostring(L, -1));
				return 0;
			}
			else
			{
				GRIZZLY_CORE_LOG(LogLevel::Warn, "Animation {0} already loaded - Skipping", lua_tostring(L, -1));
				return 0;
			}
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid argument type. Expected: string.", d.source, d.currentline);
			return 0;
		}

		return 0;
	}

	int LuaScriptingInstance::get_component(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args < 2)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: > 1. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		int entityIndex = get_entity_index(L, -args);

		if (lua_isstring(L, -(args - 1)))
		{
			const char* type = lua_tostring(L, -(args - 1));
		}

		return 0;
	}

	int LuaScriptingInstance::has_component(lua_State* L)
	{
		return 0;
	}

	int LuaScriptingInstance::set_component(lua_State* L)
	{
		return 0;
	}

	int LuaScriptingInstance::get_entity(lua_State* L)
	{
		int args = lua_gettop(L);

		if (args != 2)
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid number of arguments. Expected: 2. Given: {2}", d.source, d.currentline, args);
			return 0;
		}

		if (lua_isnumber(L, -1))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, std::to_string(lua_tointeger(L, -1)).c_str());
			lua_getfield(L, -1, "Entity");
			return 1;
		}
		else if (lua_isstring(L, -1))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, std::to_string(m_Scene->get_lookup_table().get_id(lua_tostring(L, -1))).c_str());
			lua_getfield(L, -1, "Entity");
			return 1;
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid argument type. Expected: integer or string.", d.source, d.currentline);
			return 0;
		}
	}

	uint32_t LuaScriptingInstance::get_entity_index(lua_State* L, int tableIndex)
	{
		if (lua_istable(L, tableIndex))
		{
			lua_getfield(L, tableIndex, "__entityIndex__");

			if (lua_isnumber(L, -1))
			{
				int returnIndex = (int)lua_tointeger(L, -1);
				lua_pop(L, 1);
				return returnIndex;
			}
			else
			{
				lua_Debug d = get_invoc_info();
				Log::lua_error_log("Invalid type __entityIndex__. Expected: integer.", d.source, d.currentline);
				lua_pop(L, 1);
				return 0;
			}
		}
		else
		{
			lua_Debug d = get_invoc_info();
			Log::lua_error_log("Invalid type self. Expected: table.", d.source, d.currentline);
			return 0;
		}
	}

	typedef int (LuaScriptingInstance::* func_reg)(lua_State* L);

	template<func_reg func>
	int func_dispatcher(lua_State* L)
	{
		LuaScriptingInstance* p = static_cast<LuaScriptingInstance*>(LuaScriptingInstance::s_ExtraSpace[L]);
		return ((*p).*func)(L);
	}

	std::unordered_map<lua_State*, void*> LuaScriptingInstance::s_ExtraSpace;

	LuaScriptingInstance::LuaScriptingInstance(Scene* scene)
	{
		GRIZZLY_CORE_LOG(LogLevel::Trace, "Init script module...");

		m_Scene = scene;

		m_L = luaL_newstate();
		luaL_openlibs(m_L);

		luaJIT_setmode(m_L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);

		s_ExtraSpace[m_L] = this;

		lua_register(m_L, "LOG_TRACE", &func_dispatcher<&LuaScriptingInstance::LOG_TRACE>);
		lua_register(m_L, "LOG_INFO", &func_dispatcher<&LuaScriptingInstance::LOG_INFO>);
		lua_register(m_L, "LOG_WARN", &func_dispatcher<&LuaScriptingInstance::LOG_WARN>);
		lua_register(m_L, "LOG_ERROR", &func_dispatcher<&LuaScriptingInstance::LOG_ERROR>);
		lua_register(m_L, "LOG_FATAL", &func_dispatcher<&LuaScriptingInstance::LOG_FATAL>);

		register_enums();
		register_key_codes();

		lua_register(m_L, "isKeyPressed", &func_dispatcher<&LuaScriptingInstance::is_key_pressed>);
		lua_register(m_L, "isMousePressed", &func_dispatcher<&LuaScriptingInstance::is_mouse_pressed>);
		lua_register(m_L, "loadAnimation", &func_dispatcher<&LuaScriptingInstance::load_animation>);
	}

	LuaScriptingInstance::~LuaScriptingInstance()
	{
		lua_close(m_L);
	}

	void LuaScriptingInstance::on_spawn(uint32_t entityID)
	{
		lua_getfield(m_L, LUA_REGISTRYINDEX, std::to_string(entityID).c_str());

		if (lua_isnil(m_L, -1))
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Script, Message: The entity {0} has no script applied!", entityID);
			return;
		}

		lua_getfield(m_L, -1, "Entity");
		lua_getfield(m_L, -1, "onSpawn");

		lua_getfield(m_L, -3, "Entity");

		int errorCode = lua_pcall(m_L, 1, 0, 0);

		if (errorCode != 0)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
		}

		lua_settop(m_L, 0);
	}

	void LuaScriptingInstance::on_update(Timestep ts, uint32_t entityID)
	{
		lua_getfield(m_L, LUA_REGISTRYINDEX, std::to_string(entityID).c_str());

		if (lua_isnil(m_L, -1))
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Script, Message: The entity {0} has no script applied!", entityID);
			return;
		}

		lua_getfield(m_L, -1, "Entity");
		lua_getfield(m_L, -1, "onUpdate");

		lua_getfield(m_L, -3, "Entity");
		lua_pushnumber(m_L, ts.get_millis());

		int errorCode = lua_pcall(m_L, 2, 0, 0);

		if (errorCode != 0)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
		}

		lua_settop(m_L, 0);
	}

	void LuaScriptingInstance::on_delete(uint32_t entityID)
	{
		lua_getfield(m_L, LUA_REGISTRYINDEX, std::to_string(entityID).c_str());

		if (lua_isnil(m_L, -1))
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Script, Message: The entity {0} has no script applied!", entityID);
			return;
		}

		lua_getfield(m_L, -1, "Entity");
		lua_getfield(m_L, -1, "onDelete");

		lua_getfield(m_L, -3, "Entity");

		int errorCode = lua_pcall(m_L, 1, 0, 0);

		if (errorCode != 0)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
		}

		lua_settop(m_L, 0);
	}

	void LuaScriptingInstance::set_framework(const Gstring& filepath)
	{
		//Create reference to file in lua table
		lua_newtable(m_L);
		lua_newtable(m_L);

		lua_getglobal(m_L, "_G");
		lua_setfield(m_L, -2, "__index");
		lua_setmetatable(m_L, -2);

		int e = luaL_loadfile(m_L, Environment::get_fetcher()->get_full_asset_path(filepath).c_str());

		if (e != LUA_OK)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
			return;
		}

		lua_pushvalue(m_L, -2);

		lua_setfenv(m_L, -2);

		//Run non function content (Not recommended due to unexpectable behaviour)
		int errorCode = lua_pcall(m_L, 0, LUA_MULTRET, 0);

		if (errorCode != LUA_OK)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
		}

		lua_setfield(m_L, LUA_REGISTRYINDEX, "Framework");
		lua_getfield(m_L, LUA_REGISTRYINDEX, "Framework");

		lua_getfield(m_L, -1, "Scene");

		if (lua_istable(m_L, -1))
		{
			lua_pushstring(m_L, "__name__");
			//lua_pushinteger(m_L, entityID);
			lua_settable(m_L, -3);

			//Register all functions
			lua_pushstring(m_L, "build_entity");
			//lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::set_position>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "spawnEntity");
			//lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::set_position>);
			lua_settable(m_L, -3);
		}
		else
		{
			lua_pop(m_L, 1);
		}

		//Pop table when everything is registered
		lua_pop(m_L, 1);

		lua_settop(m_L, 0);

		return;
	}

	void LuaScriptingInstance::load_framework()
	{
	}

	void LuaScriptingInstance::update_framework(Timestep ts)
	{
	}

	void LuaScriptingInstance::shutdown_framework()
	{
	}

	void LuaScriptingInstance::load_script(const Gstring& filepath, uint32_t entityID)
	{
		//Create reference to file in lua table
		lua_newtable(m_L);
		lua_newtable(m_L);

		lua_getglobal(m_L, "_G");
		lua_setfield(m_L, -2, "__index");
		lua_setmetatable(m_L, -2); 

		int e = luaL_loadfile(m_L, Environment::get_fetcher()->get_full_asset_path(filepath).c_str());

		if (e != LUA_OK)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
			return;
		}

		lua_pushvalue(m_L, -2);

		lua_setfenv(m_L, -2);

		//Run non function content (Not recommended due to unexpectable behaviour)
		int errorCode = lua_pcall(m_L, 0, LUA_MULTRET, 0);

		if (errorCode != LUA_OK)
		{
			GRIZZLY_CLIENT_LOG(LogLevel::Error, lua_tostring(m_L, -1));
		}

		lua_setfield(m_L, LUA_REGISTRYINDEX, std::to_string(entityID).c_str());
		lua_getfield(m_L, LUA_REGISTRYINDEX, std::to_string(entityID).c_str());

		lua_getfield(m_L, -1, "Entity");

		if (lua_istable(m_L, -1))
		{
			lua_pushstring(m_L, "__entityIndex__");
			lua_pushinteger(m_L, entityID);
			lua_settable(m_L, -3);

			//Register all functions
			lua_pushstring(m_L, "setPosition");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::set_position>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "getPosition");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::get_position>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "setRotation");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::set_rotation>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "getRotation");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::get_rotation>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "setSize");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::set_size>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "getSize");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::get_size>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "getEntity");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::get_entity>);
			lua_settable(m_L, -3);

			lua_pushstring(m_L, "playAnimation");
			lua_pushcfunction(m_L, &func_dispatcher<&LuaScriptingInstance::play_animation>);
			lua_settable(m_L, -3);
		}
		else
		{
			lua_pop(m_L, 1);
		}

		//Pop table when everything is registered
		lua_pop(m_L, 1);

		lua_settop(m_L, 0);

		return;
	}

	void LuaScriptingInstance::delete_script(uint32_t entityID)
	{
		lua_pushnil(m_L);
		lua_setfield(m_L, LUA_REGISTRYINDEX, std::to_string(entityID).c_str());

		//Garbage collect the tables etc.
		lua_gc(m_L, LUA_GCCOLLECT, 0);
	}

	lua_Debug LuaScriptingInstance::get_invoc_info()
	{
		lua_Debug info;
		lua_getstack(m_L, 1, &info);
		lua_getinfo(m_L, "nSltu", &info);
		return info;
	}


	void LuaScriptingInstance::register_key_codes()
	{
		#define REGISTER_KEY_CODE(x) lua_pushnumber(m_L, x); lua_setglobal(m_L, #x)

		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_1);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_2);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_3);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_4);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_5);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_6);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_7);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_8);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_LEFT);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_RIGHT);
		REGISTER_KEY_CODE(GRIZZLY_MOUSE_BUTTON_MIDDLE);

		REGISTER_KEY_CODE(GRIZZLY_KEY_SPACE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_APOSTROPHE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_COMMA);
		REGISTER_KEY_CODE(GRIZZLY_KEY_MINUS);
		REGISTER_KEY_CODE(GRIZZLY_KEY_PERIOD);
		REGISTER_KEY_CODE(GRIZZLY_KEY_SLASH);
		REGISTER_KEY_CODE(GRIZZLY_KEY_0);
		REGISTER_KEY_CODE(GRIZZLY_KEY_1);
		REGISTER_KEY_CODE(GRIZZLY_KEY_2);
		REGISTER_KEY_CODE(GRIZZLY_KEY_3);
		REGISTER_KEY_CODE(GRIZZLY_KEY_4);
		REGISTER_KEY_CODE(GRIZZLY_KEY_5);
		REGISTER_KEY_CODE(GRIZZLY_KEY_6);
		REGISTER_KEY_CODE(GRIZZLY_KEY_7);
		REGISTER_KEY_CODE(GRIZZLY_KEY_8);
		REGISTER_KEY_CODE(GRIZZLY_KEY_9);
		REGISTER_KEY_CODE(GRIZZLY_KEY_SEMICOLON);
		REGISTER_KEY_CODE(GRIZZLY_KEY_EQUAL);
		REGISTER_KEY_CODE(GRIZZLY_KEY_A);
		REGISTER_KEY_CODE(GRIZZLY_KEY_B);
		REGISTER_KEY_CODE(GRIZZLY_KEY_C);
		REGISTER_KEY_CODE(GRIZZLY_KEY_D);
		REGISTER_KEY_CODE(GRIZZLY_KEY_E);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F);
		REGISTER_KEY_CODE(GRIZZLY_KEY_G);
		REGISTER_KEY_CODE(GRIZZLY_KEY_H);
		REGISTER_KEY_CODE(GRIZZLY_KEY_I);
		REGISTER_KEY_CODE(GRIZZLY_KEY_J);
		REGISTER_KEY_CODE(GRIZZLY_KEY_K);
		REGISTER_KEY_CODE(GRIZZLY_KEY_L);
		REGISTER_KEY_CODE(GRIZZLY_KEY_M);
		REGISTER_KEY_CODE(GRIZZLY_KEY_N);
		REGISTER_KEY_CODE(GRIZZLY_KEY_O);
		REGISTER_KEY_CODE(GRIZZLY_KEY_P);
		REGISTER_KEY_CODE(GRIZZLY_KEY_Q);
		REGISTER_KEY_CODE(GRIZZLY_KEY_R);
		REGISTER_KEY_CODE(GRIZZLY_KEY_S);
		REGISTER_KEY_CODE(GRIZZLY_KEY_T);
		REGISTER_KEY_CODE(GRIZZLY_KEY_U);
		REGISTER_KEY_CODE(GRIZZLY_KEY_V);
		REGISTER_KEY_CODE(GRIZZLY_KEY_W);
		REGISTER_KEY_CODE(GRIZZLY_KEY_X);
		REGISTER_KEY_CODE(GRIZZLY_KEY_Y);
		REGISTER_KEY_CODE(GRIZZLY_KEY_Z);
		REGISTER_KEY_CODE(GRIZZLY_KEY_LEFT_BRACKET);
		REGISTER_KEY_CODE(GRIZZLY_KEY_BACKSLASH);
		REGISTER_KEY_CODE(GRIZZLY_KEY_RIGHT_BRACKET);
		REGISTER_KEY_CODE(GRIZZLY_KEY_GRAVE_ACCENT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_ESCAPE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_ENTER);
		REGISTER_KEY_CODE(GRIZZLY_KEY_TAB);

		REGISTER_KEY_CODE(GRIZZLY_KEY_BACKSPACE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_INSERT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_DELETE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_RIGHT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_LEFT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_DOWN);
		REGISTER_KEY_CODE(GRIZZLY_KEY_UP);
		REGISTER_KEY_CODE(GRIZZLY_KEY_PAGE_UP);
		REGISTER_KEY_CODE(GRIZZLY_KEY_PAGE_DOWN);
		REGISTER_KEY_CODE(GRIZZLY_KEY_HOME);
		REGISTER_KEY_CODE(GRIZZLY_KEY_END);
		REGISTER_KEY_CODE(GRIZZLY_KEY_CAPS_LOCK);
		REGISTER_KEY_CODE(GRIZZLY_KEY_SCROLL_LOCK);
		REGISTER_KEY_CODE(GRIZZLY_KEY_NUM_LOCK);
		REGISTER_KEY_CODE(GRIZZLY_KEY_PRINT_SCREEN);
		REGISTER_KEY_CODE(GRIZZLY_KEY_PAUSE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F1);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F2);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F3);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F4);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F5);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F6);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F7);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F8);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F9);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F10);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F11);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F12);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F13);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F14);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F15);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F16);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F17);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F18);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F19);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F20);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F21);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F22);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F23);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F24);
		REGISTER_KEY_CODE(GRIZZLY_KEY_F25);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_0);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_1);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_2);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_3);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_4);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_5);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_6);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_7);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_8);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_9);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_DECIMAL);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_DIVIDE);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_MULTIPLY);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_SUBTRACT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_ADD);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_ENTER);
		REGISTER_KEY_CODE(GRIZZLY_KEY_KP_EQUAL);
		REGISTER_KEY_CODE(GRIZZLY_KEY_LEFT_SHIFT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_LEFT_CONTROL);
		REGISTER_KEY_CODE(GRIZZLY_KEY_LEFT_ALT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_LEFT_SUPER);
		REGISTER_KEY_CODE(GRIZZLY_KEY_RIGHT_SHIFT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_RIGHT_CONTROL);
		REGISTER_KEY_CODE(GRIZZLY_KEY_RIGHT_ALT);
		REGISTER_KEY_CODE(GRIZZLY_KEY_RIGHT_SUPER);

		#undef REGISTER_KEY_CODE
	}


	void LuaScriptingInstance::register_enums()
	{
		lua_newtable(m_L);

		lua_pushstring(m_L, "PRESSED");
		lua_pushnumber(m_L, 0);
		lua_settable(m_L, -3);

		lua_pushstring(m_L, "RELEASED");
		lua_pushnumber(m_L, 1);
		lua_settable(m_L, -3);

		lua_pushstring(m_L, "TYPED");
		lua_pushnumber(m_L, 2);
		lua_settable(m_L, -3);

		lua_setglobal(m_L, "InputAction");
	}


}