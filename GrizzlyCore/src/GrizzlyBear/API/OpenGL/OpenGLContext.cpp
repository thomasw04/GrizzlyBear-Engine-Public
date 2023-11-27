#include <gbpch.h>
#include "OpenGLContext.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>


#include "OpenGLBase.h"

#include "OpenGLVertexArray.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"

#include "OpenGLStatistics.h"

namespace GrizzlyBear {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		GRIZZLY_CORE_ASSERT(windowHandle, "Error: Window handle is null!")

		glfwMakeContextCurrent(m_WindowHandle);
		GLenum status = glewInit();
		GRIZZLY_CORE_ASSERT(status, "Failed to initialize Glad!");

		GRIZZLY_CORE_LOG(LogLevel::Info, "OpenGL Info: ");
		GRIZZLY_CORE_LOG(LogLevel::Info, "  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		GRIZZLY_CORE_LOG(LogLevel::Info, "  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		GRIZZLY_CORE_LOG(LogLevel::Info, "  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		int versionMajor;
		int versionMinor;
		GL_VALIDATE(glGetIntegerv(GL_MAJOR_VERSION, &versionMajor));
		GL_VALIDATE(glGetIntegerv(GL_MINOR_VERSION, &versionMinor));

		GRIZZLY_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "OpenGL version 4.5 is required!");

		GL_VALIDATE(glEnable(GL_BLEND));
		GL_VALIDATE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GL_VALIDATE(glEnable(GL_DEPTH_TEST));
	}

	void OpenGLContext::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GL_VALIDATE(glViewport(x, y, width, height));
	}

	void OpenGLContext::enable_wire_mode(float lineWidth)
	{
		GL_VALIDATE(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

		GL_VALIDATE(glEnable(GL_LINE_SMOOTH));

		GL_VALIDATE(glLineWidth(lineWidth));
	}

	void OpenGLContext::disable_wire_mode()
	{
		GL_VALIDATE(glDisable(GL_LINE_SMOOTH));

		GL_VALIDATE(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	void OpenGLContext::set_clear_color(const glm::vec4& color)
	{
		GL_VALIDATE(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLContext::clear()
	{
		GL_VALIDATE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	
	void OpenGLContext::draw_triangle(const Ref<VertexArray> vertexArray, uint32_t count)
	{
		if (count == 0) count = vertexArray->get_index_buffer()->get_count();

		GL_VALIDATE(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLContext::draw_point(const Ref<VertexArray> vertexArray, uint32_t count)
	{
		if (count == 0) count = vertexArray->get_index_buffer()->get_count();

		GL_VALIDATE(glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLContext::draw_line(const Ref<VertexArray> vertexArray, uint32_t count)
	{
		GL_VALIDATE(glDrawArrays(GL_LINES, 0, count));
	}

	void OpenGLContext::draw_framebuffer(const Ref<FrameBuffer> buffer)
	{
		int width, height;
		glfwGetWindowSize(m_WindowHandle, &width, &height);

		GL_VALIDATE(glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer->get_render_id()));
		GL_VALIDATE(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		GL_VALIDATE(glBlitFramebuffer(0, 0, buffer->get_width(), buffer->get_height(), 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	}

	Ref<VertexBuffer> OpenGLContext::create_vertex_buffer(float* vertices, uint32_t size, BufferUsage resourceUsage)
	{
		return std::make_shared<OpenGLVertexBuffer>(vertices, size, resourceUsage);
	}

	Ref<IndexBuffer> OpenGLContext::create_index_buffer(uint32_t* indices, uint32_t count, BufferUsage resourceUsage)
	{
		return std::make_shared<OpenGLIndexBuffer>(indices, count, resourceUsage);
	}

	Ref<RenderBuffer> OpenGLContext::create_render_buffer(int width, int height, int internalFormat)
	{
		return std::make_shared<OpenGLRenderBuffer>(width, height, internalFormat, 1);
	}

	Ref<FrameBuffer> OpenGLContext::create_frame_buffer(int width, int height)
	{
		return std::make_shared<OpenGLFrameBuffer>(width, height);
	}

	Ref<ShaderBuffer> OpenGLContext::create_shader_buffer(float* data, uint32_t slot, uint32_t size, BufferUsage resourceUsage)
	{
		return std::make_shared<OpenGLShaderBuffer>(data, slot, size, resourceUsage);
	}

	Ref<Shader> OpenGLContext::create_shader(const Gstring& name, const Gbuffer& buffer)
	{
		return std::make_shared<OpenGLShader>(name, buffer);
	}

	Ref<Shader> OpenGLContext::create_shader(const Gstring& name, std::initializer_list<std::reference_wrapper<Gbuffer>> sources)
	{
		return std::make_shared<OpenGLShader>(name, sources);
	}

	Ref<Texture2D> OpenGLContext::create_texture_2d(uint32_t width, uint32_t height)
	{
		return std::make_shared<OpenGLTexture2D>(width, height);
	}

	Ref<Texture2D> OpenGLContext::create_texture_2d(Ref<AssetData> data)
	{
		return std::make_shared<OpenGLTexture2D>(data);
	}

	Ref<Statistics> OpenGLContext::get_statistics()
	{
		if (GraphicsContext::m_Statistics == nullptr)
		{
			return (GraphicsContext::m_Statistics = std::make_shared<OpenGLStatistics>());
		}

		return GraphicsContext::m_Statistics;
	}

	Ref<VertexArray> OpenGLContext::create_vertex_array()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

}
