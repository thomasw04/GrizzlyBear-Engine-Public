#include <gbpch.h>

#ifdef GRIZZLY_PLATFORM_WINDOWS

#include "WindowsPlatform.h"

#include "GrizzlyBear/Core/Application.h"

#include <GLFW/glfw3.h>

namespace GrizzlyBear {

	static void GLFWErrorCallback(int error, const char* description)
	{
		GRIZZLY_CORE_LOG(LogLevel::Error, "Module: GLFW, Message: ({0}): {1}", error, description);
	}

	WindowsPlatform::WindowsPlatform()
	{
		int success = glfwInit();

		if (!success)
		{
			m_Info = {ModuleState::ERROR_FATAL, "Failed to initialize glfw."};
			return;
		}

		glfwSetErrorCallback(GLFWErrorCallback);

		RECT desktop;

		const HWND hDesktop = GetDesktopWindow();

		GetWindowRect(hDesktop, &desktop);

		m_Width = desktop.right;
		m_Height = desktop.bottom;

		//Enable ansi support
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleMode(console, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | DISABLE_NEWLINE_AUTO_RETURN);

		m_Info = { ModuleState::GOOD, "" };
	}

	WindowsPlatform::~WindowsPlatform()
	{

	}

	bool WindowsPlatform::get_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt)
	{
		Gstring filepath_internal(MAX_PATH, '\0');

		OPENFILENAMEA ofn = {};

		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = filter.c_str();
		ofn.lpstrFile = &filepath_internal[0];
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = defaultExt.c_str();
		ofn.lpstrTitle = title.c_str();
		
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn))
		{
			filepath = filepath_internal;

			//Delete all null termination characters
			filepath.erase(std::find(filepath.begin(), filepath.end(), '\0'), filepath.end());
			return true;
		}

		return false;
	}

	bool WindowsPlatform::save_file_impl(Gstring& filepath, const Gstring& title, const Gstring& filter, const Gstring& defaultExt)
	{
		Gstring filepath_internal(MAX_PATH, '\0');

		OPENFILENAMEA ofn = {};

		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = filter.c_str();
		ofn.lpstrFile = &filepath_internal[0];
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = title.c_str();
		ofn.lpstrDefExt = defaultExt.c_str();
		ofn.Flags = OFN_DONTADDTORECENT;

		if (GetSaveFileNameA(&ofn))
		{
			filepath = filepath_internal;

			//Delete all null termination characters
			filepath.erase(std::find(filepath.begin(), filepath.end(), '\0'), filepath.end());
			return true;
		}

		return false;
	}

	int WindowsPlatform::get_display_width_impl() const
	{
		return m_Width;
	}

	int WindowsPlatform::get_display_height_impl() const
	{
		return m_Height;
	}

	Gstring WindowsPlatform::get_executable_path_impl() const
	{
		HMODULE hmod = GetModuleHandle(NULL);

		char filepath[255];

		if (hmod)
		{
			GetModuleFileNameA(hmod, filepath, sizeof(filepath));
			return Gstring(filepath);
		}

		GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Platform, Message: Unable to retrieve the executable path.");
		return Gstring();
	}

	bool WindowsPlatform::is_absolute_filepath_impl(const Gstring& path) const
	{
		if (path[0] == '/' || (std::isalpha(path[0]) && path[1] == ':' && path[2] == '/'))
		{
			return true;
		}

		return false;
	}

	ModuleInfo WindowsPlatform::get_info() const
	{
		return m_Info;
	}

	bool WindowsPlatform::is_valid_filepath_impl(const Gstring& path) const
	{
		if (path.empty()) return false;

		for (int i = 0; i < path.size(); i++)
		{
			//Filter windows reserved characters, 
			if ((path[i] == '<' || path[i] == '>' || (path[i] == ':' && i != 1) || path[i] == '\"' || path[i] == '|' || path[i] == '?' || path[i] == '*') || (path[i] >= 0 && path[i] <= 31))
			{
				return false;
			}
		}

		//Path is not allowed to end with period or space
		if (path[path.size() - 1] == '.' || path[path.size() - 1] == ' ') return false;

		return true;
	}

}

#endif
