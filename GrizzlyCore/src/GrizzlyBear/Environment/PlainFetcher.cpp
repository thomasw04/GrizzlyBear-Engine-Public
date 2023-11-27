#include <gbpch.h>

#include "Environment.h"
#include "PlainFetcher.h"

#include "GrizzlyBear/Core/FileIO.h"

#include <sodium.h>

namespace GrizzlyBear {

	PlainFetcher::PlainFetcher(const Gstring& target)
		: ChunkFetcher(target)
	{
	}

	bool PlainFetcher::is_locked()
	{
		Gstring value = Environment::get_prj(m_Target, "locked");
		std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
		{
			return std::tolower(c);
		});

		if (value == "true")
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool PlainFetcher::unlock(const Gstring& key, const Gstring& nonce)
	{
		/*auto paths = find_assets("", true);

		size_t size = 0;
		unsigned char* buf = new unsigned char[size];

		//This will take forever
		for (RelativePath p : paths)
		{
			Gbuffer data = get_asset_data(p.m_Relative);

			if (size < data.m_Buffer.size() - crypto_secretbox_MACBYTES)
			{
				delete[] buf;
				size = data.m_Buffer.size();
				buf = new unsigned char[size];
			}
			int status = crypto_secretbox_open_easy(buf, (const unsigned char*)data.m_Buffer.c_str(), data.m_Buffer.size() + crypto_secretbox_MACBYTES, (const unsigned char*)nonce.c_str(), (const unsigned char*)key.c_str());
			
			if (status == -1)
			{
				GRIZZLY_CORE_LOG(LogLevel::Warn, "Module: Environment, Message: Failed to unlock file {0}.", p.m_FullPath.get_name().c_str());
				delete[] buf;
				return false;
			}
			else
			{
				Gbuffer newData = { std::string(reinterpret_cast<char const*>(buf), data.m_Buffer.size()), Gbuffer::State::GOOD };
				save_asset_data(p.m_Relative, newData);
			}
		}
#
		delete[] buf;*/
		return true;
	}

	void PlainFetcher::lock(const Gstring& key, const Gstring& nonce)
	{
		/*auto paths = find_assets("", true);

		size_t size = crypto_secretbox_MACBYTES;
		unsigned char* buf = new unsigned char[size];

		//This will take forever
		for (RelativePath p : paths)
		{
			Gbuffer data = get_asset_data(p.m_Relative);

			if (size < data.m_Buffer.size() + crypto_secretbox_MACBYTES)
			{
				delete[] buf;
				size = data.m_Buffer.size() + crypto_secretbox_MACBYTES;
				buf = new unsigned char[size];
			}

			crypto_secretbox_easy(buf, (const unsigned char*)data.m_Buffer.c_str(), data.m_Buffer.size(), (const unsigned char*)nonce.c_str(), (const unsigned char*)key.c_str());

			Gbuffer newData = { std::string(reinterpret_cast<char const*>(buf), data.m_Buffer.size() + crypto_secretbox_MACBYTES), Gbuffer::State::GOOD };
			save_asset_data(p.m_Relative, newData);
		}

		delete[] buf;*/
	}

	Gbuffer PlainFetcher::get_asset_data(const Gstring& relPath)
	{
		Gstring assetDir = Environment::get_prj("Project", "AssetDirectory");

		FilePath fpath("");

		if (relPath.substr(0, 7) == "engine/")
		{
			fpath = FilePath(FileIO::get_program_directory() + relPath.substr(6));
		}
		else if (relPath.substr(0, assetDir.size()) == assetDir)
		{
			fpath = FilePath(ChunkFetcher::m_Target + relPath);
		}
		else
		{
			fpath = FilePath(ChunkFetcher::m_Target + assetDir + relPath);
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
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: input stream failure while reading from: {0}", relPath.c_str());
				return { Gstring(), Gbuffer::State::INVALID };
			}
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: invalid path to read file: {0}", relPath.c_str());
		return { Gstring(), Gbuffer::State::FAILED };
	}

	Gstring PlainFetcher::get_asset_path(const Gstring& path)
	{
		Gstring assetDir = Environment::get_prj("Project", "AssetDirectory");

		FilePath fpath("");

		if (path.substr(0, assetDir.size()) == assetDir)
		{
			fpath = FilePath(ChunkFetcher::m_Target + path);
		}
		else
		{
			fpath = FilePath(ChunkFetcher::m_Target + assetDir + path);
		}

		return fpath.str();
	}

	bool PlainFetcher::save_asset_data(const Gstring& relPath, const Gbuffer& data)
	{
		Gstring assetDir = Environment::get_prj("Project", "AssetDirectory");
		Gstring cacheDir = Environment::get_prj("Project", "CacheDirectory");

		FilePath fpath("");

		if (relPath.substr(0, assetDir.size()) == assetDir)
		{
			fpath = FilePath(ChunkFetcher::m_Target + relPath);
		}
		else
		{
			fpath = FilePath(ChunkFetcher::m_Target + assetDir + relPath);
		}

		if (fpath.is_valid_path())
		{
			if (FileIO::exists(relPath))
			{
				Gbuffer fallback = get_asset_data(relPath);

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
				stream << data.m_Buffer;
				return true;
			}
			catch (std::ofstream::failure& e)
			{
				(void)e;
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: output stream failure while writing to: {0}", relPath.c_str());

				FilePath name = relPath;
				FilePath recovery = FilePath(ChunkFetcher::m_Target + cacheDir + name.get_name());

				if (recovery.is_valid_path())
				{
					File asset = FileIO::open(recovery);

					try
					{
						asset.set_ofstream_mode(std::ios::out | std::ios::trunc | std::ios::binary);
						std::ofstream& stream = asset.get_ofstream();
						stream << data.m_Buffer;

						GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "Module: Environment, Message: recovery file for {0} can be found in cache directory", relPath.c_str());
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

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: Environment, Message: invalid path to save file: {0}", relPath.c_str());
		return false;
	}

	std::vector<RelativePath> PlainFetcher::find_assets(const Gstring& relPath, bool recursive)
	{
		FilePath fpath = m_Target + relPath;

		if (fpath.is_valid_path())
		{
			return FileIO::get_files_in_dir_rel(fpath, recursive);
		}

		return std::vector<RelativePath>();
	}

	std::vector<RelativePath> PlainFetcher::find_entries(const Gstring& relPath)
	{
		FilePath fpath = m_Target + relPath;

		if (fpath.is_valid_path())
		{
			return FileIO::get_things_in_dir_rel(fpath);
		}

		return std::vector<RelativePath>();
	}

	std::unordered_map<Gstring, Gmapping> PlainFetcher::update_configs()
	{
		Gmapping inbuild = Environment::get_cfg_section("InBuild");

		std::unordered_map<Gstring, Gmapping> returnValue;

		FilePath configDir = FilePath(Environment::get_prj("Project", "ConfigDirectory"));

		std::vector<FilePath> files = FileIO::get_files_in_dir(configDir, true);

		if (!files.empty())
		{
			for (FilePath& path : files)
			{
				Ref<Gcff> temp = Gcff::open(path);
				temp->copy(returnValue);
			}
		}

		returnValue["InBuild"] = inbuild;
		return returnValue;
	}
}