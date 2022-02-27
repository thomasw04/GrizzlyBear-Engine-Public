#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

namespace GrizzlyBear {

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

		static Ref<ScriptingInstance> create_instance(Scene* scene);
	};
}
