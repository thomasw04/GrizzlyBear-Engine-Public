#pragma once

#include "FileFetcher.h"

#include "GProjectParser.h"

namespace GrizzlyBear {

	class ProductionFileFetcher : public FileFetcher
	{
	public:
		ProductionFileFetcher(const Gstring& projectDir, Ref<GProject> prj)
			: FileFetcher(projectDir, prj) {}

		virtual Gbuffer get_asset_file_raw(const Gstring& identifier) override;

		virtual Gstring get_full_asset_path(const Gstring& path) override;

		virtual bool save_asset_file(const Gstring& path, const Gbuffer& buffer) override;

		virtual std::vector<RelativePath> get_files_in_dir(const Gstring& path, bool recursive) override;
		virtual std::vector<RelativePath> get_things_in_dir(const Gstring& path) override;

		virtual void update_configs() override;
	};

}