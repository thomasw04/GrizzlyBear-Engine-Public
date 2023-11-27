#pragma once

#include <unordered_map>
#include <vector>

#include "GrizzlyBear/Core/FileIO.h"

#include "Gcff.h"

namespace GrizzlyBear {

	class ChunkFetcher
	{
	public:
		ChunkFetcher(const Gstring& target)
			: m_Target(target) {}

		virtual bool is_locked() = 0;
		virtual bool unlock(const Gstring& key, const Gstring& nonce) = 0;
		virtual void lock(const Gstring& key, const Gstring& nonce) = 0;

		virtual Gbuffer get_asset_data(const Gstring& relPath) = 0;
		virtual bool save_asset_data(const Gstring& relPath, const Gbuffer& data) = 0;

		virtual Gstring get_asset_path(const Gstring& relPath) = 0;

		virtual std::vector<RelativePath> find_assets(const Gstring& identifier, bool recursive) = 0;
		virtual std::vector<RelativePath> find_entries(const Gstring& relPath) = 0;

		virtual std::unordered_map<Gstring, Gmapping> update_configs() = 0;
	protected:
		const Gstring m_Target;
	};
}
