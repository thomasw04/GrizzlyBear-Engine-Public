#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"

#include "GrizzlyBear/Scene/Components/BaseTypes.h"

namespace GrizzlyBear {

	class GuiRenderer
	{
	public:
		static void init(Ref<GraphicsContext> context);

		static void draw_element(const glm::vec2& screenPos, float rotation, const glm::vec2& size, const MaterialProps2D& material);

	private:
		struct SceneData
		{
			Ref<GraphicsContext> m_Context;

			Ref<BufferLayout> m_BufferLayout;

			Ref<VertexArray> m_VertexArray;
			Ref<VertexBuffer> m_VertexBuffer;
			Ref<IndexBuffer> m_IndexBuffer;

			Ref<Shader> m_GuiShader;
		};

		static Scope<SceneData> s_SceneData;
	};

}
