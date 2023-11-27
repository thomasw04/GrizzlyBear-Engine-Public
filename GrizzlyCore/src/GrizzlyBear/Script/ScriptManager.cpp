#include <gbpch.h>

#include "ScriptManager.h"

#include "LuaScript.h"

namespace GrizzlyBear {

	/*

	int LuaFunctions::RegisterSpawnFunction(lua_State* L)
	{
		if (!lua_isfunction(L, -1))
		{
			luaL_argerror(L, -1, "Expected the spawn function");
		}

		lua_pushvalue(L, -1);

		std::get<0>(ScriptManager::m_Scripts.at(ScriptManager::m_CurrentFile)) = luaL_ref(L, LUA_REGISTRYINDEX);

		return 0;
	}

	int LuaFunctions::RegisterUpdateFunction(lua_State* L)
	{
		if (!lua_isfunction(L, -1))
		{
			luaL_argerror(L, -1, "Expected the update function");
		}

		lua_pushvalue(L, -1);

		std::get<1>(ScriptManager::m_Scripts.at(ScriptManager::m_CurrentFile)) = luaL_ref(L, LUA_REGISTRYINDEX);

		return 0;
	}

	int LuaFunctions::RegisterDeleteFunction(lua_State* L)
	{
		if (!lua_isfunction(L, -1))
		{
			luaL_argerror(L, -1, "Expected the delete function");
		}

		lua_pushvalue(L, -1);

		std::get<2>(ScriptManager::m_Scripts.at(ScriptManager::m_CurrentFile)) = luaL_ref(L, LUA_REGISTRYINDEX);

		return 0;
	}

	int LuaFunctions::LOG_TRACE(lua_State* L)
	{
		GRIZZLY_CLIENT_TRACE(GetLog(L));

		return 0;
	}

	int LuaFunctions::LOG_INFO(lua_State* L)
	{
		GRIZZLY_CLIENT_INFO(GetLog(L));

		return 0;
	}

	int LuaFunctions::LOG_WARN(lua_State* L)
	{
		GRIZZLY_CLIENT_WARN(GetLog(L));

		return 0;
	}

	int LuaFunctions::LOG_ERROR(lua_State* L)
	{
		GRIZZLY_CLIENT_ERROR(GetLog(L));

		return 0;
	}

	int LuaFunctions::LOG_FATAL(lua_State* L)
	{
		GRIZZLY_CLIENT_FATAL(GetLog(L));

		return 0;
	}

	Gstring LuaFunctions::GetLog(lua_State* L)
	{
		int args = lua_gettop(L);

		Gstring string = "";

		for (int i = 1; i <= args; i++)
		{
			if (lua_isstring(L, i))
			{
				string += lua_tostring(L, i);
			}
			else
			{
				string += lua_tostring(L, i);
			}
		}

		return string;
	}

	std::unordered_map<Gstring, std::tuple<int, int, int>> ScriptManager::m_Scripts;

	Gstring ScriptManager::m_CurrentFile;

	lua_State* ScriptManager::m_L = nullptr;

	void ScriptManager::Init()
	{
		m_L = luaL_newstate();
		luaL_openlibs(m_L);

		lua_register(m_L, "RegisterSpawnFunction", LuaFunctions::RegisterSpawnFunction);
		lua_register(m_L, "RegisterUpdateFunction", LuaFunctions::RegisterUpdateFunction);
		lua_register(m_L, "RegisterDeleteFunction", LuaFunctions::RegisterDeleteFunction);

		lua_register(m_L, "LOG_TRACE", LuaFunctions::LOG_TRACE);
		lua_register(m_L, "LOG_INFO", LuaFunctions::LOG_INFO);
		lua_register(m_L, "LOG_WARN", LuaFunctions::LOG_WARN);
		lua_register(m_L, "LOG_ERROR", LuaFunctions::LOG_ERROR);
		lua_register(m_L, "LOG_FATAL", LuaFunctions::LOG_FATAL);
	}

	void ScriptManager::Shutdown()
	{
		lua_close(m_L);
	}

	void ScriptManager::Update(Timestep ts)
	{
		for (auto it = m_Scripts.begin(); it != m_Scripts.end(); it++)
		{
			m_CurrentFile = it->first;

			lua_rawgeti(m_L, LUA_REGISTRYINDEX, std::get<1>(it->second));
			lua_pushnumber(m_L, ts.GetMillis());

			lua_pcall(m_L, 1, 0, 0);
		}
	}

	void ScriptManager::LoadScript(const Gstring& filepath)
	{
		m_CurrentFile = filepath;

		m_Scripts.insert({ filepath, {0, 0, 0} });

		luaL_dofile(m_L, filepath.c_str());
	}

	void ScriptManager::TriggerSpawnEvent(const Gstring& filepath)
	{
		m_CurrentFile = filepath;

		lua_rawgeti(m_L, LUA_REGISTRYINDEX, std::get<0>(m_Scripts.at(filepath)));

		lua_pcall(m_L, 0, 0, 0);
	}

	void ScriptManager::TriggerDeleteEvent(const Gstring& filepath)
	{
		m_CurrentFile = filepath;

		lua_rawgeti(m_L, LUA_REGISTRYINDEX, std::get<2>(m_Scripts.at(filepath)));

		lua_pcall(m_L, 0, 0, 0);
	}

	*/
	
	Ref<ScriptingInstance> ScriptingInstance::create_instance(Scene* scene)
	{
		return CreateRef<LuaScriptingInstance>(scene);
	}

}