#pragma once

#include <memory>
#include <string>
#include <algorithm>

#include "Reflection/Reflector.h"

#ifdef GRIZZLY_PLATFORM_MACOS
	#include <TargetConditionals.h>
#endif

#ifdef GRIZZLY_DEBUG 
    #define GRIZZLY_ENABLE_ASSERTS

    #ifdef GRIZZLY_PLATFORM_WINDOWS
        #define GRIZZLY_BREAKPOINT() __debugbreak();
    #elif defined(GRIZZLY_PLATFORM_MACOS)
        #define GRIZZLY_BREAKPOINT() __builtin_trap();
    #elif defined(GRIZZLY_PLATFORM_LINUX)
        #include <signal.h>
        #define GRIZZLY_BREAKPOINT() raise(SIGTRAP);
    #endif
#endif // GRIZZLY_DEBUG

#if defined(_WIN32) || defined(_WIN64)
	#if defined(_WIN64)
		#define GRIZZLY_ENV_64
	#else
		#define GRIZZLY_ENV_32
	#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
	#if defined(__x86_64__) || defined(__ppc64__)
		#define GRIZZLY_ENV_64
	#else
		#define GRIZZLY_ENV_32
	#endif
#endif


//Helper
#define GRIZZLY_BIT(x) (1 << x)

#define GRIZZLY_BIND_NDEVENT(function) std::bind(&function, this)
#define GRIZZLY_BIND_EVENT(function) std::bind(&function, this, std::placeholders::_1)

#define GRIZZLY_DISABLED 0
#define GRIZZLY_ENABLED 1

//end

namespace GrizzlyBear {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using Gstring = std::string;
	using CGs = const Gstring&;
	using Gmapping = std::unordered_map<Gstring, Gstring>;

	template<typename T>
	struct Tag
	{
		using type = T;
	};

	enum class Dimensions
	{
		None = 0, One = 1, TWO = 2, THREE = 3 
	};

	enum class Asset
	{
		None = 0, Shader = 1, Texture2D = 2
	};

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time), m_LastTime(0) {}

		void update(float currentTime)
		{ 
			m_Time = currentTime - m_LastTime;
			m_LastTime = currentTime;
		}

		operator float() const { return m_Time; }

		float get_seconds() const { return m_Time; }
		float get_millis() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
		float m_LastTime;
	};

	enum class ModuleState
	{
		GOOD = 0, ERROR_USABLE = 1, ERROR_RISKY = 2, ERROR_FATAL = 3  
	};

	struct ModuleInfo
	{
		ModuleState STATE;
		Gstring WHAT;
	};

	struct Gbuffer
	{
		enum class State
		{
			//Good - The buffer is okay the data can be used
			//Failed - The buffer is empty due to an error in getting the data
			//Invalid - A part of the data is in the buffer but an error was raised during the filling process
			GOOD = 0, FAILED, INVALID
		};

		bool is_good() const { return m_State == State::GOOD; }
		bool is_failed() const { return m_State == State::FAILED; }
		bool is_invalid() const { return m_State == State::INVALID; }

		Gstring m_Buffer;
		State m_State;
	};

	struct Version
	{
		//A Version is build of the following scheme "major.minor[.revision] type"
		// Major - Complete rewrites, complete new user experience (Probably not compatible with earlier versions)
		// Minor - New features, smaller changes to existing behaviour (Mostly compatible with earlier versions)
		// Revision - Bugfixes, small optimization, non affecting things to the user experience (Optional) (Must be compatible with earlier versions)
		// Type - Indicating what kind of build the software is e.g. alpha, beta, release (When the type changes the whole counter resets) (Not compatible with earlier versions)

		//The compare functions are not considering the type, they are just comparing the numbers
		Version() : m_Internal(0) {}

		Version(const Gstring& identifier)
		{
			m_Identifier = identifier;
			size_t index = m_Identifier.rfind(' ');
			m_Type = m_Identifier.substr(index);

			pack_version();
		}

		bool operator>(const Version& other)
		{
			return !operator<=(other);
		}

		bool operator<(const Version& other)
		{
			return !operator>=(other);
		}

		bool operator>=(const Version& other)
		{
			return operator>(other) || operator==(other);
		}

		bool operator<=(const Version& other)
		{
			return m_Internal < other.m_Internal || operator==(other);
		}

		bool operator==(const Version& other)
		{
			return m_Internal == other.m_Internal;
		}

		bool operator!=(const Version& other)
		{
			return !operator==(other);
		}

		void validate()
		{
			unpack_version();
		}

	private:

		void pack_version()
		{
			size_t part = 0;
			size_t begin = 0;

			int major = 0;
			int minor = 0;
			int revision = 0;

			for (size_t i = 0; i < m_Identifier.size(); i++)
			{
				if (m_Identifier[i] == '.' || m_Identifier[i] == ' ')
				{
					int group = std::stoi(m_Identifier.substr(begin, i - begin));

					if (part == 0)
						major = group;
					if (part == 1)
						minor = group;
					if (part == 2)
						revision = group;

					begin = i + 1;
					part++;
				}

				if (part == 3 && i == m_Identifier.size() - 1)
				{
					m_Type = m_Identifier.substr(begin, i - begin + 1);
				}
			}

			m_Internal = (major * 1000000) + (minor * 10000) + revision;
		}

		void unpack_version()
		{
			int major = m_Internal / 1000000;
			int minor = (m_Internal - (major * 1000000)) / 10000;
			int revision = (m_Internal - (major * 1000000) - (minor * 10000));

			std::stringstream s;

			s << major << "." << minor;

			if (revision != 0) s << "." << revision;

			s << " " << m_Type;

			m_Identifier = s.str();
		}

	private:

		GRIZZLY_REFLECT_VARS
		(
			(Gstring) m_Type,
			(int) m_Internal
		)

		Gstring m_Identifier;
	};

	class ArgumentUtils
	{
	public:
		ArgumentUtils(int argc, char* argv[])
			: m_Count(argc), m_Begin(argv) {}

		bool is_enabled(const std::string& opt)
		{
			return std::find(m_Begin, m_Begin + m_Count, opt) != m_Begin + m_Count;
		}

		std::string get_value(const std::string& opt)
		{
			auto it = std::find(m_Begin, m_Begin + m_Count, opt);

			if (it != m_Begin + m_Count)
			{
				if (++it != m_Begin + m_Count)
				{
					if ((*it)[0] != '-')
					{
						return *it;
					}
				}
			}
		}

	private:
		int m_Count;
		char** m_Begin;
	};

}