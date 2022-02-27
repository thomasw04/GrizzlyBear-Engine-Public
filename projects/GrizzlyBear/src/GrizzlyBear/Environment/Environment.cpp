#include <gbpch.h>

#include "GrizzlyBear/Core/FileIO.h"

#include "Environment.h"

#include "DevelopmentFileFetcher.h"
#include "ProductionFileFetcher.h"

namespace GrizzlyBear {

	Ref<FileFetcher> Environment::s_Fetcher = nullptr;
	Environment::EnvironmentType Environment::s_Type = Environment::EnvironmentType::None;
	Ref<GProject> Environment::s_ParsedFile = nullptr;
	bool Environment::s_Has = false;

	ModuleState Environment::init(const Gstring& target)
	{
		FilePath path = target;

		//If Valid environment
		if (FileIO::exists(path))
		{
			//Check in which environment we are in
			s_ParsedFile = CreateRef<GProject>(GProjectParser::parse_file(path));

			GProject::GSection section = s_ParsedFile->get_section("Project");

			Gstring value = section.get_value("EnvironmentType");

			if (value == "Development")
			{
				s_Type = EnvironmentType::Development;

				s_Fetcher = CreateRef<DevelopmentFileFetcher>(path.get_parent(), s_ParsedFile);
				s_Has = true;
				return ModuleState::GOOD;
			}
			else if (value == "Production")
			{
				s_Type = EnvironmentType::Production;

				s_Fetcher = CreateRef<ProductionFileFetcher>(path.get_parent(), s_ParsedFile);
				s_Has = true;
				return ModuleState::GOOD;
			}
			else
			{
				return ModuleState::ERROR_FATAL;
			}
		}
		else
		{
			//TODO Virtual Environment
		}

		return ModuleState::ERROR_FATAL;
	}

	Environment::EnvironmentType Environment::get_environment_type()
	{
		return s_Type;
	}

	Ref<FileFetcher> Environment::get_fetcher()
	{
		return s_Fetcher;
	}

}