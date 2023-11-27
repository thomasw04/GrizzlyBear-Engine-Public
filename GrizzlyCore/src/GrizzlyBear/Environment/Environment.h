#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "ChunkFetcher.h"

#include "Gcff.h"

namespace GrizzlyBear {

	enum class EngineFeature
	{
		Precompiled_Shaders = 1
	};

	class Environment
	{
	public:
		enum class FetcherType
		{
			Plain = 0
		};

	public:
		/*
		* Init the Environment class target is the path to a gprj file
		*/
		static ModuleInfo init(const FilePath& projectFile);

		static Ref<ChunkFetcher> get_fetcher();

		static int get_feature(EngineFeature feature);
		static void set_feature(EngineFeature feature, int value);

		static Gstring get_project_dir();

		static Gstring get_cfg(const Gstring& section, const Gstring& key);
		static Gstring get_prj(const Gstring& section, const Gstring& key);

		static Gmapping& get_cfg_section(const Gstring& section);
		static Gmapping& get_prj_section(const Gstring& section);

		static void reload_custom_configs();
	private:

		static Ref<ChunkFetcher> s_Fetcher;

		static Ref<Gcff> s_Project;
		static Gstring s_WorkingDir;
		static std::unordered_map<Gstring, Gmapping> s_CustomConfig;
		static std::unordered_map<int, int> s_Features;
	};
}
