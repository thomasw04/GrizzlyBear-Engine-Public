#pragma once

#include "GrizzlyBear/Asset/AssetData.h"

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

namespace GrizzlyBear {

	class Shader
	{

	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		//Need to move to an material system
		virtual void upload_uniform_int(const Gstring& name, const int value) = 0;
		virtual void upload_uniform_int_array(const Gstring& name, int* values, int count) = 0;

		virtual void upload_uniform_float_1(const Gstring& name, const float value) = 0;
		virtual void upload_uniform_float_2(const Gstring& name, const glm::vec2& value) = 0;
		virtual void upload_uniform_float_3(const Gstring& name, const glm::vec3& value) = 0;
		virtual void upload_uniform_float_4(const Gstring& name, const glm::vec4& value) = 0;

		virtual void upload_uniform_mat_3(const Gstring& name, const glm::mat3& value) = 0;
		virtual void upload_uniform_mat_4(const Gstring& name, const glm::mat4& value) = 0;

		virtual void execute_compute(uint32_t x, uint32_t y, uint32_t z) = 0;

		virtual const Gstring& get_name() const = 0;
	};

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
}
