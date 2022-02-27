#pragma once

#include "Renderer.h"

#include "GrizzlyBear/Misc/RenderUtils.h"

#include "GrizzlyBear/Scene/Scene.h"

#include "GrizzlyBear/Asset/AssetManager.h"

namespace GrizzlyBear {

	class Renderer2D
	{
	public:

		enum SceneFlags
		{
			SceneHint_None = 0
		};

		enum class LightType
		{
			POINT = 0, AMBIENT
		};

		struct SceneProps
		{
			glm::mat4 PROJECTION;
			glm::mat4 TRANSFORMATION;
		};

	public:
		Renderer2D(Ref<GraphicsContext> context, RendererFlags flags);

		void load_quad_shader(const Gstring& filepath);
		void load_line_shader(const Gstring& filepath);

		void shutdown();

		//Primitive
		void begin_scene(const glm::mat4& projection, const glm::mat4& transformation);

		void end_scene();

		void draw_point_light(const glm::vec3& position, float intensity, const glm::vec4& color);
		void set_ambient_light(const glm::vec4& color, float intensity);

		void draw_quad(const glm::vec3& position, const float rotation, const glm::vec2& size, const QuadProps& data);
		void draw_quad(const glm::vec2& position, const float rotation, const glm::vec2& size, const QuadProps& data);

		void draw_line(const std::vector<glm::vec3> positions, const glm::vec4& color, float thickness);

		//Complex
		bool render_scene(Timestep ts, const Ref<Scene> scene);
		bool render_scene(Timestep ts, const Ref<Scene> scene, const glm::mat4& camProjection, const glm::mat4& camTransformation);

	private:

		void draw_quad_impl(const glm::vec3& position, const float rotation, const glm::vec2& size, const QuadProps& data, const uint16_t textureID, bool translucent);

		void reserve_texture();

		Ref<VertexBuffer> create_quad_vertex_buffer();
		Ref<IndexBuffer> create_quad_index_buffer();
		Ref<ShaderBuffer> create_quad_shader_buffer();

		Ref<VertexBuffer> create_line_vertex_buffer();

		//Legacy
		void begin_quad_batch();
		uint32_t flush_opaque_quad(uint32_t begin);
		uint32_t flush_translucent_quad(uint32_t begin);
		void end_quad_batch();

		void begin_line_batch();
		uint32_t flush_line(uint32_t begin);
		void end_line_batch();

	private:
		Ref<GraphicsContext> m_Context;
		Ref<AssetManager> m_AssetManager;

		//Assets
		Ref<Shader> m_QuadShader;
		Ref<Shader> m_LineShader;

		Ref<Texture2D> m_WhiteTexture;

		//Render Stats
		uint32_t m_BuffersPerDraw = 0;

		uint32_t m_MaxQuadsPerBuffer = 10000;
		uint32_t m_MaxLinesPerBuffer = 40000;

		uint32_t m_CurrentQuads = 0;
		uint32_t m_CurrentLines = 0;

		uint32_t m_CurrentLights = 0;

		//GL Buffers
		Ref<BufferLayout> m_QuadBufferLayout;

		Ref<VertexArray> m_aBuffer;
		Ref<VertexBuffer> m_vBuffer;
		Ref<IndexBuffer> m_iBuffer;

		Ref<VertexArray> m_StatABuffer;
		Ref<VertexBuffer> m_StatVBuffer;
		Ref<IndexBuffer> m_StatIBuffer;

		Ref<ShaderBuffer> m_sBuffer;
		Ref<ShaderBuffer> m_sLightsBuffer;

		//Intermediate Buffers
		VertexCollector<QuadVertex> m_OpaqueCollector = VertexCollector<QuadVertex>(Geometry::QUADS);
		VertexCollector<QuadVertex> m_TranslucentCollector = VertexCollector<QuadVertex>(Geometry::QUADS);

		BasicCollector<QuadSSBO> m_OpaqueQuadAttributes = BasicCollector<QuadSSBO>();
		BasicCollector<QuadSSBO> m_TranslucentQuadAttributes = BasicCollector<QuadSSBO>();

		BasicCollector<LightSSBO> m_Lights = BasicCollector<LightSSBO>();

		glm::vec4 m_AmbientColor = {1.0f, 1.0f, 1.0f, 1.0f};
		float m_AmbientIntensity = 1.0f;

		std::unordered_map<Ref<Texture>, int> m_TextureIDs;
		int m_OpaqueTextureIDs[10000];
		int m_TranslucentTextureIDs[10000];

		//Lines

		Ref<BufferLayout> m_LineBufferLayout;
		Ref<VertexArray> m_LineABuffer;
		Ref<VertexBuffer> m_LineVBuffer;

		VertexCollector<LineVertex> m_LineCollector = VertexCollector<LineVertex>(Geometry::LINES);


		RendererFlags m_Flags;
	};

}
