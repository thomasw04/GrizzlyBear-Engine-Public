#pragma once

#include "VertexArray.h"
#include "Buffer.h"
#include "Texture.h"
#include "Shader.h"

#include "Statistics.h"

namespace GrizzlyBear {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void enable_wire_mode(float lineWidth) = 0;
		virtual void disable_wire_mode() = 0;

		virtual void set_clear_color(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void draw_point(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;
		virtual void draw_line(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;
		virtual void draw_triangle(const Ref<VertexArray>& vertexArray, uint32_t count) = 0;

		virtual Ref<VertexBuffer> create_vertex_buffer(float* vertices, uint32_t size, BufferUsage resourceUsage = BufferUsage::Static) = 0;
		virtual Ref<IndexBuffer> create_index_buffer(uint32_t* indices, uint32_t count, BufferUsage resourceUsage = BufferUsage::Static) = 0;
		virtual Ref<RenderBuffer> create_render_buffer(int width, int height, int internalFormat = GRIZZLY_INTERNAL_DEPTH24) = 0;
		virtual Ref<FrameBuffer> create_frame_buffer(int width, int height) = 0;
		virtual Ref<ShaderBuffer> create_shader_buffer(float* data, uint32_t slot, uint32_t size, BufferUsage resourceUsage = BufferUsage::Static) = 0;

		virtual Ref<Shader> create_shader(const Gstring& name, const Gbuffer& buffer) = 0;
		virtual Ref<Shader> create_shader(const Gstring& name, std::initializer_list<std::reference_wrapper<Gbuffer>> sources) = 0;

		virtual Ref<Texture2D> create_texture_2d(uint32_t width, uint32_t height) = 0;
		virtual Ref<Texture2D> create_texture_2d(Ref<AssetData> data) = 0;

		virtual Ref<Statistics> get_statistics() = 0;

		virtual Ref<VertexArray> create_vertex_array() = 0;

	protected:
		Ref<Statistics> m_Statistics = nullptr;
	};
}
