#include <gbpch.h>

#include "DevelopmentFileFetcher.h"

#include "GrizzlyBear/Core/FileIO.h"

namespace GrizzlyBear {

	DevelopmentFileFetcher::DevelopmentFileFetcher(const Gstring& projectDir, Ref<GProject> prj)
		: FileFetcher(projectDir, prj)
	{
		const GProject::GSection& section = FileFetcher::m_ProjectFile->get_section("Project");

		m_Config["InBuild"].add_pair("AssetDir", section.get_value("AssetDirectory"));
		m_Config["InBuild"].add_pair("CacheDir", section.get_value("CacheDirectory"));
		m_Config["InBuild"].add_pair("LogDir", section.get_value("LogDirectory"));
		m_Config["InBuild"].add_pair("Name", section.get_value("Name"));
	}

	Gbuffer DevelopmentFileFetcher::get_asset_file_raw(const Gstring& path)
	{
		Gstring assetDir = get_inbuild_config("AssetDir");

		FilePath fpath("");

		if (path.substr(0, 7) == "engine/")
		{
			fpath = FilePath(FileIO::get_program_directory().get_path() + path.substr(7));
		}
		else if (path.substr(0, assetDir.size()) == assetDir)
		{
			fpath = FilePath(FileFetcher::m_Target + path);
		}
		else
		{
			fpath = FilePath(FileFetcher::m_Target + assetDir + path);
		}

		if (fpath.is_valid_path() && FileIO::exists(fpath))
		{
			File asset = FileIO::open(fpath);

			if (asset.is_directory())
			{
				return { Gstring(), Gbuffer::State::FAILED };
			}

			try 
			{
				asset.set_ifstream_mode(std::ios::in | std::ios::binary);

				std::ifstream& stream = asset.get_ifstream();

				std::stringstream sstream;
				sstream << stream.rdbuf();

				return { sstream.str(), Gbuffer::State::GOOD };
			}
			catch (std::ifstream::failure& e)
			{
				(void)e;
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: input stream failure while reading from: {0}", path.c_str());
				return { Gstring(), Gbuffer::State::INVALID };
			}
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: invalid path to read file: {0}", path.c_str());
		return { Gstring(), Gbuffer::State::FAILED };
	}

	Gstring DevelopmentFileFetcher::get_full_asset_path(const Gstring& path)
	{
		Gstring assetDir = get_inbuild_config("AssetDir");

		FilePath fpath("");

		if (path.substr(0, assetDir.size()) == assetDir)
		{
			fpath = FilePath(FileFetcher::m_Target + path);
		}
		else
		{
			fpath = FilePath(FileFetcher::m_Target + assetDir + path);
		}

		return fpath.get_path();
	}

	bool DevelopmentFileFetcher::save_asset_file(const Gstring& path, const Gbuffer& buffer)
	{
		Gstring assetDir = get_inbuild_config("AssetDir");
		Gstring cacheDir = get_inbuild_config("CacheDir");

		FilePath fpath("");

		if (path.substr(0, assetDir.size()) == assetDir)
		{
			fpath = FilePath(FileFetcher::m_Target + path);
		}
		else
		{
			fpath = FilePath(FileFetcher::m_Target + assetDir + path);
		}

		if (fpath.is_valid_path())
		{
			if (FileIO::exists(path))
			{
				Gbuffer fallback = get_asset_file_raw(path);

				if (!fallback.is_good())
				{
					//Return if no fallback could be saved!
					return false;
				}
			}

			File asset = FileIO::open(fpath);

			if (asset.is_directory()) return false;

			try
			{
				asset.set_ofstream_mode(std::ios::out | std::ios::trunc | std::ios::binary);
				std::ofstream& stream = asset.get_ofstream();
				stream << buffer.m_Buffer;
				return true;
			}
			catch (std::ofstream::failure& e)
			{
				(void)e;
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: output stream failure while writing to: {0}", path.c_str());

				FilePath name(path);
				FilePath recovery = FilePath(FileFetcher::m_Target + cacheDir + name.get_name());

				if (recovery.is_valid_path())
				{
					File asset = FileIO::open(recovery);

					try
					{
						asset.set_ofstream_mode(std::ios::out | std::ios::trunc | std::ios::binary);
						std::ofstream& stream = asset.get_ofstream();
						stream << buffer.m_Buffer;

						GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Module: Environment, Message: recovery file for {0} can be found in cache directory", path.c_str());
						return false;
					}
					catch (std::ofstream::failure& e)
					{
						(void)e;
						GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Fatal, "Module: Environment, Message: output stream failure for recovery of: {0}", name.get_name().c_str());
						return false;
					}
				}

				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Fatal, "Module: Environment, Message: invalid cache directory path while saving recovery for: {0}", name.get_name().c_str());
				return false;
			}
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: invalid path to save file: {0}", path.c_str());
		return false;
	}

	std::vector<RelativePath> DevelopmentFileFetcher::get_files_in_dir(const Gstring& path, bool recursive)
	{
		FilePath fpath(m_Target + path);

		if (fpath.is_valid_path())
		{
			File file = FileIO::open(fpath);

			return FileIO::get_files_in_dir_rel(file, recursive);
		}

		return std::vector<RelativePath>();
	}

	std::vector<RelativePath> DevelopmentFileFetcher::get_things_in_dir(const Gstring& path)
	{
		FilePath fpath(m_Target + path);

		if (fpath.is_valid_path())
		{
			File file = FileIO::open(fpath);

			return FileIO::get_things_in_dir_rel(file);
		}

		return std::vector<RelativePath>();
	}

	void DevelopmentFileFetcher::update_configs()
	{
		GProject::GSection inbuild = m_Config["InBuild"];

		m_Config.clear();

		GProjectParser::parse_file(*m_ProjectFile);

		const GProject::GSection& section = m_ProjectFile->get_section("Project");

		File configDir = FileIO::open(FilePath(section.get_value("ConfigDirectory")));

		std::vector<FilePath> files = FileIO::get_files_in_dir(configDir, true);

		if (!files.empty())
		{
			for (FilePath& path : files)
			{
				GProjectParser::parse_file(m_Config, path);
			}
		}

		m_Config["InBuild"] = inbuild;
	}
}