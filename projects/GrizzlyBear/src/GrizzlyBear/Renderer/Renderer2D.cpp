#include <gbpch.h>

#include "Renderer2D.h"

#include "GrizzlyBear/Core/Application.h"
#include "GrizzlyBear/Environment/Environment.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

namespace GrizzlyBear {

	Renderer2D::Renderer2D(Ref<GraphicsContext> context, RendererFlags flags)
	{
		m_Context = context;

		m_Flags = flags;

		m_aBuffer = m_Context->create_vertex_array();

		m_QuadBufferLayout = CreateRef<BufferLayout>();

		m_QuadBufferLayout->add_slot("a_Identifier", ShaderDataType::Float);
		m_QuadBufferLayout->add_slot("a_Position", ShaderDataType::Float3);
		m_QuadBufferLayout->add_slot("a_TexCoord", ShaderDataType::Float2);

		m_vBuffer = create_quad_vertex_buffer();
		m_iBuffer = create_quad_index_buffer();
		m_sBuffer = create_quad_shader_buffer();

		m_sLightsBuffer = m_Context->create_shader_buffer(nullptr, 2, sizeof(LightSSBO) * 128, BufferUsage::Dynamic);

		if (!(m_Flags & RendererHint_NoDefaultShader))
		{
			load_quad_shader("engine/engine/shaders/Quad.glsl");
			load_line_shader("engine/engine/shaders/Line.glsl");
		}

		m_WhiteTexture = m_Context->create_texture_2d(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_WhiteTexture->set_data(&whiteTextureData, sizeof(uint32_t));

		m_LineABuffer = m_Context->create_vertex_array();

		m_LineBufferLayout = CreateRef<BufferLayout>();

		m_LineBufferLayout->add_slot("a_Identifier", ShaderDataType::Float);
		m_LineBufferLayout->add_slot("a_Position", ShaderDataType::Float3);
		m_LineBufferLayout->add_slot("a_Color", ShaderDataType::Float4);
		m_LineBufferLayout->add_slot("a_Thickness", ShaderDataType::Float);

		m_LineVBuffer = create_line_vertex_buffer();
	}

	void Renderer2D::load_quad_shader(const Gstring& filepath)
	{
		Gbuffer buffer;

		if (m_Flags & RendererHint_NoEnvironment)
		{
			FilePath path("");

			if (filepath.substr(0, 7) == "engine/")
			{
				path = FilePath(FileIO::get_program_directory().get_path() + filepath.substr(7));
			}
			else
			{
				path = FilePath(FileIO::get_program_directory().get_path() + filepath);
			}

			buffer = FileIO::get_file_content(path.get_path());
		}
		else
		{
			buffer = Environment::get_fetcher()->get_asset_file_raw(filepath);
		}

		m_QuadShader = m_Context->create_shader(FilePath(filepath).get_name_woe(), buffer);
	}

	void Renderer2D::load_line_shader(const Gstring& filepath)
	{
		Gbuffer buffer;

		if (m_Flags & RendererHint_NoEnvironment)
		{
			FilePath path("");

			if (filepath.substr(0, 7) == "engine/")
			{
				path = FilePath(FileIO::get_program_directory().get_path() + filepath.substr(7));
			}
			else
			{
				path = FilePath(FileIO::get_program_directory().get_path() + filepath);
			}

			buffer = FileIO::get_file_content(path.get_path());
		}
		else
		{
			buffer = Environment::get_fetcher()->get_asset_file_raw(filepath);
		}

		m_LineShader = m_Context->create_shader(FilePath(filepath).get_name_woe(), buffer);
	}

	void Renderer2D::shutdown()
	{
	}

	void Renderer2D::begin_scene(const glm::mat4& projection, const glm::mat4& transformation)
	{
		if (m_QuadShader == nullptr)
		{
			GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Renderer2D, Message: No shader set!");
			return;
		}

		glm::mat4 viewProjectionMatrix = projection * glm::inverse(transformation);

		m_QuadShader->bind();
		m_QuadShader->upload_uniform_mat_4("u_ViewProjection", viewProjectionMatrix);

		m_LineShader->bind();
		m_LineShader->upload_uniform_mat_4("u_ViewProjection", viewProjectionMatrix);

		m_BuffersPerDraw = 0;

		begin_quad_batch();
		begin_line_batch();
	}

	void Renderer2D::end_scene()
	{
		end_quad_batch();
		end_line_batch();

		m_CurrentLights = 0;
	}

	void Renderer2D::draw_point_light(const glm::vec3& position, float intensity, const glm::vec4& color)
	{
		LightSSBO* l = m_Lights.add_data();

		l->COLOR = color;
		l->INTENSITY = intensity;
		l->POSITION = position;
	}

	void Renderer2D::set_ambient_light(const glm::vec4& color, float intensity)
	{
		m_AmbientColor = color;
		m_AmbientIntensity = intensity;
	}

	void Renderer2D::begin_quad_batch()
	{
		//Reset the vertex collector for the new batch
		m_OpaqueCollector.recycle_collector();
		m_TranslucentCollector.recycle_collector();
		m_OpaqueQuadAttributes.recycle_collector();
		m_TranslucentQuadAttributes.recycle_collector();
		m_Lights.recycle_collector();

		//Next we set all core textures in the texture array, which are necessary for the engine to function properly
		reserve_texture();
	}

	
	uint32_t Renderer2D::flush_opaque_quad(uint32_t begin)
	{
		VertexCollector<QuadVertex>& opaqueCollector = m_OpaqueCollector;

		//First calculate the remaining amount of quads in the collector and take maximum MaxQuadsPerBuffer from it.
		uint32_t currentQuads = begin + m_MaxQuadsPerBuffer * 4 > opaqueCollector.m_Index ? (opaqueCollector.m_Index - begin) / 4 : m_MaxQuadsPerBuffer;
		uint32_t currentVertices = currentQuads * 4;

		//Return end if there is nothing to draw
		if (currentQuads == 0) return opaqueCollector.m_Index;

		//Now upload all needed quads to the VertexBuffer
		m_vBuffer->set_sub_data(opaqueCollector.m_Begin + begin, 0, currentVertices * sizeof(QuadVertex));
		m_sBuffer->set_sub_data(m_OpaqueQuadAttributes.m_Begin + (begin / 4), 0, currentQuads * sizeof(QuadSSBO));


		//Reformat the texture ids (todo cleaner)
		Ref<int[]> sampler(new int[m_TextureIDs.size()]);

		for (auto texture : m_TextureIDs)
		{
			texture.first->bind(texture.second);

			sampler[texture.second] = texture.second;
		}

		//Upload the texture ids to the shader sampler
		m_QuadShader->upload_uniform_int_array("u_Textures", sampler.get(), (int)m_TextureIDs.size());

		//And finally draw all uploaded quads
		m_Context->draw_triangle(m_aBuffer, 6 * currentQuads);

		//Return the begin for the next batch
		return begin + currentVertices;
	}

	uint32_t Renderer2D::flush_translucent_quad(uint32_t begin)
	{
		VertexCollector<QuadVertex>& translucentCollector = m_TranslucentCollector;

		//First calculate the remaining amount of quads in the collector and take maximum MaxQuadsPerBuffer from it.
		uint32_t currentQuads = begin + m_MaxQuadsPerBuffer * 4 > translucentCollector.m_Index ? (translucentCollector.m_Index - begin) / 4 : m_MaxQuadsPerBuffer;
		uint32_t currentVertices = currentQuads * 4;

		//Return end if there is nothing to draw
		if (currentQuads == 0) return translucentCollector.m_Index;

		//Now upload all needed quads to the VertexBuffer
		m_vBuffer->set_sub_data(translucentCollector.m_Begin + begin, 0, currentVertices * sizeof(QuadVertex));
		m_sBuffer->set_sub_data(m_TranslucentQuadAttributes.m_Begin + (begin / 4), 0, currentQuads * sizeof(QuadSSBO));

		//Reformat the texture ids (todo cleaner)
		Ref<int[]> sampler(new int[m_TextureIDs.size()]);

		for (auto texture : m_TextureIDs)
		{
			texture.first->bind(texture.second);

			sampler[texture.second] = texture.second;
		}

		//Upload the texture ids to the shader sampler
		m_QuadShader->upload_uniform_int_array("u_Textures", sampler.get(), (int)m_TextureIDs.size());

		//And finally draw all uploaded quads
		m_Context->draw_triangle(m_aBuffer, 6 * currentQuads);

		//Return the begin for the next batch
		return begin + currentVertices;
	}

	void Renderer2D::end_quad_batch()
	{
		m_OpaqueCollector.validate_vertices(true);
		m_TranslucentCollector.validate_vertices(false);

		//m_TextureShader->upload_uniform_float_1("u_NumLights", (float)m_CurrentLights);

		m_QuadShader->bind();
		m_aBuffer->bind();

		m_sLightsBuffer->set_sub_data(m_Lights.m_Begin + m_Lights.m_Index - 1, 0, sizeof(LightSSBO) * m_Lights.m_Index);

		m_QuadShader->upload_uniform_float_4("u_AmbientColor", m_AmbientColor);
		m_QuadShader->upload_uniform_float_1("u_AmbientIntensity", m_AmbientIntensity);

		uint32_t begin = 0;
		uint32_t end = m_OpaqueCollector.m_Index;

		//TODO hard coded size
		//m_TextureShader->upload_uniform_int_array("m_TextureIDs", m_OpaqueTextureIDs, 10000);

		//Flush until nothing is left
		while (true)
		{
			if (begin == end) break;
			begin = flush_opaque_quad(begin);
		}

		begin = 0;
		end = m_TranslucentCollector.m_Index;

		//TODO hard coded size
		//m_TextureShader->upload_uniform_int_array("m_TextureIDs", m_TranslucentTextureIDs, 10000);

		//Flush until nothing is left
		while (true)
		{
			if (begin == end) break;
			begin = flush_translucent_quad(begin);
		}

		//Delete all texture ids for the new batch
		m_TextureIDs.clear();
	}

	void Renderer2D::begin_line_batch()
	{
		m_LineCollector.recycle_collector();
	}

	uint32_t Renderer2D::flush_line(uint32_t begin)
	{
		VertexCollector<LineVertex>& lineCollector = m_LineCollector;

		uint32_t currentLines = begin + m_MaxLinesPerBuffer * 2 > lineCollector.m_Index ? (lineCollector.m_Index - begin) / 2 : m_MaxLinesPerBuffer;
		uint32_t currentVertices = currentLines * 2;

		//Return end if there is nothing to draw
		if (currentLines == 0) return lineCollector.m_Index;

		//Now upload all needed quads to the VertexBuffer
		m_LineVBuffer->set_sub_data(lineCollector.m_Begin + begin, 0, currentVertices * sizeof(LineVertex));

		//And finally draw all uploaded lines
		m_Context->draw_line(m_LineABuffer, 2 * currentLines);

		//Return the begin for the next batch
		return begin + currentVertices;
	}

	void Renderer2D::end_line_batch()
	{
		m_LineCollector.validate_vertices(false);

		m_LineShader->bind();
		m_LineABuffer->bind();

		uint32_t begin = 0;
		uint32_t end = m_LineCollector.m_Index;

		//Flush until nothing is left
		while (true)
		{
			if (begin == end) break;
			begin = flush_line(begin);
		}
	}

	//Batched ------------------------------------------------------------------------------------------------------------------------------------------------

	void Renderer2D::draw_quad(const glm::vec2& position, const float rotation, const glm::vec2& size, const QuadProps& data)
	{
		draw_quad({ position.x, position.y, 0.0f }, rotation, size, data);
	}

	void Renderer2D::draw_line(const std::vector<glm::vec3> positions, const glm::vec4& color, float thickness)
	{
		for (size_t i = 0; i < positions.size() - 1; i++)
		{
			GrizzlyBear::LineVertex* begin = m_LineCollector.add_vertices();

			begin[0].POSITION = positions[i];
			begin[0].COLOR = color;
			begin[0].THICKNESS = thickness;

			begin[1].POSITION = positions[i+1];
			begin[1].COLOR = color;
			begin[1].THICKNESS = thickness;
		}
	}

	bool Renderer2D::render_scene(Timestep ts, const Ref<Scene> scene)
	{
		if (!scene->has_component<CompCamera>(MemoryType::Dynamic, scene->get_camera())) return false;

		CompCamera* camera = scene->get_component<CompCamera>(MemoryType::Dynamic, scene->get_camera());
		CompTransformation2D* camTransform = scene->get_component<CompTransformation2D>(MemoryType::Dynamic, scene->get_camera());

		glm::mat4 camTransformationMatrix = glm::translate(glm::mat4(1.0f), camTransform->m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(camTransform->m_Rotation), glm::vec3(0, 0, 1));

		return render_scene(ts, scene, camera->m_ProjectionMatrix, camTransformationMatrix);
	}

	bool Renderer2D::render_scene(Timestep ts, const Ref<Scene> scene, const glm::mat4& camProjection, const glm::mat4& camTransformation)
	{
		if (!scene->is_loaded(false, true)) return false;

		begin_scene(camProjection, camTransformation);

		draw_point_light({ 0.0f, 0.0f, 5.0f }, 15.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

		for (auto it = scene->create_iterator<CompMesh2D, CompTransformation2D>(); it != scene->get_end(); it++)
		{
			auto mesh = it.get<CompMesh2D>();
			auto transform = it.get<CompTransformation2D>();

			draw_quad(transform->m_Position, transform->m_Rotation, transform->m_Size, scene->internal_cvt_quad(ts, it.get_index(), mesh));
		}

		end_scene();

		return true;
	}

	void Renderer2D::draw_quad(const glm::vec3& position, const float rotation, const glm::vec2& size, const QuadProps& data)
	{
		float texID = 0.0f;

		bool translucent = false;

		if (data.TEXTURE != nullptr)
		{
			//Check if texture is translucent
			if (data.TEXTURE->get_channels() == 4) translucent = true;

			if (m_TextureIDs.find(data.TEXTURE) != m_TextureIDs.end())
			{
				//Setting id to already existing texture
				texID = (float)m_TextureIDs.at(data.TEXTURE);
			}
			else
			{
				//If no more texture units are left, we have to draw the current buffer and make a new one
				if (m_TextureIDs.size() == m_Context->get_statistics()->get_max_texture_units())
				{
					end_quad_batch();
					begin_quad_batch();
				}

				//Setting id to size => first = 0, second = 1, third = 3, etc.
				texID = (float)m_TextureIDs.size();
				m_TextureIDs.insert({ data.TEXTURE, (int)texID });
			}
		}

		if (data.TINT.a < 1.0f)
		{
			translucent = true;
		}

		draw_quad_impl(position, rotation, size, data, (uint16_t)texID, translucent);
	}

	void Renderer2D::draw_quad_impl(const glm::vec3& position, const float rotation, const glm::vec2& size, const QuadProps& data, uint16_t textureID, bool translucent)
	{
		glm::vec3 v0Pos;
		glm::vec3 v1Pos;
		glm::vec3 v2Pos;
		glm::vec3 v3Pos;

		if (rotation != 0.0f)
		{
			float Vrotation = glm::radians(rotation);

			glm::mat4 translate = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f });
			glm::mat4 inTranslate = glm::inverse(translate);
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), Vrotation, { 0.0f, 0.0f, 1.0f });

