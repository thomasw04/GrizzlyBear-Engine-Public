#include <gbpch.h>
#include "Ansicodes.h"

namespace GrizzlyBear {

	void AnsiTerminal::move_to(uint32_t line, uint32_t column)
	{
		m_Oss << m_EscB << line << ";" << column << "H";
	}

	void AnsiTerminal::move_up(uint32_t lines)
	{
		m_Oss << m_EscB << lines << "A";
	}

	void AnsiTerminal::move_down(uint32_t lines)
	{
		m_Oss << m_EscB << lines << "B";
	}

	void AnsiTerminal::move_left(uint32_t columns)
	{
		m_Oss << m_EscB << columns << "D";
	}

	void AnsiTerminal::move_right(uint32_t columns)
	{
		m_Oss << m_EscB << columns << "C";
	}

	void AnsiTerminal::move_down_beg(uint32_t lines)
	{
		m_Oss << m_EscB << lines << "E";
	}

	void AnsiTerminal::move_up_beg(uint32_t lines)
	{
		m_Oss << m_EscB << lines << "F";
	}

	void AnsiTerminal::move_to_column(uint32_t column)
	{
		m_Oss << m_EscB << column << "G";
	}

	void AnsiTerminal::save_position()
	{
		m_Oss << m_Esc << " 7";
	}

	void AnsiTerminal::restore_position()
	{
		m_Oss << m_Esc << " 8";
	}

	void AnsiTerminal::erase_from_cursor()
	{
		m_Oss << m_EscB << "0J";
	}

	void AnsiTerminal::erase_until_cursor()
	{
		m_Oss << m_EscB << "1J";
	}

	void AnsiTerminal::erase_screen()
	{
		m_Oss << m_EscB << "2J";
	}

	void AnsiTerminal::erase_saved()
	{
		m_Oss << m_EscB << "3J";
	}

	void AnsiTerminal::erase_from_cursor_eol()
	{
		m_Oss << m_EscB << "0K";
	}

	void AnsiTerminal::erase_from_beg_cursor()
	{
		m_Oss << m_EscB << "1K";
	}

	void AnsiTerminal::erase_line()
	{
		m_Oss << m_EscB << "2K";
	}

	void AnsiTerminal::reset_graphics()
	{
		m_Oss << m_EscB << "0m";
	}

	void AnsiTerminal::set_bold(bool enable)
	{
		if(enable)
			m_Oss << m_EscB << "1m";
		else
			m_Oss << m_EscB << "22m";
	}

	void AnsiTerminal::set_dim(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "2m";
		else
			m_Oss << m_EscB << "22m";
	}

	void AnsiTerminal::set_italic(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "3m";
		else
			m_Oss << m_EscB << "23m";
	}

	void AnsiTerminal::set_underline(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "4m";
		else
			m_Oss << m_EscB << "24m";
	}

	void AnsiTerminal::set_blinking(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "5m";
		else
			m_Oss << m_EscB << "25m";
	}

	void AnsiTerminal::set_inverse(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "7m";
		else
			m_Oss << m_EscB << "27m";
	}

	void AnsiTerminal::set_hidden(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "8m";
		else
			m_Oss << m_EscB << "28m";
	}

	void AnsiTerminal::set_strikethrough(bool enable)
	{
		if (enable)
			m_Oss << m_EscB << "9m";
		else
			m_Oss << m_EscB << "29m";
	}

	void AnsiTerminal::set_default_color(bool foreground)
	{
		if (foreground)
			m_Oss << m_EscB << "39m";
		else
			m_Oss << m_EscB << "49m";
	}

	void AnsiTerminal::set_foreground(uint32_t id)
	{
		m_Oss << m_EscB << "38;5;" << id << "m";
	}

	void AnsiTerminal::set_background(uint32_t id)
	{
		m_Oss << m_EscB << "48;5;" << id << "m";
	}

}
