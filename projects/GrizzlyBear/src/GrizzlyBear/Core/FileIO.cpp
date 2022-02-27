#include <gbpch.h>

#include "FileIO.h"

#include <stack>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace GrizzlyBear {

	//FilePath -----------------------------------------------------------

	FilePath::FilePath(const Gstring& path)
		: m_Path(path), m_IsAbsolute(false), m_IsValid(false), m_IsEmpty(false)
	{
		//Normalize string
		std::replace(m_Path.begin(), m_Path.end(), '\\', '/');

		m_IsAbsolute = Platform::is_absolute_filepath(m_Path);
		m_IsValid = Platform::is_valid_filepath(m_Path);
		m_IsEmpty = m_Path.empty();

		try
		{
			if (!boost::filesystem::exists(m_Path))
			{
				m_IsDirectory = false;
				return;
			}

			m_IsDirectory = boost::filesystem::is_directory(m_Path);
		}
		catch (const boost::filesystem::filesystem_error& e [[maybe_unused]] )
		{
			GRIZZLY_CORE_ASSERT(false, "Something went wrong in the filesystem: {0}", e.what());
			m_IsDirectory = false;
		}
	}

	Gstring FilePath::get_path() const
	{
		return m_Path;
	}

	Gstring FilePath::get_parent() const
	{
		size_t last = m_Path.rfind("/", m_Path.npos);

		return m_Path.substr(0, last + 1);
	}

	Gstring FilePath::get_name() const
	{
		size_t last = m_Path.rfind("/", m_Path.npos);

		return m_Path.substr(last + 1, m_Path.size() - last);
	}

	Gstring FilePath::get_name_woe() const
	{
		size_t last = m_Path.rfind("/", m_Path.npos);
		size_t lastp = m_Path.rfind(".", m_Path.npos);

		return m_Path.substr(last + 1, lastp - last);
	}

	Gstring FilePath::get_extension() const
	{
		size_t last = m_Path.rfind(".", m_Path.npos);

		return boost::to_lower_copy(m_Path.substr(last + 1));
	}

	bool FilePath::is_absolute_path() const
	{
		return m_IsAbsolute;
	}

	bool FilePath::is_valid_path() const
	{
		return m_IsValid;
	}

	bool FilePath::is_empty() const
	{
		return m_IsEmpty;
	}

	bool FilePath::is_directory() const
	{
		return m_IsDirectory;
	}

	void FilePath::add(const Gstring& next)
	{
		m_Path += next;

		m_IsValid = Platform::is_valid_filepath(m_Path);
		
		try
		{
			if (!boost::filesystem::exists(m_Path)) m_IsDirectory = false;
			m_IsDirectory = boost::filesystem::is_directory(m_Path);
		}
		catch (const boost::filesystem::filesystem_error& e [[maybe_unused]] )
		{
			GRIZZLY_CORE_ASSERT(false, "Something went wrong in the filesystem: {0}", e.what());
			m_IsDirectory = false;
		}
	}

	void FilePath::back(int steps)
	{
		for (int i = 0; i < steps; i++)
		{
			size_t index = m_Path.rfind("/", m_Path.size() - 2);

			if (index == m_Path.npos)
			{
				m_Path = "";
				return;
			}

			m_Path = m_Path.substr(0, index + 1);
		}
	}

	FilePath& FilePath::operator+=(const FilePath& rhs)
	{
		m_Path += rhs.m_Path;

		return *this;
	}

	FilePath FilePath::operator+(const FilePath& rhs)
	{
		return FilePath(this->m_Path + rhs.m_Path);
	}

	//File --------------------------------------------------------------------------------

	File::File(File& file)
		: m_Path(file.m_Path), out(file.m_Path.get_path()), in(file.m_Path.get_path())
	{
		out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	}

	File::~File()
	{
		in.close();
		out.close();
	}

	bool File::copy(const FilePath& destination, bool overwrite)
	{
		try {

			if (!destination.is_valid_path()) return false;
			if (!FileIO::exists(m_Path.get_path())) return false;

			if (is_directory())
			{
				boost::filesystem::copy_directory(m_Path.get_path(), destination.get_path());
				return true;
			}

			if (overwrite)
			{
				boost::filesystem::copy_file(m_Path.get_path(), destination.get_path(), boost::filesystem::copy_option::overwrite_if_exists);
				return true;
			}
			else
			{
				boost::filesystem::copy_file(m_Path.get_path(), destination.get_path(), boost::filesystem::copy_option::fail_if_exists);
				return true;
			}

		}
		catch (const boost::filesystem::filesystem_error& e)
		{
			GRIZZLY_CORE_ASSERT(false, "Something went wrong in the filesystem: {0}", e.what());
			return false;
		}

	}

	bool File::move(const FilePath& newPath)
	{
		if (newPath.is_valid_path())
		{
			try
			{
				if (!FileIO::exists(m_Path.get_path())) return false;
				boost::filesystem::rename(m_Path.get_path(), newPath.get_path());
				return true;

			}
			catch (const boost::filesystem::filesystem_error& e)
			{
				GRIZZLY_CORE_ASSERT(false, "Something went wrong in the filesystem: {0}", e.what());

				return false;
			}
		}
		else
		{
			GRIZZLY_CORE_ASSERT(false, "Invalid path");
			return false;
		}
	}

	bool File::rename(const Gstring& newName)
	{
		move(m_Path.get_parent() + newName);
		return true;
	}

	bool File::is_directory() const
	{
		return m_Path.is_directory();
	}

	bool File::destroy()
	{
		try
		{
			if (!FileIO::exists(m_Path.get_path())) return false;

			in.close();
			out.close();

			boost::filesystem::remove(m_Path.get_path());
			return true;
		}
		catch (const boost::filesystem::filesystem_error& e)
		{
			GRIZZLY_CORE_ASSERT(false, "Something went wrong in the filesystem: {0}", e.what());
			return false;
		}
	}

	void File::set_ofstream_mode(std::ios_base::openmode mode)
	{
		out = std::ofstream(m_Path.get_path(), mode);
	}

	void File::set_ifstream_mode(std::ios_base::openmode mode)
	{
		in = std::ifstream(m_Path.get_path(), mode);
	}

	std::ofstream& File::get_ofstream()
	{
		return out;
	}

	std::ifstream& File::get_ifstream()
	{
		return in;
	}

	FilePath File::get_path()
	{
		return m_Path;
	}

	//FileIO ----------------------------------------------------------------------------------------------

	File FileIO::open(const FilePath& path)
	{
		if (!path.is_valid_path())
		{
			GRIZZLY_CORE_ASSERT(false, "Invalid file path");

			return File(FilePath(""));
		}

		if (!exists(path))
		{
			boost::filesystem::create_directories(path.get_parent());
			boost::filesystem::ofstream file(path.get_path());
			file.close();

			return File(path);
		}
		else
		{
			return File(path);
		}
	}

	Gbuffer FileIO::get_file_content(const FilePath& path)
	{
		if (path.is_valid_path() && FileIO::exists(path))
		{
			File file = FileIO::open(path);

			if (file.is_directory())
			{
				return { Gstring(), Gbuffer::State::FAILED };
			}

			try
			{
				file.set_ifstream_mode(std::ios::in | std::ios::binary);

				std::ifstream& stream = file.get_ifstream();

				std::stringstream sstream;
				sstream << stream.rdbuf();

				return { sstream.str(), Gbuffer::State::GOOD };
			}
			catch (std::ifstream::failure& e)
			{
				(void)e;
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: FileIO, Message: input stream failure while reading from: {0}", path.get_path().c_str());
				return { Gstring(), Gbuffer::State::INVALID };
			}
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: FileIO, Message: invalid path to read file: {0}", path.get_path().c_str());
		return { Gstring(), Gbuffer::State::FAILED };
	}

	bool FileIO::exists(const FilePath& file)
	{
		if (!file.is_valid_path()) return false;

		try
		{
			return boost::filesystem::exists(file.get_path());
		}
		catch (const boost::filesystem::filesystem_error& e)
		{
			GRIZZLY_CORE_ASSERT(false, "Something went wrong in the filesystem: {0}", e.what());
			return false;
		}
	}

	bool FileIO::set_current_path(const FilePath& path)
	{
		if (path.is_valid_path())
		{
			boost::filesystem::current_path(path.get_path());
			return true;
		}

		return false;
	}

	FilePath FileIO::get_current_path()
	{
		return FilePath(boost::filesystem::current_path().string());
	}

	FilePath FileIO::get_program_directory()
	{
		return FilePath(Platform::get_executable_path()).get_parent();
	}

	std::vector<FilePath> FileIO::get_files_in_dir(const File& dir, bool recursive)
	{
		std::stack<Gstring> subDirs;

		if (dir.is_directory())
		{
			subDirs.push(dir.m_Path.get_path());

			std::vector<FilePath> paths;

			while (!subDirs.empty())
			{
				//Get the next folder and pop it from the stack
				Gstring nextPath = subDirs.top();
				subDirs.pop();

				boost::filesystem::directory_iterator end;

				for (boost::filesystem::directory_iterator it(nextPath); it != end; it++)
				{
					if (boost::filesystem::is_regular_file(it->status()))
					{
						paths.push_back(it->path().string());	
						continue;
					}
					else if (boost::filesystem::is_directory(it->path()) && recursive)
					{
						//Push new directory on the stack to process in the further iterations, when recursive is enabled!
						subDirs.push(it->path().string());
					}
				}
			}

			return paths;
		}

		return std::vector<FilePath>();
	}

	std::vector<RelativePath> FileIO::get_files_in_dir_rel(const File& dir, bool recursive)
	{
		std::stack<Gstring> subDirs;

		if (dir.is_directory())
		{
			subDirs.push(dir.m_Path.get_path());

			std::vector<RelativePath> paths;

			while (!subDirs.empty())
			{
				//Get the next folder and pop it from the stack
				Gstring nextPath = subDirs.top();
				subDirs.pop();

				boost::filesystem::directory_iterator end;

				for (boost::filesystem::directory_iterator it(nextPath); it != end; it++)
				{
					if (boost::filesystem::is_regular_file(it->status()))
					{
						paths.push_back({ it->path().string().substr(dir.m_Path.get_path().size()), dir.m_Path });

						continue;
					}
					else if (boost::filesystem::is_directory(it->path()) && recursive)
					{
						//Push new directory on the stack to process in the further iterations, when recursive is enabled!
						subDirs.push(it->path().string());
					}
				}
			}

			return paths;
		}

		return std::vector<RelativePath>();
	}

	std::vector<RelativePath> FileIO::get_things_in_dir_rel(const File& dir)
	{
		std::vector<RelativePath> paths;

		boost::filesystem::directory_iterator end;

		for (boost::filesystem::directory_iterator it(dir.m_Path.get_path()); it != end; it++)
		{
			paths.push_back({it->path().string().substr(dir.m_Path.get_path().size()), dir.m_Path });
		}

		return paths;
	}

	std::vector<FilePath> FileIO::get_things_in_dir(const File& dir)
	{
		std::vector<FilePath> paths;

		boost::filesystem::directory_iterator end;

		for (boost::filesystem::directory_iterator it(dir.m_Path.get_path()); it != end; it++)
		{
			paths.push_back(it->path().string());
		}

		return paths;
	}

	FilePath FileIO::ask_project_file()
	{
		return FilePath("");
	}

}