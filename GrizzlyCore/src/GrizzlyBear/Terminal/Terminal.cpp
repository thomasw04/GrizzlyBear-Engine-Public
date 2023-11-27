#include <gbpch.h>

#include "Terminal.h"

#include "GrizzlyBear/Core/Platform/Other/Ansicodes.h"

#include <boost/program_options.hpp>

#include <algorithm>

#include <fcntl.h>

namespace GrizzlyBear {

    namespace po = boost::program_options;

	Gstring Terminal::s_Header = R"(

==================================================================================================
==================================================================================================
              ________   _____    _______   _______   _______   _     __      __
             |  ______| |  __ \  |__   __| |_____  | |_____  | | |    \ \    / /
             1 1  ____  | |__) |    1 1         / /       / /  1 1     \ \  / /
             | | |__  | |  _  /     | |       / /       / /    | |      \ \/ /
             1 1____| | | | \ \   __1 1__   / /____   / /____  1 1_____  |  |
             |________| |_|  \_\ |_______| |_______| |_______| |_______| |__|
                          ______   ______   ______   _____
                         |  __  \ |  ____| || __ || |  __ \             // // //
                         | |__| / | |____  \/||||\/ | |__) |   _____    || || ||
                         |  __ |  |  ____|   \/\/   |  _  /   |_____|   /\ \/ /\
                         | |__| \ | |____  /\|  |/\ | | \ \             || || ||
                         |______/ |______| |_|  |_| |_|  \_\            \\ \\ \\
             
==================================================================================================
   GrizzlyBear-Engine Developer Terminal v1.0 Alpha
