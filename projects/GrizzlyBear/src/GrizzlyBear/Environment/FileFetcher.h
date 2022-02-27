#pragma once

#include <unordered_map>
#include <vector>

#include "GrizzlyBear/Core/FileIO.h"

#include "GProjectParser.h"

namespace GrizzlyBear {

	class FileFetcher
	{
	public:

		FileFetcher(const Gstring& target, Ref<GProject> prj) : m_Target(target), m_ProjectFile(prj) {}

		Gstring get_project_dir() 
		{
			return m_Target;
		}

		/*
		* Return the value of a specific config key. e.g. Key(Resolution) = Value(1280x720)
		*/
		Gstring get_config_value(const Gstring& section, const Gstring& key)
		{
			return m_Config[section].get_value(key);
		}

		/*
		* Returns the raw string of an asset file. E.g. a shader source or the ascii representation of a PNG
		*/
		virtual Gbuffer get_asset_file_raw(const Gstring& path) = 0;

		virtual Gstring get_full_asset_path(const Gstring& path) = 0;

		/*
		* Saves a asset to the specified identifier
		*/
		virtual bool save_asset_file(const Gstring& path, const Gbuffer& buffer) = 0;

		virtual std::vector<RelativePath> get_files_in_dir(const Gstring& path, bool recursive) = 0;
		virtual std::vector<RelativePath> get_things_in_dir(const Gstring& path) = 0;

		virtual void update_configs() = 0;

	protected:

		/*
		* Returns values from the inbuild config
		*/
		Gstring get_inbuild_config(const Gstring& key)
		{
			return m_Config["InBuild"].get_value(key);
		}

	protected:

		Gstring m_Target;

		Ref<GProject> m_ProjectFile;

		std::unordered_map<Gstring, GProject::GSection> m_Config;
	};
}
