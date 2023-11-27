#pragma once

#include <type_traits>

#include <string>
#include <vector>
#include <array>
#include <set>

#include <glm/glm.hpp>

namespace GrizzlyBear {

	//Generalized
	template<class T>
	concept Arithmetic = std::is_arithmetic_v<std::remove_const_t<std::remove_pointer_t<T>>> || std::is_enum_v<std::remove_const_t<std::remove_pointer_t<T>>>;

	template<class T>
	concept NOT_Arithmetic = !Arithmetic<T>;

	template<class T>
	concept Pointer = std::is_pointer_v<T>;

	template<class T>
	concept NOT_Pointer = !Pointer<T>;

	template<class Base, class Derived>
	concept IS_Base_of = std::is_base_of_v<Base, Derived>;

	//STD
	template<typename T>
	struct is_vector : std::false_type {};

	template<class T>
	struct is_vector<std::vector<T>> : std::true_type {};

	template<typename T>
	struct is_set : std::false_type {};

	template<class T>
	struct is_set<std::set<T>> : std::true_type {};

	template<typename T>
	struct is_array : std::false_type {};

	template<class T, std::size_t N>
	struct is_array<std::array<T, N>> : std::true_type {};

	template<typename T>
	struct is_pair : std::false_type {};

	template<class T, class U>
	struct is_pair<std::pair<T, U>> : std::true_type {};

	template<typename T>
	struct is_unordered_map : std::false_type {};

	template<class T, class U>
	struct is_unordered_map<std::unordered_map<T, U>> : std::true_type {};

	template<typename T>
	struct is_tuple : std::false_type {};

	template<class... T>
	struct is_tuple<std::tuple<T...>> : std::true_type {};

	//Types
	template<class T>
	concept STD_String = std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, std::string>;

	template<class T>
	concept STD_Vector = is_vector<std::remove_const_t<std::remove_pointer_t<T>>>::value;

	template<class T>
	concept STD_Array = is_array<std::remove_const_t<std::remove_pointer_t<T>>>::value;

	template<class T>
	concept STD_Set = is_set<std::remove_const_t<std::remove_pointer_t<T>>>::value;

	template<class T>
	concept STD_Pair = is_pair<std::remove_const_t<std::remove_pointer_t<T>>>::value;

	template<class T>
	concept STD_UnorderedMap = is_unordered_map<std::remove_const_t<std::remove_pointer_t<T>>>::value;

	template<class T>
	concept STD_Tuple = is_tuple<std::remove_const_t<std::remove_pointer_t<T>>>::value;

	template<class T>
	concept NOT_STD_Tuple = !(STD_Tuple<T>);

	template<class T>
	concept NOT_STD_Type = !(STD_String<T> || STD_Vector<T> || STD_Array<T> || STD_Set<T> || STD_Pair<T> || STD_UnorderedMap<T> || STD_Tuple<T>);

	template<class T>
	concept STD_Type = !NOT_STD_Type<T> && !std::is_pointer_v<T>;

	template<class T>
	concept STD_Type_P = !NOT_STD_Type<T> && std::is_pointer_v<T>;

	//Names---------------------------------------------------------------------------------
	template<class T>
	std::string get_std_name() requires STD_String<T>
	{
		return "std::string";
	}

	template<class T>
	std::string get_std_name() requires STD_String<T> && STD_Type_P<T>
	{
		return "std::string*";
	}

	template<class T>
	std::string get_std_name() requires STD_Vector<T>
	{
		return "std::vector";
	}

	template<class T>
	std::string get_std_name() requires STD_Vector<T> && STD_Type_P<T>
	{
		return "std::vector*";
	}

	template<class T>
	std::string get_std_name() requires STD_Array<T>
	{
		return "std::array";
	}

	template<class T>
	std::string get_std_name() requires STD_Array<T> && STD_Type_P<T>
	{
		return "std::array*";
	}

	template<class T>
	std::string get_std_name() requires STD_Set<T>
	{
		return "std::set";
	}

	template<class T>
	std::string get_std_name() requires STD_Set<T> && STD_Type_P<T>
	{
		return "std::set*";
	}

	template<class T>
	std::string get_std_name() requires STD_Pair<T>
	{
		return "std::pair";
	}

	template<class T>
	std::string get_std_name() requires STD_Pair<T> && STD_Type_P<T>
	{
		return "std::pair*";
	}

	template<class T>
	std::string get_std_name() requires STD_UnorderedMap<T>
	{
		return "std::unordered_map";
	}

	template<class T>
	std::string get_std_name() requires STD_UnorderedMap<T> && STD_Type_P<T>
	{
		return "std::unordered_map*";
	}

	template<class T>
	std::string get_std_name() requires STD_Tuple<T>
	{
		return "std::tuple";
	}

	template<class T>
	std::string get_std_name() requires STD_Tuple<T>&& STD_Type_P<T>
	{
		return "std::tuple*";
	}

	//GLM------------------------------------------------------------------------------------

	//Types
	template<class T>
	concept GLM_Vec2 = std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, glm::vec2>;

	template<class T>
	concept GLM_Vec3 = std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, glm::vec3>;

	template<class T>
	concept GLM_Vec4 = std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, glm::vec4>;

	template<class T>
	concept GLM_Mat3 = std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, glm::mat3>;

	template<class T>
	concept GLM_Mat4 = std::is_same_v<std::remove_const_t<std::remove_pointer_t<T>>, glm::mat4>;

	template<class T>
	concept NOT_GLM_Type = !(GLM_Vec2<T> || GLM_Vec3<T> || GLM_Vec4<T> || GLM_Mat3<T> || GLM_Mat4<T>);

	template<class T>
	concept GLM_Type = !NOT_GLM_Type<T> && !std::is_pointer_v<T>;

	template<class T>
	concept GLM_Type_P = !NOT_GLM_Type<T> && std::is_pointer_v<T>;

	//Names-------------------------------------------------------------------------------------------


	//Vec2
	template<class T>
	std::string get_glm_name() requires GLM_Vec2<T>
	{
		return "glm::vec2";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Vec2<T> && GLM_Type_P<T>
	{
		return "glm::vec2*";
	}

	//Vec3
	template<class T>
	std::string get_glm_name() requires GLM_Vec3<T>
	{
		return "glm::vec3";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Vec3<T> && GLM_Type_P<T>
	{
		return "glm::vec3*";
	}

	//Vec4
	template<class T>
	std::string get_glm_name() requires GLM_Vec4<T>
	{
		return "glm::vec4";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Vec4<T> && GLM_Type_P<T>
	{
		return "glm::vec4*";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Mat3<T>
	{
		return "glm::mat3";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Mat3<T> && GLM_Type_P<T>
	{
		return "glm::mat3*";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Mat4<T>
	{
		return "glm::mat4";
	}

	template<class T>
	std::string get_glm_name() requires GLM_Mat4<T> && GLM_Type_P<T>
	{
		return "glm::mat4*";
	}
}
