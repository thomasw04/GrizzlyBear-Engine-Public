#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>
#include <GrizzlyBear/Core/FileIO.h>

#include <unordered_map>

namespace GrizzlyBear {
	
	class Gcff
	{
	public:
		const Gmapping& get(const Gstring& sectionName) const;

		Gmapping& operator[](const Gstring& sectionName);
		Gstring dump() const;

		void copy(std::unordered_map<Gstring, Gmapping>& dest);

		static Ref<Gcff> open(const FilePath& path);

	public:
		Gcff(const FilePath& path)
			: m_FilePath(path) {}

	private:
		FilePath m_FilePath;

		std::unordered_map<Gstring, Gmapping> m_Sections;
	};
}
