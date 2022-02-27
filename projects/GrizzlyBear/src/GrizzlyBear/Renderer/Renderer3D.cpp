#include <gbpch.h>

#include "Renderer3D.h"

#include "glm/gtc/matrix_transform.hpp"

#include "GrizzlyBear/Core/Application.h"

namespace GrizzlyBear {

	Scope<Renderer3D::SceneData> Renderer3D::s_SceneData = CreateScope<Renderer3D::SceneData>();

	void Renderer3D::init(Ref<GraphicsContext> context)
	{
		s_SceneData->m_Context = context;
	
		s_SceneData->m_VertexArray = context->create_vertex_array();

		s_SceneData->m_BufferLayout = CreateRef<BufferLayout>();

		s_SceneData->m_BufferLayout->add_slot("v_Position", ShaderDataType::Float3);
		s_SceneData->m_BufferLayout->add_slot("v_TexCoords", ShaderDataType::Float2);

		//s_SceneData->m_PlainShader = context->create_shader("engine/engine/shaders/Plain3D.glsl");

		float vertices[5 * 4 * 6] =
		{
			-0.5f, 0.5f, 0, 0, 0,
			-0.5f, -0.5f, 0, 0, 1,
			0.5f, -0.5f, 0, 1, 1,
			0.5f, 0.5f, 0, 1, 0,

			-0.5f, 0.5f, 1, 0, 0,
			-0.5f, -0.5f, 1, 0, 1,
			0.5f, -0.5f, 1, 1, 1,
			0.5f, 0.5f, 1, 1, 0,

			0.5f, 0.5f, 0, 0, 0,
			0.5f, -0.5f, 0, 0, 1,
			0.5f, -0.5f, 1, 1, 1,
			0.5f, 0.5f, 1, 1, 0,

			-0.5f, 0.5f, 0, 0, 0,
			-0.5f, -0.5f, 0, 0, 1,
			-0.5f, -0.5f, 1, 1, 1,
			-0.5f, 0.5f, 1, 1, 0,

			-0.5f, 0.5f, 1, 0, 0,
			-0.5f, 0.5f, 0, 0, 1,
			0.5f, 0.5f, 0, 1, 1,
			0.5f, 0.5f, 1, 1, 0,

			-0.5f, -0.5f, 1, 0, 0,
			-0.5f, -0.5f, 0, 0, 1,
			0.5f, -0.5f, 0, 1, 1,
			0.5f, -0.5f, 1, 1, 0
		};

		uint32_t indices[12 * 3] =
		{
			0, 1, 3,
			3, 1, 2,
			4, 5, 7,
			7, 5, 6,
			8, 9, 11,
			11, 9, 10,
			12, 13, 15,
			15, 13, 14,
			16, 17, 19,
			19, 17, 18,
			20, 21, 23,
			23, 21, 22
		};

		/*
		float vertices[3 * 8]
		{
			1.0, 1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0, 1.0, 1.0,
			1.0, -1.0, 1.0,
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0,
			-1.0, 1.0, 1.0,
			-1.0, -1.0, 1.0
		};

		uint32_t indices[12 * 3]
		{
			5, 3, 1,
			3, 8, 4,
			7, 6, 8,
			2, 8, 6,
			1, 4, 2,
			5, 2, 6,
			5, 7, 3,
			3, 7, 8,
			7, 5, 6,
			2, 4, 8,
			1, 3, 4,
			5, 1, 2
		};*/

		s_SceneData->m_VertexBuffer = context->create_vertex_buffer(vertices, sizeof(vertices), BufferUsage::Static);
		s_SceneData->m_VertexBuffer->set_layout(*s_SceneData->m_BufferLayout);
		s_SceneData->m_VertexArray->add_vertex_buffer(s_SceneData->m_VertexBuffer);

		s_SceneData->m_IndexBuffer = context->create_index_buffer(indices, sizeof(indices), BufferUsage::Static);
		s_SceneData->m_VertexArray->set_index_buffer(s_SceneData->m_IndexBuffer);
	}

	void Renderer3D::on_window_resize(uint32_t width, uint32_t height)
	{
		s_SceneData->m_Context->set_viewport(0, 0, width, height);
	}

	void Renderer3D::begin_scene(const glm::mat4& projection, const glm::mat4& transformation)
	{
		s_SceneData->m_ViewProjection = projection * glm::inverse(transformation);
	}

	void Renderer3D::end_scene()
	{
		//TODO Batch flush
	}

	void Renderer3D::submit(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& size, Ref<Texture> texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), size);

		s_SceneData->m_PlainShader->bind();

		texture->bind(0);

		s_SceneData->m_PlainShader->upload_uniform_mat_4("u_ViewProjection", s_SceneData->m_ViewProjection);
		s_SceneData->m_PlainShader->upload_uniform_mat_4("u_Transform", transform);
		//m_SceneData->m_PlainShader->SetFloat4("u_Color", color);

		s_SceneData->m_VertexArray->bind();
		s_SceneData->m_Context->draw_triangle(s_SceneData->m_VertexArray, 0);
	}

	void Renderer3D::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();

		shader->upload_uniform_mat_4("u_ViewProjection", s_SceneData->m_ViewProjection);
		shader->upload_uniform_mat_4("u_Transform", transform);

		vertexArray->bind();
		s_SceneData->m_Context->draw_triangle(vertexArray, 0);
	}

}
