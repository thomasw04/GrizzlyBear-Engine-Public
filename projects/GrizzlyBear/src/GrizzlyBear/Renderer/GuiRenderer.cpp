#include <gbpch.h>

#include "GuiRenderer.h"

#include "glm/gtc/matrix_transform.hpp"

#include "GrizzlyBear/Asset/AssetManager.h"

namespace GrizzlyBear {

	Scope<GuiRenderer::SceneData> GuiRenderer::s_SceneData = CreateScope<GuiRenderer::SceneData>();

	void GuiRenderer::init(Ref<GraphicsContext> context)
	{
		s_SceneData->m_Context = context;

		s_SceneData->m_VertexArray = context->create_vertex_array();

		s_SceneData->m_BufferLayout = CreateRef<BufferLayout>();

		s_SceneData->m_BufferLayout->add_slot("v_Position", ShaderDataType::Float3);
		s_SceneData->m_BufferLayout->add_slot("v_TexCoords", ShaderDataType::Float2);

		//s_SceneData->m_GuiShader = context->create_shader("engine/engine/shaders/GuiShader.glsl");

		float vertices[5 * 4] =
		{
			-0.5f, 0.5f, 0, 0, 0,
			-0.5f, -0.5f, 0, 0, 1,
			0.5f, -0.5f, 0, 1, 1,
			0.5f, 0.5f, 0, 1, 0,
		};

		uint32_t indices[3 * 2] =
		{
			0, 1, 3,
			3, 1, 2,
		};

		s_SceneData->m_VertexBuffer = context->create_vertex_buffer(vertices, sizeof(vertices), BufferUsage::Static);
		s_SceneData->m_VertexBuffer->set_layout(*s_SceneData->m_BufferLayout);
		s_SceneData->m_VertexArray->add_vertex_buffer(s_SceneData->m_VertexBuffer);

		s_SceneData->m_IndexBuffer = context->create_index_buffer(indices, sizeof(indices), BufferUsage::Static);
		s_SceneData->m_VertexArray->set_index_buffer(s_SceneData->m_IndexBuffer);
	}

	void GuiRenderer::draw_element(const glm::vec2& screenPos, float rotation, const glm::vec2& size, const MaterialProps2D& material)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(screenPos.x, screenPos.y, 1.0f))
			* glm::rotate(glm::mat4(1.0f), rotation, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_SceneData->m_GuiShader->bind();

		//AssetManager::get_texture(material.TEXTURE_FILEPATH)->bind();

		s_SceneData->m_GuiShader->upload_uniform_mat_4("u_Transform", transform);

		s_SceneData->m_VertexArray->bind();
		s_SceneData->m_Context->draw_triangle(s_SceneData->m_VertexArray, 0);
	}

}
