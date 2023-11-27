#pragma once

#include "GrizzlyBear/Core/BaseDefines.h"
#include "GrizzlyBear/Core/Executor.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <sstream>

namespace GrizzlyBear {

	class Terminal : public Executable
	{
		using CmdFunc = std::function<void(const std::vector<Gstring>&)>;

		struct SaveStream : std::streambuf
		{
			SaveStream(std::streambuf* buffer)
				: m_Buffer(buffer) {}

			std::streambuf::int_type overflow(std::streambuf::int_type c)
			{
				m_Buffer->sputc(c);
				if(m_Read)
					m_History << (char)c;
				return 0;
			}

			std::streambuf* m_Buffer;
			std::stringstream m_History;
			bool m_Read = true;
		};

	public:
		Terminal();

		virtual int run(int argc, char* argv[]) override;

		void register_command(Gstring name, const CmdFunc& args);

		std::ostream& printBegin(std::ostream& out = std::cout, int mode = 1);

	public:
		static Gstring s_Header;

		static std::unordered_map<Gstring, CmdFunc> m_Cmds;

	private:
		SaveStream m_SaveStream;
		std::ostream m_Stream;
		int m_CurrentSession = 0;
		bool m_AcceptRL = true;
	};
}