==================================================================================================

)";

    std::unordered_map<Gstring, Terminal::CmdFunc> Terminal::m_Cmds;

    std::ostream& Terminal::printBegin(std::ostream& out, int mode)
    {
        switch (mode)
        {
        case 0:
            return out << CColor(42) << "[Console] [INPUT]" << EColor::White;
        case 1:
            return out << EColor::White << "[Console] [TRACE]";
        case 2:
            return out << EColor::Yellow << "[Console] [WARN]";
        case 3:
            return out << EColor::Red << "[Console] [ERROR]";
        case 4:
            return out << CColor(125) << "[Console] [FATAL]";
        default:
            return out;
        }
    }

    Terminal::Terminal()
        : m_SaveStream(std::cout.rdbuf()), m_Stream(&m_SaveStream)
    {
        m_Stream << s_Header << std::endl;

        register_command("schse", [&](const std::vector<Gstring>& args)
        {
            if (args.size() == 0)
            {
                printBegin() << " Der geheimnisvolle schse (kleingeschrieben):" << EColor::White << std::endl;
                printBegin() << "   Viele Legenden ranken sich um schse. Es ist nur sehr wenig ueber seine Person bekannt." << std::endl;
                printBegin() << "   Manch einer meint er sei ein Informatiker. Einen Helden so haben sie ihn genannt." << std::endl;
                printBegin() << "   Doch ganz so scheint es nicht zu sein." << std::endl;
                printBegin() << "   Es wird gemunkelt, dass er geht, bei den Mathematikern aus und ein." << std::endl;
                printBegin() << "   Und doch hoffen wir, dass dies nicht so ist." << std::endl;
                printBegin() << "   Das das Boese ihn nicht zerfrisst. XD" << std::endl;
            }
        });

        register_command("c+thomas", [&](const std::vector<Gstring>& args)
        {
            if (args.size() == 0)
            {
                printBegin() << " C+Thomas (auch genannt: Der Schoepfer):" << EColor::White << std::endl;
                printBegin() << "   C+Thomas ist der Ursprung dieses Betrieb.... Engine. Er hat sie erschaffen und geformt." << std::endl;
                printBegin() << "   Sein grosser Plan ist nicht begreifbar. Und doch so vollkommen und perfekt." << std::endl;
                printBegin() << "   Sein coding style ist der einzig richtige. Ihr werdet schon sehen." << std::endl;
                printBegin() << "   Er belaechelt die Mathematiker. Sie haben den richtigen Weg noch nicht entdeckt." << std::endl;
                printBegin() << "   Und er ist stolz auf die Engine Entwickler dieser Welt." << std::endl;
                printBegin() << "   Sie machen sie so perfekt." << std::endl;
            }
        });

        register_command("clear", [&](const std::vector<Gstring>& args)
        {
            if (args.size() == 0)
            {
                m_Stream << EraseScreen();
            }
        });

        register_command("attach", [&](const std::vector<Gstring>& args)
        {
            if (args.size() == 1)
            {
                if (args[0] == "master")
                {
                    if (m_CurrentSession != 0)
                    {
                        m_Stream << EraseScreen();
                        m_Stream << TargetMover(0, 0);
                        m_Stream << m_SaveStream.m_History.str() << std::endl;
                        m_SaveStream.m_Read = true;
                        m_CurrentSession = 0;
                    }
                }
                else
                {
                    if (args[0] == "core")
                    {
                        m_SaveStream.m_Read = false;
                        m_Stream << EraseScreen();
                        m_Stream << TargetMover(0, 0);
                        Log::print_core_log(m_Stream);
                        m_CurrentSession = 1;
                    }
                    else if (args[0] == "client")
                    {
                        m_SaveStream.m_Read = false;
                        m_Stream << EraseScreen();
                        m_Stream << TargetMover(0, 0);
                        Log::print_client_log(m_Stream);
                        m_CurrentSession = 2;
                    }
                    else
                    {
                        printBegin(m_Stream, 3) << " Invalid session: " << args[0] << EColor::White << std::endl;
                    }

                    m_AcceptRL = false;
                }
            }
        });
    }

    int Terminal::run(int argc, char* argv[])
    {
        while (true)
        {
            Executable::interrupt_point();

            while (has_result())
            {
                Gstring result = read_result();

                if (result.substr(0, 6) == "RELOAD")
                {
                    if (m_AcceptRL)
                    {
                        switch (m_CurrentSession)
                        {
                        case 1:
                            m_Stream << Mover(0, 1, true);
                            m_Stream << EraseLine();
                            m_Stream << Mover(0, 0, true);
                            Log::print_core_log(m_Stream, 1);
                            break;
                        case 2:
                            m_Stream << Mover(0, 1, true);
                            m_Stream << EraseLine();
                            m_Stream << Mover(0, 0, true);
                            Log::print_client_log(m_Stream, 1);
                            break;
                        }
                    }
                }
                else if (result.substr(0, 5) == "TRACE")
                {
                    printBegin(m_Stream, 1) << result.substr(7);
                }
                else if (result.substr(0, 4) == "WARN")
                {
                    printBegin(m_Stream, 2) << result.substr(6);
                }
                else if (result.substr(0, 5) == "ERROR")
                {
                    printBegin(m_Stream, 3) << result.substr(7);
                }
                else if (result.substr(0, 5) == "FATAL")
                {
                    printBegin(m_Stream, 4) << result.substr(7);
                }
            }

            m_AcceptRL = true;

            if (m_CurrentSession == 1)
            {
                m_Stream << Mover(0, 1, true);
            }
            
            printBegin(m_Stream, 0) << "> ";
            
            Gstring line; std::getline(std::cin, line);
            std::vector<Gstring> args = po::split_unix(line);

            //Save input to history
            if(m_SaveStream.m_Read)
                m_SaveStream.m_History << line << std::endl;

            if (args.empty()) continue;

            std::transform(args[0].begin(), args[0].end(), args[0].begin(), [](unsigned char c)
            { 
                return std::tolower(c);
            });

            auto it = m_Cmds.find(args[0]);

            if (it != m_Cmds.end())
            {
                //Command exists
                args.erase(args.begin());
                it->second(args);
            }
            else
            {
                printBegin(m_Stream, 3) << " Unknown command: " << args[0] << EColor::White << std::endl;
            }
            
        }

        return 0;
    }

    void Terminal::register_command(Gstring name, const CmdFunc& args)
    {
        m_Cmds.insert({ name, args });
    }

}
