#pragma once

#include "GrizzlyBear/API/GGL/Texture.h"

#include <gl/glew.h>


namespace GrizzlyBear {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(Ref<AssetData> data);
		virtual ~OpenGLTexture2D();

		virtual uint32_t get_width() const override { return m_Width; }
		virtual uint32_t get_height() const override { return m_Height; }

		virtual uint32_t get_render_id() const override { return m_RendererID; }
		virtual uint32_t get_channels() const override { return m_Channels; }

		virtual void set_data(void* data, uint32_t size) override;

		virtual void bind(uint32_t slot = 0) const override;

	private:
		uint32_t m_RendererID;

		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Channels;

		GLenum m_InternalFormat, m_Format;
	};

}
