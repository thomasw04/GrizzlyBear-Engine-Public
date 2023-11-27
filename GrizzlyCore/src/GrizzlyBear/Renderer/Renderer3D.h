#pragma once

#include "Renderer.h"

namespace GrizzlyBear {

	class Renderer3D
	{
	public:
		static void init(Ref<GraphicsContext> context);
		static void on_window_resize(uint32_t width, uint32_t height);

		static void begin_scene(const glm::mat4& projection, const glm::mat4& transformation);
		static void end_scene();

		static void submit(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& size, Ref<Texture> texture);

		static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

	private:

		struct SceneData
		{
			Ref<GraphicsContext> m_Context;

			glm::mat4 m_ViewProjection;

			Ref<BufferLayout> m_BufferLayout;

			Ref<VertexArray> m_VertexArray;
			Ref<VertexBuffer> m_VertexBuffer;
			Ref<IndexBuffer> m_IndexBuffer;

			Ref<Shader> m_PlainShader;
		};

		static Scope<SceneData> s_SceneData;
	};
}