			glm::mat4 transform = translate * rotate * inTranslate;

			float hsizeX = size.x / 2.0f;
			float hsizeY = size.y / 2.0f;

			v0Pos = transform * glm::vec4(position.x - hsizeX, position.y - hsizeY, position.z, 1.0f);
			v1Pos = transform * glm::vec4(position.x + hsizeX, position.y - hsizeY, position.z, 1.0f);
			v2Pos = transform * glm::vec4(position.x + hsizeX, position.y + hsizeY, position.z, 1.0f);
			v3Pos = transform * glm::vec4(position.x - hsizeX, position.y + hsizeY, position.z, 1.0f);
		}
		else
		{
			float hsizeX = size.x / 2.0f;
			float hsizeY = size.y / 2.0f;

			v0Pos = glm::vec3(position.x - hsizeX, position.y - hsizeY, position.z);
			v1Pos = glm::vec3(position.x + hsizeX, position.y - hsizeY, position.z);
			v2Pos = glm::vec3(position.x + hsizeX, position.y + hsizeY, position.z);
			v3Pos = glm::vec3(position.x - hsizeX, position.y + hsizeY, position.z);
		}

		QuadVertex* vertices;
		QuadSSBO* ssbo;
		uint32_t index;

		if (translucent)
		{
			vertices = m_TranslucentCollector.add_vertices();
			index = m_TranslucentCollector.get_count()-1;
			m_TranslucentTextureIDs[index] = textureID;
			ssbo = m_TranslucentQuadAttributes.add_data();
		}
		else
		{
			vertices = m_OpaqueCollector.add_vertices();
			index = m_OpaqueCollector.get_count()-1;
			m_OpaqueTextureIDs[index] = textureID;
			ssbo = m_OpaqueQuadAttributes.add_data();
		}

		ssbo->TEXTURE_ID = textureID;
		ssbo->COLOR = data.TINT;
		ssbo->HIGHLIGHT = data.HIGHLIGHT;
		ssbo->TILING_FACTOR = data.TILING_FACTOR;
		ssbo->_pad = 75.0f;

		vertices[0].IDENTIFIER = (float)index;
		vertices[0].POSITION = v0Pos;
		vertices[0].TEXTURE_COORDS = { data.TEXTURE_COORDINATES[0], data.TEXTURE_COORDINATES[1] };

		vertices[1].IDENTIFIER = (float)index;
		vertices[1].POSITION = v1Pos;
		vertices[1].TEXTURE_COORDS = { data.TEXTURE_COORDINATES[2], data.TEXTURE_COORDINATES[3] };

		vertices[2].IDENTIFIER = (float)index;
		vertices[2].POSITION = v2Pos;
		vertices[2].TEXTURE_COORDS = { data.TEXTURE_COORDINATES[4], data.TEXTURE_COORDINATES[5] };

		vertices[3].IDENTIFIER = (float)index;
		vertices[3].POSITION = v3Pos;
		vertices[3].TEXTURE_COORDS = { data.TEXTURE_COORDINATES[6], data.TEXTURE_COORDINATES[7] };
	}

	void Renderer2D::reserve_texture()
	{
		m_TextureIDs.insert({ m_WhiteTexture, 0 });
	}

	Ref<VertexBuffer> Renderer2D::create_quad_vertex_buffer()
	{
		Ref<VertexBuffer> squareVB = m_Context->create_vertex_buffer(nullptr, sizeof(QuadVertex) * 4 * m_MaxQuadsPerBuffer, BufferUsage::Dynamic);
		squareVB->set_layout(*m_QuadBufferLayout);
		m_aBuffer->add_vertex_buffer(squareVB);

		return squareVB;
	}

	Ref<IndexBuffer> Renderer2D::create_quad_index_buffer()
	{
		Ref<uint32_t[]> indices = RenderUtils::generate_quad_indices(m_MaxQuadsPerBuffer);

		Ref<IndexBuffer> squareIB = m_Context->create_index_buffer(indices.get(), 6 * m_MaxQuadsPerBuffer, BufferUsage::Static);

		m_aBuffer->set_index_buffer(squareIB);
		return squareIB;
	}

	Ref<ShaderBuffer> Renderer2D::create_quad_shader_buffer()
	{
		Ref<ShaderBuffer> buffer = m_Context->create_shader_buffer(nullptr, 1, sizeof(QuadSSBO) * m_MaxQuadsPerBuffer, BufferUsage::Dynamic);
		return buffer;
	}

	Ref<VertexBuffer> Renderer2D::create_line_vertex_buffer()
	{
		Ref<VertexBuffer> buffer = m_Context->create_vertex_buffer(nullptr, sizeof(LineVertex) * 2 * m_MaxLinesPerBuffer, BufferUsage::Dynamic);
		buffer->set_layout(*m_LineBufferLayout);
		m_LineABuffer->add_vertex_buffer(buffer);
		return buffer;
	}
}
