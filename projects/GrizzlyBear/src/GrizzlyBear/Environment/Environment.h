#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "FileFetcher.h"

#include "GProjectParser.h"

namespace GrizzlyBear {

	class Environment
	{
	public:

		enum class EnvironmentType
		{
			None = 0, Development, Production
		};

	public:
		/*
		* Init the Environment class target is the path to a gprj file
		*/
		static ModuleState init(const Gstring& target);
		static EnvironmentType get_environment_type();
		static Ref<FileFetcher> get_fetcher();

		static bool has_environment() { return s_Has; }

	private:

		static Ref<FileFetcher> s_Fetcher;
		static EnvironmentType s_Type;
		static Ref<GProject> s_ParsedFile;
		static bool s_Has;
	};
}
