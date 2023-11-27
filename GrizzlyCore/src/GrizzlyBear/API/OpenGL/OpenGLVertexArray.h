#pragma once

#include "GrizzlyBear/API/GGL/Shader.h"
#include "GrizzlyBear/API/GGL/VertexArray.h"


namespace GrizzlyBear {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void add_vertex_buffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void set_index_buffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& get_vertex_buffers() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& get_index_buffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}
