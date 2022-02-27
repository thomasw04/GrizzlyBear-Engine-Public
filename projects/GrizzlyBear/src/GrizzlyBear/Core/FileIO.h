#pragma once

#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#include <GrizzlyBear/Core/BaseDefines.h>

#include <GrizzlyBear/Core/Platform/Platform.h>

namespace GrizzlyBear {

	class FilePath
	{
	public:

		FilePath(const Gstring& path);

		Gstring get_path() const;
		Gstring get_parent() const;
		Gstring get_name() const;
		Gstring get_name_woe() const;
		Gstring get_extension() const;

		bool is_absolute_path() const;
		bool is_valid_path() const;
		bool is_empty() const;
		bool is_directory() const;

		void add(const Gstring& next);
		void back(int steps);

		FilePath& operator+=(const FilePath& rhs);
		FilePath operator+(const FilePath& rhs);

	private:

		GRIZZLY_REFLECT_VARS
		(
			(Gstring) m_Path,
			(bool) m_IsAbsolute,
			(bool) m_IsValid,
			(bool) m_IsEmpty,
			(bool) m_IsDirectory
		)
	};

	class RelativePath
	{
	public:
		RelativePath(const Gstring& relative, const FilePath& parent)
			: m_FullPath(parent.get_path() + relative), m_Parent(parent), m_Relative(relative) {}

		FilePath m_FullPath;
		FilePath m_Parent;
		Gstring m_Relative;
	};

	class File
	{
		friend class FileIO;

		File(const FilePath& path)
			: m_Path(path), out(path.get_path(), std::ios::app), in(path.get_path()) {}

	public:

		File(File& file);

		~File();

		bool copy(const FilePath& destination, bool overwrite);
		bool move(const FilePath& newPath);
		bool rename(const Gstring& newName);
		bool is_directory() const;
		bool destroy();

		void set_ofstream_mode(std::ios_base::openmode mode);
		void set_ifstream_mode(std::ios_base::openmode mode);

		std::ofstream& get_ofstream();
		std::ifstream& get_ifstream();

		FilePath get_path();
	private:
		FilePath m_Path;

		std::ofstream out;
		std::ifstream in;
	};

	class FileIO
	{
	public:

		static File open(const FilePath& path);
		static Gbuffer get_file_content(const FilePath& path);

		static bool exists(const FilePath& path);
		static bool set_current_path(const FilePath& path);
		static FilePath get_current_path();
		static FilePath get_program_directory();

		static std::vector<RelativePath> get_files_in_dir_rel(const File& dir, bool recursive);
		static std::vector<FilePath> get_files_in_dir(const File& dir, bool recursive);
		static std::vector<RelativePath> get_things_in_dir_rel(const File& dir);
		static std::vector<FilePath> get_things_in_dir(const File& dir);

		static FilePath ask_project_file();
	};


}
