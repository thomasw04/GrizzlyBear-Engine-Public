#pragma once

#include "AssetData.h"

#include "stb_image.h"

namespace GrizzlyBear {
	
	class TexColumn
	{
		friend struct TextureData;

		TexColumn(stbi_uc* data, uint32_t width, uint32_t height)
			: m_Data(data), m_Width(width), m_Height(height) {}

		stbi_uc* m_Data;
		uint32_t m_Width;
		uint32_t m_Height;

	public:
		stbi_uc* operator[](int index)
		{
			if ((uint32_t)index > m_Height - 1 || m_Data == nullptr) return nullptr;

			return (stbi_uc*)(m_Data + (m_Width * index));
		}
	};

	struct TextureData : public AssetData
	{
		TextureData(stbi_uc* data, uint32_t width, uint32_t height, uint32_t channels)
			: m_Data(data), m_Width(width), m_Height(height), m_Channels(channels) {}

		TextureData(uint32_t width, uint32_t height, uint32_t channels)
		{
			m_Data = static_cast<stbi_uc*>(std::malloc(width * height * channels));

			m_Width = width;
			m_Height = height;
			m_Channels = channels;
		}

		~TextureData() { stbi_image_free(m_Data); }

		TexColumn operator[](int index)
		{
			if ((uint32_t)index > m_Width - 1) return TexColumn(nullptr, m_Width, m_Height);

			return TexColumn((stbi_uc*)(m_Data + index), m_Width, m_Height);
		}

		stbi_uc* m_Data;

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Channels;
	};
}
