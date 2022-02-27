#include <gbpch.h>

#include "AssetLoader.h"

//Environment base module
#include "GrizzlyBear/Environment/Environment.h"

namespace GrizzlyBear {

	Gstring AssetLoader::ERROR_TEXTURE_FILEPATH = "engine/engine/textures/ErrorTexture.png";

	Ref<TextureData> AssetLoader::load_texture(const Gstring& filepath, int* errorCode, bool withEnv)
	{
		stbi_uc* data;

		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		Gbuffer buffer;

		if (withEnv)
		{
			buffer = Environment::get_fetcher()->get_asset_file_raw(filepath);
		}
		else
		{
			FilePath path("");

			if (filepath.substr(0, 7) == "engine/")
			{
				path = FilePath(FileIO::get_program_directory().get_path() + filepath.substr(7));
			}
			else
			{
				path = FilePath(FileIO::get_program_directory().get_path() + filepath);
			}

			buffer = FileIO::get_file_content(path.get_path());
		}

		data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(buffer.m_Buffer.c_str()), (int)buffer.m_Buffer.size(), &width, &height, &channels, 0);

		*errorCode = GRIZZLY_ASSET_ERRORCODE_SUCCESS;

		Gstring errorTexture = ERROR_TEXTURE_FILEPATH;

		if (data == nullptr)
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Failed to load texture: {0}", filepath.c_str());

			stbi_image_free(data);

			//Get the asset from the environment (filepath points not necessarily to a file, the data could also be inside a gchunk)
			Gbuffer errorbuffer = Environment::get_fetcher()->get_asset_file_raw(errorTexture);
			data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(errorbuffer.m_Buffer.c_str()), (int)errorbuffer.m_Buffer.size(), &width, &height, &channels, 0);

			*errorCode = GRIZZLY_ASSET_ERRORCODE_NOT_FOUND;
		}

		GRIZZLY_CORE_ASSERT(data, "Failed to load ERROR-TEXTURE! This engine is corrupted please load a other engine build!");

		return std::make_shared<TextureData>(data, width, height, channels);
	}
}
