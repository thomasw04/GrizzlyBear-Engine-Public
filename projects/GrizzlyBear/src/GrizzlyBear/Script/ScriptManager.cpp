#include <gbpch.h>

#include "ScriptManager.h"

#include "LuaScript.h"

namespace GrizzlyBear {
	
	Ref<ScriptingInstance> ScriptingInstance::create_instance(Scene* scene)
	{
		return CreateRef<LuaScriptingInstance>(scene);
	}

}