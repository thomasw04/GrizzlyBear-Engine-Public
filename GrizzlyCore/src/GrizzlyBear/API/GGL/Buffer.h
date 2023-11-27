#pragma once

#include "Texture.h"
#include "Shader.h"

namespace GrizzlyBear {

    #define GRIZZLY_INTERNAL_DEPTH24 0

	enum class BufferUsage
	{
		Static = 0, Dynamic = 2, Stream = 3
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		void add_slot(const char* name, ShaderDataType type, bool normalized = false) { m_Elements.push_back({ type, normalized }); }

		const std::vector<std::pair<ShaderDataType, bool>>& get_elements() const { return m_Elements; }

		std::vector<std::pair<ShaderDataType, bool>>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<std::pair<ShaderDataType, bool>>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<std::pair<ShaderDataType, bool>> m_Elements;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void set_sub_data(const void* vertices, uint32_t offset, uint32_t size) = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const BufferLayout& get_layout() const = 0;
		virtual void set_layout(const BufferLayout& layout) = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void set_sub_data(uint32_t* indices, uint32_t offset, uint32_t count) = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_count() const = 0;
	};

	class RenderBuffer
	{
	public:
		virtual ~RenderBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		virtual int get_internal_format() const = 0;

		virtual uint32_t get_render_id() const = 0;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual int get_width() const = 0;
		virtual int get_height() const = 0;

		virtual void attach_texture_2d(Ref<Texture2D> texture, int slot) = 0;

		virtual void attach_render_buffer(Ref<RenderBuffer> buffer, int internalFormat) = 0;

		virtual uint32_t get_render_id() const = 0;
	};

	class ShaderBuffer
	{
	public:
		virtual ~ShaderBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void set_sub_data(const void* data, uint32_t offset, uint32_t size) = 0;
		virtual void set_binding(uint32_t slot) = 0;
	};

	class TextureBuffer
	{
	public:
		virtual ~TextureBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};
}
