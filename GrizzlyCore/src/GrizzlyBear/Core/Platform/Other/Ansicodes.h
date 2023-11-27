#pragma once

#include <stdint.h>

namespace GrizzlyBear {

	enum class EColor
	{
		Black = 0,
		Red = 1,
		Green = 2,
		Yellow = 3,
		Blue = 4,
		Magenta = 5,
		Cyan = 6,
		White = 7,
	};

	struct CColor
	{
	public:
		explicit CColor(int id) : ID(id) {}
		int get_id() const { return ID; }

	private:
		int ID;
	};

	constexpr const char* const ESC = "\x1b";
	constexpr const char* const ESCB = "\x1b[";

	struct Mover
	{
		explicit Mover(int x, int y, bool start) : x(x), y(y), start(start) {}
		int x, y;
		bool start;
	};

	struct TargetMover
	{
		explicit TargetMover(int x, int y) : x(x), y(y) {}
		int x, y;
	};

	struct Saver
	{
		explicit Saver(bool save) : save(save) {}
		bool save;
	};

	struct CursorEraser
	{
		explicit CursorEraser(bool fromOrUntil, bool onlyLine)
			: fromOrUntil(fromOrUntil), onlyLine(onlyLine) {}

		bool fromOrUntil, onlyLine;
	};

	struct EraseLine {};
	struct EraseScreen {};
	struct EraseSaved {};

	inline std::ostream& operator<<(std::ostream& out, const EColor& color)
	{
		return out << ESCB << "38;5;" << (int)color << "m";
	}

	inline std::ostream& operator<<(std::ostream& out, const CColor& color)
	{
		return out << ESCB << "38;5;" << color.get_id() << "m";
	}

	inline std::ostream& operator<<(std::ostream& out, const EraseLine& er)
	{
		return out << ESCB << "2K";
	}

	inline std::ostream& operator<<(std::ostream& out, const EraseScreen& er)
	{
		return out << ESCB << "2J";
	}

	inline std::ostream& operator<<(std::ostream& out, const EraseSaved& er)
	{
		return out << ESCB << "3J";
	}

	inline std::ostream& operator<<(std::ostream& out, const CursorEraser& ce)
	{
		if (ce.fromOrUntil)
		{
			if (ce.onlyLine)
			{
				out << ESCB << "0K";
			}
			else
			{
				out << ESCB << "0J";
			}
		}
		else
		{
			if (ce.onlyLine)
			{
				out << ESCB << "1K";
			}
			else
			{
				out << ESCB << "1J";
			}
		}

		return out;
	}

	inline std::ostream& operator<<(std::ostream& out, const Saver& saver)
	{
		if (saver.save)
		{
			out << ESCB << " 7";
		}
		else
		{
			out << ESCB << " 8";
		}

		return out;
	}

	inline std::ostream& operator<<(std::ostream& out, const TargetMover& mover)
	{
		if (mover.x < 0)
		{
			return out;
		}
		else
		{
			out << ESCB << mover.x << "G";
		}

		if (mover.y >= 0)
		{
			out << ESCB << mover.y << ";" << mover.x << "H";
		}

		return out;
	}

	inline std::ostream& operator<<(std::ostream& out, const Mover& mover)
	{
		if (mover.start)
		{
			if (mover.y > 0)
			{
				out << ESCB << mover.y << "F";
			}
			else
			{
				out << ESCB << -mover.y << "E";
			}
		}
		else
		{
			if (mover.x > 0)
			{
				out << ESCB << mover.x << "C";
			}
			else
			{
				out << ESCB << -mover.x << "D";
			}

			if (mover.y > 0)
			{
				out << ESCB << mover.y << "A";
			}
			else
			{
				out << ESCB << -mover.y << "B";
			}
		}

		return out;
	}
}


