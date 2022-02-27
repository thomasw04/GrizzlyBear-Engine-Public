#include <gbpch.h>

#include "GProjectParser.h"

namespace GrizzlyBear {

	//GSection--------------------------------------------------------------------------------------------

	Gstring GProject::GSection::get_value(const Gstring& key) const
	{
		try
		{
			return m_Values.at(key);
		}
		catch (std::out_of_range& e)
		{
			(void)e;
			return "";
		}
	}

	void GProject::GSection::add_pair(const Gstring& key, const Gstring& value)
	{
		m_Values.insert({ key, value });
	}

	//GProject--------------------------------------------------------------------------------------------

	const GProject::GSection GProject::get_section(const Gstring& sectionName)
	{
		return m_Sections[sectionName];
	}

	//GProjectParser--------------------------------------------------------------------------------------

	GProject GProjectParser::parse_file(const FilePath& path)
	{
		GProject parser(path);
		parse_file(parser.m_Sections, path);
		return parser;
	}

	void GProjectParser::parse_file(GProject& prj)
	{
		parse_file(prj.m_Sections, prj.m_File);
	}

	void GProjectParser::parse_file(std::unordered_map<Gstring, GProject::GSection>& destination, const FilePath& path)
	{
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
				GRIZZLY_CORE_ASSERT(endSection != source.npos, "Syntax error: Expected a \']\'");

				sectionName = source.substr(pos + 1, endSection - (pos + 1));
				GRIZZLY_CORE_ASSERT(!sectionName.empty(), "Syntax error: Expected a section");

				pos = endSection + 1 > source.size() - 1 ? source.npos : endSection + 1;

				while (pos != source.npos && pos != '[')
				{
					size_t endKey = source.find_first_of('=', pos);
					GRIZZLY_CORE_ASSERT(endKey != source.npos, "Syntax error: Expected a \'=\'");

					key = source.substr(pos, endKey - pos);
					GRIZZLY_CORE_ASSERT(!key.empty(), "Syntax error: Expected a key");

					pos = endKey + 1 > source.size() - 1 ? source.npos : endKey + 1;
					GRIZZLY_CORE_ASSERT(pos != source.npos, "Syntax error: Expected a value");

					size_t endValue = source.find_first_of(';', pos);
					GRIZZLY_CORE_ASSERT(endValue != source.npos, "Syntax error: Expected a \';\'");

					value = source.substr(pos, endValue - pos);
					GRIZZLY_CORE_ASSERT(!value.empty(), "Syntax error: Expected a value");

					pos = endValue + 1 > source.size() - 1 ? source.npos : endValue + 1;

					destination[sectionName].add_pair(key, value);
				}
			}
		}
	}

}