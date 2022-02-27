#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/Core/FileIO.h"

#include <vector>
#include <unordered_map>

namespace GrizzlyBear {

	class GProject
	{
		friend class GProjectParser;
	public:
		struct GSection
		{
			GSection() {}

			GSection(const std::unordered_map<Gstring, Gstring>& map)
				: m_Values(map) {}

			Gstring get_value(const Gstring& key) const;

		public:

			void add_pair(const Gstring& key, const Gstring& value);

		private:

			std::unordered_map<Gstring, Gstring> m_Values;
		};

	public:
		const GSection get_section(const Gstring& sectionName);

	private:
		GProject(const FilePath& path)
			: m_File(path) {}

	private:
		FilePath m_File;

		std::unordered_map<Gstring, GSection> m_Sections;

	};

	class GProjectParser
	{
	public:
		static GProject parse_file(const FilePath& path);
		static void parse_file(GProject& prj);

		static void parse_file(std::unordered_map<Gstring, GProject::GSection>& destination, const FilePath& path);
	};
}
