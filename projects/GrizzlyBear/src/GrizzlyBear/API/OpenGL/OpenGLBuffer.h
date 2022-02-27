#pragma once

#include "GrizzlyBear/API/GGL/Buffer.h"


namespace GrizzlyBear {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage resourceUsage);

		virtual ~OpenGLVertexBuffer();

		virtual void set_sub_data(const void* vertices, uint32_t offset, uint32_t size) override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const BufferLayout& get_layout() const override { return m_Layout; }
		virtual void set_layout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count, BufferUsage resourceUsage);

		virtual ~OpenGLIndexBuffer();

		virtual void set_sub_data(uint32_t* indices, uint32_t offset, uint32_t count) override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t get_count() const override { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLRenderBuffer : public RenderBuffer
	{
	public:
		OpenGLRenderBuffer(int width, int height, int internalFormat);

		virtual ~OpenGLRenderBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t get_width() const override { return m_Width; };
		virtual uint32_t get_height() const override { return m_Height; };

		virtual uint32_t get_render_id() const override { return m_RendererID; };

		virtual int get_internal_format() const override { return m_InternalFormat; };

	private:
		uint32_t m_RendererID;
		int m_InternalFormat;

		uint32_t m_Width;
		uint32_t m_Height;
	};

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(int width, int height);

		virtual ~OpenGLFrameBuffer();

		virtual void bind() const override;
		virtual void unbind(int resetWidth, int resetHeight) const override;

		virtual int get_width() const override { return m_Width; }
		virtual int get_height() const override { return m_Height; }

		virtual void attach_texture_2d(Ref<Texture2D> texture, int slot) override;

		virtual void attach_render_buffer(Ref<RenderBuffer> buffer, int internalFormat) override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Width;
		uint32_t m_Height;
	private:
		uint32_t m_RenderBufferID;
	};

	class OpenGLShaderBuffer : public ShaderBuffer
	{
	public:
		OpenGLShaderBuffer(float* data, uint32_t slot, uint32_t size, BufferUsage resourceUsage);

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void set_sub_data(const void* data, uint32_t offset, uint32_t size) override;
		virtual void set_binding(uint32_t slot) override;

	private:
		uint32_t m_RendererID;
	};

}
