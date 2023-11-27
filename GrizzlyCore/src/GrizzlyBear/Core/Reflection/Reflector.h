#pragma once

#include <type_traits>

#include <boost/preprocessor.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/bind/bind.hpp>
#include <boost/ref.hpp>

#include <nlohmann/json.hpp>

#include "TypeUtils.h"

/*Module Reflection
 * 
 * This is the reflection module of the GrizzlyBear Engine.
 * 
 * Disclaimer: Some of the code is based on the answear from Paul Futz II, which can be found at https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application.
 * 
 * Usage:
 * 
 * Definition:
 *	To reflect a class or struct simply add the Statement GRIZZLY_REFLECT_CLASS(className) inside the class. 
 *	Furthermore you have to declare all your variables in the format GRIZZLY_REFLECT_VARS((typeName) varName, (typeName) varName) (Multiline supported!)
 *	Attention: Inline initialization is not supported and will result in errors (Use the constructor!)
 *	In order to serialize or deserialize, simple call GrizzlyBear::serialize(Class c, Json json) or GrizzlyBear::deserialize(Class c, Json json); 
 *
 * 
 * Supported Types:
 *	All custom data types are supported by the API as long as they implement reflection in their own definition.
 *	Due to the fact that the std library does not support this reflection, these types are hardcoded into the api.
 *	For templated classes such as vector or array all primitive data types such as int, char etc. are accepted.
 *	In order to support your own types in containers such as vector or array you have to write GRIZZLY_SERIALIZE_CON_CLASS(className) after the class definition (In the same namespace!).
 * 
 * Pointers:
 *	The API provides limited support for pointers. This includes pointers to all primitve data types, to std types, but not to glm types.
 *	In the serialized view the pointed value will be stored, not the adress to wich the pointer is pointing.
 *	Attention: If a pointer value gets deserialized, the needed memory will be heap allocated by the API with "new". In the course of that you have to deallocate them by yourself.
 *	Although, this feature is only recommended for big data types, in order to not overflow the stack.
 * 
 * References:
 *	There is no API support for references. Due to the fact that serialization of references does (in most cases) not make much sense,
 *	because if the reference is refering to a value in another class or piece of code, it is very likely that this other class is not the same at serialization and deserialization.
 * 
 * C-Style arrays:
 *	There is no API support for C-Style arrays. Please use std::array.
*/

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

#define GRIZZLY_REFL_REM(...) __VA_ARGS__
#define GRIZZLY_REFL_EAT(...)

// Retrieve the type

#define GRIZZLY_REFL_DETAIL_TYPEOF_HEAD(x, ...) GRIZZLY_REFL_REM x
#define GRIZZLY_REFL_DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),
#define GRIZZLY_REFL_DETAIL_TYPEOF(...) GRIZZLY_REFL_DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#define GRIZZLY_REFL_TYPEOF(x) GRIZZLY_REFL_DETAIL_TYPEOF(GRIZZLY_REFL_DETAIL_TYPEOF_PROBE x,)

// Strip off the type
#define GRIZZLY_REFL_STRIP(x) GRIZZLY_REFL_EAT x
// Show the type without parenthesis
#define GRIZZLY_REFL_PAIR(x) GRIZZLY_REFL_REM x

#elif defined(_MSC_VER)

#define GRIZZLY_REFL_REM(...) __VA_ARGS__
#define GRIZZLY_REFL_EAT(...)

#define GRIZZLY_REFL_DETAIL_TYPEOF_HEAD(x, ...) GRIZZLY_REFL_REM x
#define GRIZZLY_REFL_DETAIL_TYPEOF_INT2(tuple) GRIZZLY_REFL_DETAIL_TYPEOF_HEAD tuple
#define GRIZZLY_REFL_DETAIL_TYPEOF_INT(...) GRIZZLY_REFL_DETAIL_TYPEOF_INT2((__VA_ARGS__))
#define GRIZZLY_REFL_DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),
#define GRIZZLY_REFL_TYPEOF(x) GRIZZLY_REFL_DETAIL_TYPEOF_INT(GRIZZLY_REFL_DETAIL_TYPEOF_PROBE x,)

#define GRIZZLY_REFL_STRIP(x) GRIZZLY_REFL_EAT x
#define GRIZZLY_REFL_PAIR(x) GRIZZLY_REFL_REM x

#endif

namespace GrizzlyBear {

	struct Reflector
	{
	private:
		template<int first, int second>
		struct get_min_at_compile { static const int value = first < second ? true : false; };
	public:
		template<int size, typename T>
		static typename T::template field_value<size, T> get_field_value(T& x)
		{
			return typename T::template field_value<size, T>(x);
		}

		template<typename T>
		static const char* get_class_name(T& x)
		{
			return x.get_class_name();
		}

		template<typename T>
		struct Stats
		{
			static const int number_of_fields = T::number_of_fields;

			static bool is_reflected()
			{
				return number_of_fields != 0;
			}
		};

		//View

