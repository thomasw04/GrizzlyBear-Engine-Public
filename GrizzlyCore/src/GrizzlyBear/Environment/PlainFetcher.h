#pragma once

#include "ChunkFetcher.h"

#include "Gcff.h"

namespace GrizzlyBear {

	class PlainFetcher : public ChunkFetcher
	{
	public:
		PlainFetcher(const Gstring& target);

		virtual bool is_locked() override;
		virtual bool unlock(const Gstring& key, const Gstring& nonce) override;
		virtual void lock(const Gstring& key, const Gstring& nonce) override;

		virtual Gbuffer get_asset_data(const Gstring& relPath) override;
		virtual bool save_asset_data(const Gstring& relPath, const Gbuffer& data) override;

		virtual Gstring get_asset_path(const Gstring& relPath) override;

		virtual std::vector<RelativePath> find_assets(const Gstring& identifier, bool recursive) override;
		virtual std::vector<RelativePath> find_entries(const Gstring& relPath) override;

		virtual std::unordered_map<Gstring, Gmapping> update_configs() override;
	};

}