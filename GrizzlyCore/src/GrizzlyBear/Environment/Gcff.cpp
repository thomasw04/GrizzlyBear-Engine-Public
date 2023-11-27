#include <gbpch.h>

#include "Gcff.h"

namespace GrizzlyBear {

	const Gmapping& Gcff::get(const Gstring& sectionName) const
	{
		if (m_Sections.contains(sectionName))
		{
			return m_Sections.at(sectionName);
		}
		return Gmapping();
	}

	Gmapping& Gcff::operator[](const Gstring& sectionName)
	{
		return m_Sections[sectionName];
	}

	Gstring Gcff::dump() const
	{
		Gstring returnStr;

		for (auto s1 : m_Sections)
		{
			returnStr.append("[").append(s1.first).append("]\n");

			for (auto s2 : s1.second)
			{
				returnStr.append(s2.first).append(" = ").append(s2.second).append(";\n");
			}
		}

		return returnStr;
	}

	void Gcff::copy(std::unordered_map<Gstring, Gmapping>& dest)
	{
		for (auto s1 : m_Sections)
		{
			for (auto s2 : s1.second)
			{
				dest[s1.first][s2.first] = s2.second;
			}
		}
	}

	Ref<Gcff> Gcff::open(const FilePath& path)
	{
		Ref<Gcff> returnValue = std::make_shared<Gcff>(path);

		if (FileIO::exists(path))
		{
			File gprj = FileIO::open(path);

			std::ifstream& input = gprj.get_ifstream();

			std::stringstream stream;
			stream << input.rdbuf();

			std::string source = stream.str();

			size_t pos = source.find('[', 0);

			std::string sectionName;
			std::string key;
			std::string value;

			// Remove all whitespaces
			int i, j, max;

			for (i = 0, j = 0; i < source.size(); i++)
			{
				if (source[i] != ' ')
				{
					source[j] = source[i];
					j++;
				}
			}

			source[j] = '\0';
			max = j;

			//Remove end line
			for (i = 0, j = 0; i < max; i++)
			{
				if (source[i] != '\n')
				{
					source[j] = source[i];
					j++;
				}
			}

			source[j] = '\0';
			source = source.substr(0, j);

			while (pos != source.npos)
			{
				size_t endSection = source.find_first_of(']', pos);

				if (endSection == source.npos)
				{
					GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a \']\'", path.str().c_str(), endSection);
					returnValue->m_Sections.clear();
					return returnValue;
				}

				sectionName = source.substr(pos + 1, endSection - (pos + 1));

				if (sectionName.empty())
				{
					GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a section name", path.str().c_str(), pos + 1);
					returnValue->m_Sections.clear();
					return returnValue;
				}

				pos = endSection + 1 > source.size() - 1 ? source.npos : endSection + 1;

				while (pos != source.npos && pos != '[')
				{
					size_t endKey = source.find_first_of('=', pos);

					if (endKey == source.npos)
					{
						GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a \'=\'", path.str().c_str(), endKey);
						returnValue->m_Sections.clear();
						return returnValue;
					}

					key = source.substr(pos, endKey - pos);

					if (key.empty())
					{
						GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a key", path.str().c_str(), pos);
						returnValue->m_Sections.clear();
						return returnValue;
					}

					pos = endKey + 1 > source.size() - 1 ? source.npos : endKey + 1;

					if (pos == source.npos)
					{
						GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a value", path.str().c_str(), pos);
						returnValue->m_Sections.clear();
						return returnValue;
					}

					size_t endValue = source.find_first_of(';', pos);

					if (endValue == source.npos)
					{
						GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a \';\'", path.str().c_str(), endValue);
						returnValue->m_Sections.clear();
						return returnValue;
					}

					value = source.substr(pos, endValue - pos);

					if (value.empty())
					{
						GRIZZLY_CORE_LOG(LogLevel::Error, "File: {0} at position {1}, Syntax error: Expected a value", path.str().c_str(), pos);
						returnValue->m_Sections.clear();
						return returnValue;
					}

					pos = endValue + 1 > source.size() - 1 ? source.npos : endValue + 1;

					(*returnValue)[sectionName].insert({ key, value });
				}
			}

			return returnValue;
		}
	}

}