#pragma once

#include <glm/glm.hpp>

#include "GrizzlyBear/Core/FileIO.h"
#include "GrizzlyBear/Core/Platform/Other/Ansicodes.h"

namespace GrizzlyBear {

	class Platform
	{
	public:
		static ModuleState init();

		virtual ~Platform() {};
			    
		inline static bool get_file(Gstring& filepath, const Gstring& title, const Gstring& filter = "", const Gstring& defaultExt = "") { return m_Platform->get_file_impl(filepath, title, filter, defaultExt); }
		inline static bool save_file(Gstring& filepath, const Gstring& title, const Gstring& filter = "", const Gstring& defaultExt = "") { return m_Platform->save_file_impl(filepath, title, filter, defaultExt); }
			    
		inline static int get_display_width() { return m_Platform->get_display_width_impl(); }
		inline static int get_display_height() { return m_Platform->get_display_height_impl(); }

		inline static Gstring get_executable_path() { return m_Platform->get_executable_path_impl(); }

		inline static bool is_valid_filepath(const Gstring& path) { return m_Platform->is_valid_filepath_impl(path); }
		inline static bool is_absolute_filepath(const Gstring& path) { return m_Platform->is_absolute_filepath_impl(path); }

		inline static AnsiTerminal get_ansi_std() { return m_StdOut; }

		/*
		#ifdef GRIZZLY_ENV_64
			inline static void* map_memory(void* address, size_t length, int protection, int flags, uint64_t offset) { return m_Platform->map_memory_impl(address, length, protection, flags, offset); }
		#elif GRIZZLY_ENV_32
			inline static void* map_memory(void* address, size_t length, int protection, int flags, uint32_t offset) { return m_Platform->map_memory_impl(address, length, protection, flags, offset); }
		#endif*/

		static Gstring get_date(const char* format);

	private:
		virtual bool get_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt) = 0;
		virtual bool save_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt) = 0;

		virtual int get_display_width_impl() const = 0;
		virtual int get_display_height_impl() const = 0;

		virtual Gstring get_executable_path_impl() const = 0;

		virtual bool is_valid_filepath_impl(const Gstring& path) const = 0;
		virtual bool is_absolute_filepath_impl(const Gstring& path) const = 0;

		/*
		#ifdef GRIZZLY_ENV_64
				virtual void* map_memory_impl(void* address, size_t length, int protection, int flags, uint64_t offset) = 0;
		#elif GRIZZLY_ENV_32
				virtual void* map_memory_impl(void* address, size_t length, int protection, int flags, uint32_t offset) = 0;
		#endif*/

	private:
		static Ref<Platform> m_Platform;
		static AnsiTerminal m_StdOut;
	};
}
