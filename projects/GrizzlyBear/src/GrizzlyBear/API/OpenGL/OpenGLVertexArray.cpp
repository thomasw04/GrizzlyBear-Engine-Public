#include <gbpch.h>

#include "OpenGLVertexArray.h"

#include "OpenGLBase.h"

#include <glad/glad.h>


namespace GrizzlyBear {

	static GLenum get_opengl_type(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:   break;
			case ShaderDataType::Float:	 return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Mat3:	 return GL_FLOAT;
			case ShaderDataType::Mat4:	 return GL_FLOAT;
			case ShaderDataType::Int:	 return GL_INT;
			case ShaderDataType::Int2:	 return GL_INT;
			case ShaderDataType::Int3:	 return GL_INT;
			case ShaderDataType::Int4:	 return GL_INT;
			case ShaderDataType::Bool:	 return GL_BOOL;
		}

		GRIZZLY_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static uint32_t get_prim_count(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:   return 0;
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3:   return 9;
			case ShaderDataType::Mat4:   return 16;
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
			case ShaderDataType::Bool:   return 1;
			default: return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GL_VALIDATE(glCreateVertexArrays(1, &m_RendererID));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GL_VALIDATE(glDeleteVertexArrays(1, &m_RendererID));
	}

	void OpenGLVertexArray::bind() const
	{
		GL_VALIDATE(glBindVertexArray(m_RendererID));
	}

	void OpenGLVertexArray::unbind() const
	{
		GL_VALIDATE(glBindVertexArray(0));
	}

	void OpenGLVertexArray::add_vertex_buffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		GRIZZLY_CORE_ASSERT(vertexBuffer->get_layout().get_elements().size(), "Vertex Buffer has no layout!");

		GL_VALIDATE(glBindVertexArray(m_RendererID));
		vertexBuffer->bind();

		const auto& layout = vertexBuffer->get_layout();

		uint32_t stride = 0;
		for (const auto& element : layout)
		{
			stride += get_prim_count(element.first) * 4;
		}

		uint32_t index = 0;
		uint32_t offset = 0;
		for (const auto& element : layout)
		{
			GL_VALIDATE(glEnableVertexAttribArray(index));
			GL_VALIDATE(glVertexAttribPointer(index, get_prim_count(element.first), get_opengl_type(element.first), element.second ? GL_TRUE : GL_FALSE, stride, (const void*)(uintptr_t)offset));
			offset += get_prim_count(element.first) * 4;
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::set_index_buffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GL_VALIDATE(glBindVertexArray(m_RendererID));
		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;
	}
}
