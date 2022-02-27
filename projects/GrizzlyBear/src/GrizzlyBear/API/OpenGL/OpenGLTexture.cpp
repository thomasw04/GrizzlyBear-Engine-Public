#include <gbpch.h>

#include "OpenGLTexture.h"

#include "OpenGLBase.h"

#include "GrizzlyBear/Asset/AssetDataSpecific.h"

namespace GrizzlyBear
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height), m_Channels(4)
	{
		m_InternalFormat = GL_RGBA8;
		m_Format = GL_RGBA;

		GL_VALIDATE(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
		GL_VALIDATE(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height));

		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}

	OpenGLTexture2D::OpenGLTexture2D(Ref<AssetData> idata)
	{
		Ref<TextureData> data = std::dynamic_pointer_cast<TextureData>(idata);

		GLenum internalFormat = 0, dataFormat = 0;

		if (data->m_Channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (data->m_Channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_Format = dataFormat;

		GRIZZLY_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		GL_VALIDATE(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
		GL_VALIDATE(glTextureStorage2D(m_RendererID, 1, internalFormat, data->m_Width, data->m_Height));

		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_VALIDATE(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GL_VALIDATE(glTextureSubImage2D(m_RendererID, 0, 0, 0, data->m_Width, data->m_Height, dataFormat, GL_UNSIGNED_BYTE, data->m_Data));

		m_Width = data->m_Width;
		m_Height = data->m_Height;
		m_Channels = data->m_Channels;
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GL_VALIDATE(glDeleteTextures(1, &m_RendererID));
	}

	void OpenGLTexture2D::set_data(void* data, uint32_t size)
	{
		uint32_t bpp = m_Format == GL_RGBA ? 4 : 3;
		GRIZZLY_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be as big as the entire texture!");
		GL_VALIDATE(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data));
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		GL_VALIDATE(glBindTextureUnit(slot, m_RendererID));
	}

}
