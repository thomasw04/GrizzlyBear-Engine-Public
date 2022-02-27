#pragma once

#include "AssetDataSpecific.h"

namespace GrizzlyBear {

	//TODO: Preprocessor statements are never namespaced, search for alternative
	#define GRIZZLY_ASSET_ERRORCODE_SUCCESS 0
	#define GRIZZLY_ASSET_ERRORCODE_NOT_FOUND 1

	struct AssetLoader
	{
		static Gstring ERROR_TEXTURE_FILEPATH;

		/**Loads a texture from memory, but you need to upload the TextureData to the gpu manually!*/
		static Ref<TextureData> load_texture(const Gstring& filepath, int* errorCode, bool withEnv);
	};

}
