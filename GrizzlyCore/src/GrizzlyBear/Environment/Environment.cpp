#include <gbpch.h>

#include "GrizzlyBear/Core/FileIO.h"

#include "Environment.h"

#include "PlainFetcher.h"

namespace GrizzlyBear {

	Ref<ChunkFetcher> Environment::s_Fetcher = nullptr;
	Ref<Gcff> Environment::s_Project = nullptr;
	Gstring Environment::s_WorkingDir;
	std::unordered_map<Gstring, Gmapping> Environment::s_CustomConfig;
	std::unordered_map<int, int> Environment::s_Features;

	ModuleInfo Environment::init(const FilePath& projectFile)
	{
		s_Project = Gcff::open(projectFile.str());
		s_WorkingDir = projectFile.get_parent();
		s_Fetcher = std::make_shared<PlainFetcher>(s_WorkingDir);
		return ModuleInfo({ ModuleState::GOOD, "" });
	}

	Ref<ChunkFetcher> Environment::get_fetcher()
	{
		return s_Fetcher;
	}

	int Environment::get_feature(EngineFeature feature)
	{
		return s_Features[(int)feature];
	}

	void Environment::set_feature(EngineFeature feature, int value)
	{
		s_Features[(int)feature] = value;
	}

	Gstring Environment::get_project_dir()
	{
		return s_WorkingDir;
	}

	Gstring Environment::get_cfg(const Gstring& section, const Gstring& key)
	{
		return s_CustomConfig[section][key];
	}

	Gstring Environment::get_prj(const Gstring& section, const Gstring& key)
	{
		return (*s_Project)[section][key];
	}

	Gmapping& Environment::get_cfg_section(const Gstring& section)
	{
		return (*s_Project)[section];
	}

	Gmapping& Environment::get_prj_section(const Gstring& section)
	{
		return (*s_Project)[section];
	}

	void Environment::reload_custom_configs()
	{
		s_CustomConfig = s_Fetcher->update_configs();
	}

}