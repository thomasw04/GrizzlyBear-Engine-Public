#pragma once

#ifdef GRIZZLY_PLATFORM_LINUX

#include "GrizzlyBear/Core/Platform/Platform.h"

namespace GrizzlyBear {

	class LinuxPlatform : public Platform
	{
	public:
		LinuxPlatform();
		virtual ~LinuxPlatform();

		virtual bool get_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt) override;
		virtual bool save_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt) override;

		virtual int get_display_width_impl() const override;
		virtual int get_display_height_impl() const override;

		virtual Gstring get_executable_path_impl() const override;

		virtual bool is_valid_filepath_impl(const Gstring& path) const override;
		virtual bool is_absolute_filepath_impl(const Gstring& path) const override;

	private:
		int m_Width;
		int m_Height;
	};
}
#endif

