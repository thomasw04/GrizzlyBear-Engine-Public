#include <gbpch.h>

#include "ProductionFileFetcher.h"

namespace GrizzlyBear {

    //TODO

    Gbuffer ProductionFileFetcher::get_asset_file_raw(const Gstring& identifier)
    {
        return Gbuffer();
    }

    Gstring ProductionFileFetcher::get_full_asset_path(const Gstring& path)
    {
        return Gstring();
    }

    bool ProductionFileFetcher::save_asset_file(const Gstring& path, const Gbuffer& buffer)
    {
        return false;
    }

    std::vector<RelativePath> ProductionFileFetcher::get_files_in_dir(const Gstring& path, bool recursive)
    {
        return std::vector<RelativePath>();
    }

    std::vector<RelativePath> ProductionFileFetcher::get_things_in_dir(const Gstring& path)
    {
        return std::vector<RelativePath>();
    }

    void ProductionFileFetcher::update_configs()
    {
    }

}