#include <gbpch.h>

#include "FileIO.h"

#include <stack>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace GrizzlyBear {

	//FilePath -----------------------------------------------------------

	FilePath::FilePath(const Gstring& path)
		: m_Dirty(true), m_Path(path), m_IsAbsolute(false), m_IsValid(false), m_IsDirectory(false)
	{
		//Normalize string
		std::replace(m_Path.begin(), m_Path.end(), '\\', '/');
		if (!m_Path.empty())
		{
			if (m_Path[m_Path.size() - 1] == '/') m_Path.erase(m_Path.end() - 1);
		}
	}

	FilePath::FilePath(const FilePath& other)
		: m_Dirty(other.m_Dirty), m_Path(other.m_Path), m_IsAbsolute(other.m_IsAbsolute), m_IsValid(other.m_IsValid), m_IsDirectory(other.m_IsDirectory) {}

	FilePath::FilePath()
		: m_Dirty(false), m_Path(""), m_IsAbsolute(false), m_IsValid(false), m_IsDirectory(false) {}

	Gstring FilePath::str() const
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
		check_file();

		return m_IsAbsolute;
	}

	bool FilePath::is_valid_path() const
	{
		check_file();

		return m_IsValid;
	}

	bool FilePath::is_empty() const
	{
		return m_Path.empty();
	}

	bool FilePath::is_directory() const
	{
		check_file();

		return m_IsDirectory;
	}

	void FilePath::add(const Gstring& next)
	{
		Gstring n = next;
		std::replace(n.begin(), n.end(), '\\', '/');
		if (n[n.size() - 1] == '/') n.erase(n.end() - 1);

		if (next[0] == '/')
		{
			m_Path.append(n);
		}
		else
		{
			m_Path.append("/").append(n);
		}

		m_Dirty = true;
	}

	void FilePath::back(int steps)
	{
		if (steps == 0) return;
		m_Dirty = true;

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

	FilePath& FilePath::operator=(const Gstring& other)
	{
		m_Path = other;
		std::replace(m_Path.begin(), m_Path.end(), '\\', '/');
		if (m_Path[m_Path.size() - 1] == '/') m_Path.erase(m_Path.end() - 1);
		m_Dirty = true;
		return *this;
	}

	FilePath::operator Gstring() const
	{
		return m_Path;
	}

	FilePath& FilePath::operator+=(const Gstring& rhs)
	{
		m_Path += rhs;
		m_Dirty = true;
		return *this;
	}

	FilePath FilePath::operator+(const Gstring& rhs) const
	{
		return FilePath(this->m_Path + rhs);
	}

	const Gstring FilePath::operator[](int index) const
	{
		return Gstring();
	}

	int FilePath::get_depth() const
	{
		size_t index = 0;
		int counter = 0;
		while (index != m_Path.npos)
		{
			counter++;
			index = m_Path.find('/', index);
			if (index == m_Path.npos) break;
			index++;
		}

		return counter;
	}

	void FilePath::check_file() const
	{
		if (m_Dirty && !m_Path.empty())
		{
			m_IsValid = Platform::is_valid_filepath(m_Path);
			
			if (m_IsValid)
			{
				m_IsAbsolute = Platform::is_absolute_filepath(m_Path);

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

			m_Dirty = false;
		}
	}

	//File --------------------------------------------------------------------------------

	File::File(File& file)
		: m_Path(file.m_Path), out(file.m_Path.str()), in(file.m_Path.str())
	{
		out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	}

	File::~File()
	{
		in.close();
		out.close();
	}

	bool File::copy(const FilePath& destination, bool overwrite) const
	{
		try {

			if (!destination.is_valid_path()) return false;
			if (!FileIO::exists(m_Path)) return false;

			if (is_directory())
			{
				boost::filesystem::copy_directory(m_Path.str(), destination.str());
				return true;
			}

			if (overwrite)
			{
				boost::filesystem::copy_file(m_Path.str(), destination.str(), boost::filesystem::copy_option::overwrite_if_exists);
				return true;
			}
			else
			{
				boost::filesystem::copy_file(m_Path.str(), destination.str(), boost::filesystem::copy_option::fail_if_exists);
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
				if (!FileIO::exists(m_Path)) return false;
				boost::filesystem::rename(m_Path.str(), newPath.str());
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
			if (!FileIO::exists(m_Path)) return false;

			in.close();
			out.close();

			boost::filesystem::remove(m_Path.str());
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
		out = std::ofstream(m_Path.str(), mode);
	}

	void File::set_ifstream_mode(std::ios_base::openmode mode)
	{
		in = std::ifstream(m_Path.str(), mode);
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
			std::ofstream file(path.str());
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
				GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: FileIO, Message: input stream failure while reading from: {0}", path.str().c_str());
				return { Gstring(), Gbuffer::State::INVALID };
			}
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Module: FileIO, Message: invalid path to read file: {0}", path.str().c_str());
		return { Gstring(), Gbuffer::State::FAILED };
	}

	bool FileIO::exists(const FilePath& file)
	{
		if (!file.is_valid_path()) return false;

		try
		{
			return boost::filesystem::exists(file.str());
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
			boost::filesystem::current_path(path.str());
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

	std::vector<FilePath> FileIO::get_files_in_dir(const FilePath& in, bool recursive)
	{
		std::stack<Gstring> subDirs;

		FilePath dir = in + std::string("/");

		if (dir.is_directory())
		{
			subDirs.push(dir.str());

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

	std::vector<RelativePath> FileIO::get_files_in_dir_rel(const FilePath& in, bool recursive)
	{
		std::stack<Gstring> subDirs;
		FilePath dir = in + std::string("/");

		if (dir.is_directory())
		{
			subDirs.push(dir.str());

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
						paths.push_back({ it->path().string().substr(dir.str().size()), dir });

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

	std::vector<RelativePath> FileIO::get_things_in_dir_rel(const FilePath& in)
	{
		std::vector<RelativePath> paths;

		FilePath dir = in + std::string("/");

		boost::filesystem::directory_iterator end;

		for (boost::filesystem::directory_iterator it(dir.str()); it != end; it++)
		{
			paths.push_back({it->path().string().substr(dir.str().size()), dir });
		}

		return paths;
	}

	std::vector<FilePath> FileIO::get_things_in_dir(const FilePath& in)
	{
		std::vector<FilePath> paths;

		FilePath dir = in + std::string("/");

		boost::filesystem::directory_iterator end;

		for (boost::filesystem::directory_iterator it(dir.str()); it != end; it++)
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