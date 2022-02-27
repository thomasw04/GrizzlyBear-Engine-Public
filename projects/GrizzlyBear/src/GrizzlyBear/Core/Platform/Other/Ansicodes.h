#pragma once

#include <stdint.h>

namespace GrizzlyBear {

	class AnsiTerminal
	{
	public:
		AnsiTerminal(std::ostream& oss)
			: m_Oss(oss) {}

		void move_to(uint32_t line, uint32_t column);
		void move_up(uint32_t lines);
		void move_down(uint32_t lines);
		void move_left(uint32_t columns);
		void move_right(uint32_t columns);
		void move_down_beg(uint32_t lines);
		void move_up_beg(uint32_t lines);
		void move_to_column(uint32_t column);

		void save_position();
		void restore_position();

		void erase_from_cursor();
		void erase_until_cursor();
		void erase_screen();
		void erase_saved();
		void erase_from_cursor_eol();
		void erase_from_beg_cursor();
		void erase_line();

		void reset_graphics();
		void set_bold(bool enable = true);
		void set_dim(bool enable = true);
		void set_italic(bool enable = true);
		void set_underline(bool enable = true);
		void set_blinking(bool enable = true);
		void set_inverse(bool enable = true);
		void set_hidden(bool enable = true);
		void set_strikethrough(bool enable = true);

		void set_default_color(bool foreground = true);
		void set_foreground(uint32_t id);
		void set_background(uint32_t id);

	private:
		std::ostream& m_Oss;
		const char* m_Esc = "\x1b";
		const char* m_EscB = "\x1b[";
	};
}


