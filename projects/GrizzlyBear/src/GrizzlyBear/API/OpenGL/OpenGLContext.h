#pragma once
#include "GrizzlyBear/API/GGL/GraphicsContext.h"

struct GLFWwindow;

namespace GrizzlyBear {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void enable_wire_mode(float lineWidth) override;
		virtual void disable_wire_mode() override;

		virtual void set_clear_color(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void draw_point(const Ref<VertexArray>& vertexArray, uint32_t count) override;
		virtual void draw_line(const Ref<VertexArray>& vertexArray, uint32_t count) override;
		virtual void draw_triangle(const Ref<VertexArray>& vertexArray, uint32_t count) override;

		virtual Ref<VertexBuffer> create_vertex_buffer(float* vertices, uint32_t size, BufferUsage resourceUsage = BufferUsage::Static) override;
		virtual Ref<IndexBuffer> create_index_buffer(uint32_t* indices, uint32_t count, BufferUsage resourceUsage = BufferUsage::Static) override;
		virtual Ref<RenderBuffer> create_render_buffer(int width, int height, int internalFormat = GRIZZLY_INTERNAL_DEPTH24) override;
		virtual Ref<FrameBuffer> create_frame_buffer(int width, int height) override;
		virtual Ref<ShaderBuffer> create_shader_buffer(float* data, uint32_t slot, uint32_t size, BufferUsage resourceUsage = BufferUsage::Static) override;

		virtual Ref<Shader> create_shader(const Gstring& name, const Gbuffer& buffer) override;
		virtual Ref<Shader> create_shader(const Gstring& name, std::initializer_list<std::reference_wrapper<Gbuffer>> sources) override;

		virtual Ref<Texture2D> create_texture_2d(uint32_t width, uint32_t height) override;
		virtual Ref<Texture2D> create_texture_2d(Ref<AssetData> data) override;

		virtual Ref<Statistics> get_statistics() override;

		virtual Ref<VertexArray> create_vertex_array() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
