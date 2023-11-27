#include <gbpch.h>

#include "OpenGLBuffer.h"

#include "OpenGLBase.h"

#include <gl/glew.h>

namespace GrizzlyBear {

	//VertexBuffer -----------------------------------------------------------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage resourceUsage)
	{
		GL_VALIDATE(glCreateBuffers(1, &m_RendererID));
		GL_VALIDATE(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

		if (resourceUsage == BufferUsage::Static)
		{
			GL_VALIDATE(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
		}
		else if (resourceUsage == BufferUsage::Dynamic)
		{
			GL_VALIDATE(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW));
		}
		else if (resourceUsage == BufferUsage::Stream)
		{
			GL_VALIDATE(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW));
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GL_VALIDATE(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLVertexBuffer::set_sub_data(const void* vertices, uint32_t offset, uint32_t size)
	{
		GL_VALIDATE(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GL_VALIDATE(glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices));
	}

	void OpenGLVertexBuffer::bind() const
	{
		GL_VALIDATE(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void OpenGLVertexBuffer::unbind() const
	{
		GL_VALIDATE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	//IndexBuffer ------------------------------------------------------------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count, BufferUsage resourceUsage)
		: m_Count(count)
	{
		GL_VALIDATE(glCreateBuffers(1, &m_RendererID));
		GL_VALIDATE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

		if (resourceUsage == BufferUsage::Static)
		{
			GL_VALIDATE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
		}
		else if (resourceUsage == BufferUsage::Dynamic)
		{
			GL_VALIDATE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW));
		}
		else if (resourceUsage == BufferUsage::Stream)
		{
			GL_VALIDATE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STREAM_DRAW));
		}
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GL_VALIDATE(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLIndexBuffer::set_sub_data(uint32_t* indices, uint32_t offset, uint32_t count)
	{
		GL_VALIDATE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GL_VALIDATE(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(uint32_t), indices));
	}

	void OpenGLIndexBuffer::bind() const
	{
		GL_VALIDATE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void OpenGLIndexBuffer::unbind() const
	{
		GL_VALIDATE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	//RenderBuffer ------------------------------------------------------------------------

	OpenGLRenderBuffer::OpenGLRenderBuffer(int width, int height, int internalFormat, int samples)
		: m_RendererID(0), m_InternalFormat(internalFormat), m_Width(width), m_Height(height)
	{
		GL_VALIDATE(glCreateRenderbuffers(1, &m_RendererID));
		GL_VALIDATE(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));

		if (internalFormat == GRIZZLY_INTERNAL_DEPTH24)
		{
			GL_VALIDATE(glRenderbufferStorageMultisample(GL_RENDERBUFFER, std::min(samples, GL_MAX_SAMPLES), GL_DEPTH_COMPONENT24, width, height));
		}
	}

	OpenGLRenderBuffer::~OpenGLRenderBuffer()
	{
		GL_VALIDATE(glDeleteRenderbuffers(1, &m_RendererID));
	}

	void OpenGLRenderBuffer::bind() const
	{
		GL_VALIDATE(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
	}

	void OpenGLRenderBuffer::unbind() const
	{
		GL_VALIDATE(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	//FrameBuffer ------------------------------------------------------------------------

	OpenGLFrameBuffer::OpenGLFrameBuffer(int width, int height)
		: m_RendererID(0), m_Width(width), m_Height(height), m_RenderBufferID(0)
	{
		GL_VALIDATE(glCreateFramebuffers(1, &m_RendererID));
		GL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		GL_VALIDATE(glDeleteRenderbuffers(1, &m_RenderBufferID));
		GL_VALIDATE(glDeleteFramebuffers(1, &m_RendererID));
	}

	void OpenGLFrameBuffer::bind() const
	{
		GL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	}

	void OpenGLFrameBuffer::unbind() const
	{
		GRIZZLY_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

		GL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFrameBuffer::attach_texture_2d(Ref<Texture2D> texture, int slot)
	{
		GL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

		GL_VALIDATE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, texture->get_render_id(), 0));

		GRIZZLY_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Something went wrong on attachment creation");
	}

	void OpenGLFrameBuffer::attach_render_buffer(Ref<RenderBuffer> buffer, int internalFormat)
	{
		GL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

		//glCreateRenderbuffers(1, &m_RenderBufferID);
		//glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);

		if (internalFormat == GRIZZLY_INTERNAL_DEPTH24)
		{
			//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height);
			GL_VALIDATE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->get_render_id()));
		}

		GRIZZLY_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Something went wrong on attachment creation");
	}

	//ShaderBuffer-----------------------------------------------------------------------------------------

	OpenGLShaderBuffer::OpenGLShaderBuffer(float* data, uint32_t slot, uint32_t size, BufferUsage resourceUsage)
	{
		GL_VALIDATE(glGenBuffers(1, &m_RendererID));
		GL_VALIDATE(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
		
		if (resourceUsage == BufferUsage::Static)
		{
			GL_VALIDATE(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW));
		}
		else if (resourceUsage == BufferUsage::Dynamic)
		{
			GL_VALIDATE(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW));
		}
		else if (resourceUsage == BufferUsage::Stream)
		{
			GL_VALIDATE(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STREAM_DRAW));
		}

		GL_VALIDATE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID));
	}


	void OpenGLShaderBuffer::bind() const
	{
		GL_VALIDATE(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
	}

	void OpenGLShaderBuffer::unbind() const
	{
		GL_VALIDATE(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}

	void OpenGLShaderBuffer::set_sub_data(const void* data, uint32_t offset, uint32_t size)
	{
		GL_VALIDATE(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
		GL_VALIDATE(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data));
	}

	void OpenGLShaderBuffer::set_binding(uint32_t slot)
	{
		GL_VALIDATE(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
		GL_VALIDATE(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID));
	}

}