		template<typename T, typename View>
		static void view_each(T& c, View v) requires (NOT_Arithmetic<T>&& NOT_Pointer<T>) && NOT_STD_Type<T>
		{
			typedef boost::mpl::range_c<int, 0, Reflector::Stats<T>::number_of_fields> range;
			boost::mpl::for_each<range>(boost::bind<void>(field_view(), boost::ref(c), v, boost::placeholders::_1));
		}

		template<typename T, typename View>
		static void view_each(T& c, View v) requires (NOT_Arithmetic<T>&& Pointer<T>) && NOT_STD_Type<T>
		{
			typedef boost::mpl::range_c<int, 0, Reflector::Stats<std::remove_pointer_t<T>>::number_of_fields> range;
			boost::mpl::for_each<range>(boost::bind<void>(field_view(), boost::ref(*c), v, boost::placeholders::_1));
		}

		//Do nothing if it is an integral or std type
		template<typename T, typename View>
		static void view_each(T& c, View v) requires (Arithmetic<T>&& NOT_Pointer<T>) || STD_Type<T> {}

		//Do nothing if it is an integral or std type
		template<typename T, typename View>
		static void view_each(T& c, View v) requires (Arithmetic<T>&& Pointer<T>) || STD_Type_P<T> {}

		//Serialize

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires (NOT_Arithmetic<T> && NOT_Pointer<T> && NOT_STD_Type<T> && NOT_GLM_Type<T>) 
		{
			typedef boost::mpl::range_c<int, 0, Reflector::Stats<T>::number_of_fields> range;
			boost::mpl::for_each<range>(boost::bind<void>(serialize_view(), boost::ref(c), boost::ref(json), boost::placeholders::_1));
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires (NOT_Arithmetic<T> && Pointer<T> && NOT_STD_Type<T> && NOT_GLM_Type<T>)
		{
			typedef boost::mpl::range_c<int, 0, Reflector::Stats<std::remove_pointer_t<T>>::number_of_fields> range;
			boost::mpl::for_each<range>(boost::bind<void>(serialize_view(), boost::ref(*c), boost::ref(json), boost::placeholders::_1));
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires (Arithmetic<T>&& NOT_Pointer<T>)
		{
			json = c;
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires (Arithmetic<T>&& Pointer<T>)
		{
			json = *c;
		}

		template<typename T, size_t index = 0>
		static typename std::enable_if<get_min_at_compile<index, std::tuple_size_v<T>>::value, void>::type serialize_impl(T& c, nlohmann::json& json) requires STD_Tuple<T>
		{
			serialize(std::get<index>(c), json);

			serialize_impl<T, index + 1>(c, json);
		}

		template<typename T, size_t index = 0>
		static typename std::enable_if<index == std::tuple_size_v<T>, void>::type serialize_impl(T& c, nlohmann::json& json) requires STD_Tuple<T> {}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires STD_Tuple<T>
		{
			//serialize_impl<T>(c, json);
			json = c;
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires STD_Type<T> && NOT_STD_Tuple<T>
		{
			json = c;
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires STD_Type_P<T>
		{
			json = *c;
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires GLM_Vec2<T>
		{
			json = nlohmann::json::array({ c.x, c.y });
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires GLM_Vec3<T>
		{
			json = nlohmann::json::array({ c.x, c.y, c.z });
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires GLM_Vec4<T>
		{
			json = nlohmann::json::array({ c.x, c.y, c.z, c.w });
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires GLM_Mat3<T>
		{
			json = nlohmann::json::array({ c[0][0], c[0][1], c[0][2], c[1][0], c[1][1], c[1][2], c[2][0], c[2][1], c[2][2] });
		}

		template<typename T>
		static void serialize(T& c, nlohmann::json& json) requires GLM_Mat4<T>
		{
			json = nlohmann::json::array({ c[0][0], c[0][1], c[0][2], c[0][3], c[1][0], c[1][1], c[1][2], c[1][3], c[2][0], c[2][1], c[2][2], c[2][3], c[3][0], c[3][1], c[3][2], c[3][3] });
		}

		//Deserialize

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires (NOT_Arithmetic<T>&& NOT_Pointer<T>) && NOT_STD_Type<T>&& NOT_GLM_Type<T>
		{
			typedef boost::mpl::range_c<int, 0, Reflector::Stats<T>::number_of_fields> range;
			boost::mpl::for_each<range>(boost::bind<void>(deserialize_view(), boost::ref(c), boost::ref(json), boost::placeholders::_1));
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires (NOT_Arithmetic<T>&& Pointer<T>) && NOT_STD_Type<T>&& NOT_GLM_Type<T>
		{
			c = new std::remove_pointer_t<T>;

			typedef boost::mpl::range_c<int, 0, Reflector::Stats<std::remove_pointer_t<T>>::number_of_fields> range;
			boost::mpl::for_each<range>(boost::bind<void>(deserialize_view(), boost::ref(*c), boost::ref(json), boost::placeholders::_1));
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires (Arithmetic<T>&& NOT_Pointer<T>)
		{
			c = json;
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires (Arithmetic<T>&& Pointer<T>)
		{
			//Allocating memory for the pointed object. Must be deallocated by the user himself
			c = new std::remove_pointer_t<T>;

			*c = json;
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires STD_Type<T>
		{
			c = json.get<T>();
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires STD_Type_P<T>
		{
			//Allocating memory for the pointed object. Must be deallocated by the user himself
			c = new std::remove_pointer_t<T>;

			*c = json;
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires GLM_Vec2<T>
		{
			c.x = json[0];
			c.y = json[1];
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires GLM_Vec3<T>
		{
			c.x = json[0];
			c.y = json[1];
			c.z = json[2];
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires GLM_Vec4<T>
		{
			c.x = json[0];
			c.y = json[1];
			c.z = json[2];
			c.w = json[3];
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires GLM_Mat3<T>
		{
			c[0][0] = json[0];
			c[0][1] = json[1];
			c[0][2] = json[2];

			c[1][0] = json[3];
			c[1][1] = json[4];
			c[1][2] = json[5];

			c[2][0] = json[6];
			c[2][1] = json[7];
			c[2][2] = json[8];
		}

		template<typename T>
		static void deserialize(T& c, const nlohmann::json& json) requires GLM_Mat4<T>
		{
			c[0][0] = json[0];
			c[0][1] = json[1];
			c[0][2] = json[2];
			c[0][3] = json[3];

			c[1][0] = json[4];
			c[1][1] = json[5];
			c[1][2] = json[6];
			c[1][3] = json[7];

			c[2][0] = json[8];
			c[2][1] = json[9];
			c[2][2] = json[10];
			c[2][3] = json[11];

			c[3][0] = json[12];
			c[3][1] = json[13];
			c[3][2] = json[14];
			c[3][3] = json[15];
		}

	private:
		struct serialize_visitor
		{
			template<typename T, typename Field>
			void operator()(Field field, T& c, nlohmann::json& json)
			{
				nlohmann::json nested;

				serialize(field.get(), nested);

				json[field.get_name()] = nested;
			}
		};

		struct deserialize_visitor
		{
			template<typename T, typename Field>
			void operator()(Field field, T& c, const nlohmann::json& json)
			{
				nlohmann::json nested = json[field.get_name()];

				deserialize(field.get(), nested);
			}
		};

		struct field_view
		{
			template<typename T, typename View, typename I>
			void operator()(T& c, View v, I)
			{
				v(Reflector::get_field_value<I::value>(c));
			}

		};

		struct serialize_view
		{
			template<typename T, typename I>
			void operator()(T& c, nlohmann::json& json, I)
			{
				serialize_visitor()(Reflector::get_field_value<I::value>(c), c, json);
			}
		};

		struct deserialize_view
		{
			template<typename T, typename I>
			void operator()(T& c, const nlohmann::json& json, I)
			{
				deserialize_visitor()(Reflector::get_field_value<I::value>(c), c, json);
			}
		};
	};


	template<typename M, typename T>
	struct make_const
	{
		typedef T type;
	};

	template<typename M, typename T>
	struct make_const<const M, T>
	{
		typedef typename boost::add_const<T>::type type;
	};			

	#define GRIZZLY_REFLECT_CLASS(Name) \
		const char* get_class_name() const { return #Name; }
	
	#define GRIZZLY_REFLECT_EACH(r, data, i, x) \
		GRIZZLY_REFL_PAIR(x); \
		template<typename Self> \
		struct field_value<i, Self> \
		{ \
			Self & self; \
			field_value(Self & self) : self(self) {} \
			\
			typename GrizzlyBear::make_const<Self, GRIZZLY_REFL_TYPEOF(x)>::type & get() \
			{ \
				return self.GRIZZLY_REFL_STRIP(x); \
			} \
			\
			typename boost::add_const<GRIZZLY_REFL_TYPEOF(x)>::type & get() const \
			{ \
				return self.GRIZZLY_REFL_STRIP(x); \
			} \
			\
			const char* get_name() const \
			{ \
				return BOOST_PP_STRINGIZE(GRIZZLY_REFL_STRIP(x)); \
			} \
			\
			const char* get_type_name() const \
			{ \
				return BOOST_PP_STRINGIZE(GRIZZLY_REFL_TYPEOF(x)); \
			} \
			\
		};

	#define GRIZZLY_REFLECT_VARS(...) \
			static const int number_of_fields = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
			friend struct GrizzlyBear::Reflector; \
			template<int size, typename Self> \
			struct field_value {}; \
			BOOST_PP_SEQ_FOR_EACH_I(GRIZZLY_REFLECT_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \

	#define GRIZZLY_SERIALIZE_CON_CLASS(type) \
		\
		inline void to_json(nlohmann::json& json, const type& c) \
		{ \
			GrizzlyBear::Reflector::serialize(c, json); \
		} \
		\
		inline void from_json(const nlohmann::json& json, type& c) \
		{ \
			GrizzlyBear::Reflector::deserialize(c, json); \
		}
}