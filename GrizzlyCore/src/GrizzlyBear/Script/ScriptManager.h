#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "Native/Entity.h"

namespace GrizzlyBear {

	/*

	struct LuaFunctions
	{
		static int RegisterSpawnFunction(lua_State* L);
		static int RegisterUpdateFunction(lua_State* L);
		static int RegisterDeleteFunction(lua_State* L);

		static int LOG_TRACE(lua_State* L);
		static int LOG_INFO(lua_State* L);
		static int LOG_WARN(lua_State* L);
		static int LOG_ERROR(lua_State* L);
		static int LOG_FATAL(lua_State* L);

		static Gstring GetLog(lua_State* L);
	};

	class ScriptManager
	{
		friend LuaFunctions;

	public:
		static void Init();
		static void Shutdown();

		static void Update(Timestep ts);

		static void LoadScript(const Gstring& filepath);

		static void TriggerSpawnEvent(const Gstring& filepath);
		static void TriggerDeleteEvent(const Gstring& filepath);

	private:
		static std::unordered_map<Gstring, std::tuple<int, int, int>> m_Scripts;

		static Gstring m_CurrentFile;

		static lua_State* m_L;
	};

	*/

	class Scene;

	class ScriptingInstance
	{
	public:
		virtual void set_framework(const Gstring& filepath) = 0;

		virtual void load_framework() = 0;
		virtual void update_framework(Timestep ts) = 0;
		virtual void shutdown_framework() = 0;

		virtual void load_script(const Gstring& filepath, uint32_t entityID) = 0;
		virtual void delete_script(uint32_t entityID) = 0;

		virtual void on_spawn(uint32_t entityID) = 0;
		virtual void on_update(Timestep ts, uint32_t entityID) = 0;
		virtual void on_delete(uint32_t entityID) = 0;

		std::unordered_map<size_t, Ref<Entity2D>> m_Native;

		static Ref<ScriptingInstance> create_instance(Scene* scene);
	};
}
