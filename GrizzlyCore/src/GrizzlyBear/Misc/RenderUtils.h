#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <GrizzlyBear/Core/BaseDefines.h>

#include <GrizzlyBear/Renderer/Renderer.h>

#include "TextureUtils.h"

namespace GrizzlyBear {

	/**
	* Architecture for a basic GrizzlyBear vertex
	*/
	struct Vertex3D
	{
		float IDENTIFIER;
		glm::vec3 POSITION;
	};

	struct QuadVertex : public Vertex3D
	{
		glm::vec2 TEXTURE_COORDS;
	};

	struct LineVertex : public Vertex3D
	{
		glm::vec4 COLOR;
		float THICKNESS;
	};

	struct QuadProps
	{
		QuadProps()
			: TINT({ 1.0f, 1.0f, 1.0f, 1.0f }), TILING_FACTOR(1.0f), HIGHLIGHT(0.0f),
			TEXTURE_COORDINATES({ 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }), TEXTURE(nullptr) {}

		glm::vec4 TINT;
		float TILING_FACTOR;
		float HIGHLIGHT;
		std::array<float, 8> TEXTURE_COORDINATES;
		Ref<Texture> TEXTURE;
	};

	struct AmbientLight
	{
		glm::vec4 COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
		float INTENSITY = 1.0f;
	};

	struct QuadSSBO
	{
		glm::vec4 COLOR;
		float TILING_FACTOR;
		float TEXTURE_ID;
		float HIGHLIGHT;
		float _pad;
	};

	struct LightSSBO
	{
		glm::vec3 POSITION;
		float INTENSITY;
		glm::vec4 COLOR;
	};

	template<typename T>
	struct BasicCollector
	{
		T* m_Begin = nullptr;
		T* m_Iterator = nullptr;
		uint32_t m_Index;

	private:
		uint32_t m_DefaultCapacity = 10000;
		uint32_t m_DefaultCapacityIncrease = 10000;

		uint32_t m_Capacity;
		uint32_t m_CapacityIncrease;
	public:
		BasicCollector();

		bool set_capacity(uint32_t newCapacity);
		bool set_capacity_increase(uint32_t newCapacityIncrease);

		uint32_t get_capacity();
		uint32_t get_capacity_increase();

		/**
		* Resets the index counter in order to override the existing data
		* Does preserve the Capacity and CapacityIncrease
		*/
		void recycle_collector();

		/**
		* Resets the whole collector inclusive Capacity and CapacityIncrease
		*/
		void reset_collector();

		/**
		* Returns the size of the whole BasicCollector in bytes
		*/
		uint32_t get_collector_size();

		T* add_data();
	};

	enum class Geometry
	{
		QUADS = 4, TRIANGLES = 3, LINES = 2, POINTS = 1
	};

	/**
	* Struct for collecting vertices in order to store them in a GL VertexBuffer
	* The collector automatically resizes, also it is not possible to delete vertices with the inbuild functions.
	*/
	template<typename T>
	struct VertexCollector
	{
		T* m_Begin = nullptr;
		T* m_Iterator = nullptr;

		Geometry m_Geometry;

		//The index is pointing at the next available object in the array
		uint32_t m_Index;

	private:
		uint32_t m_DefaultCapacity = 10000;
		uint32_t m_DefaultCapacityIncrease = 10000;

		uint32_t m_Capacity;
		uint32_t m_CapacityIncrease;

		uint32_t m_IndicesPerObject;
	public:

		VertexCollector(Geometry geometry);
		~VertexCollector();

		bool set_capacity(uint32_t newCapacity);
		bool set_capacity_increase(uint32_t newCapacityIncrease);

		uint32_t get_capacity();
		uint32_t get_capacity_increase();
		uint32_t get_count();

		//TODO automatic shrink if for a specific time period less than x% of the collector was used.

		/**
		* Resets the index counter in order to override the existing vertices
		* Does preserve the Capacity and CapacityIncrease
		*/
		void recycle_collector();

		/**
		* Resets the whole collector inclusive Capacity and CapacityIncrease
		*/
		void reset_collector();

		/**
		* Returns the size of the whole VertexCollector in bytes
		*/
		uint32_t get_collector_size();

		uint32_t get_indices_count();

		/**
		* Adds a for example 4 vertices if the geometry is a quad
		*/
		T* add_vertices();

		/**
		* Must be called after all vertices are added in order to sort them
		*/
		void validate_vertices(bool backFirst);
	};

	template<typename T>
	class PersistentCache
	{
	public:

		inline void store(const T& cache)
		{
			m_Stored = true;
			m_Cache = cache;
		}

		inline T load()
		{
			if (!m_Stored) return T();

			return m_Cache;
		}

		void reset()
		{
			m_Stored = false;
		}

		bool is_stored()
		{
			return m_Stored;
		}

	private:

		T m_Cache;
		bool m_Stored = false;
	};

	struct Animation2D
	{
		Animation2D()
			: ACTIVE(false), CURRENT_FRAME(0), END_FRAME(0), PLAYTHROUGH_TIME(0),
			LAST_FRAME_TIME(0), LOOP(false), TEXTURE_FILEPATH(""), TEXTURE_LAYOUT() {}

		GRIZZLY_REFLECT_VARS
		(
			(bool) ACTIVE,
			(int) CURRENT_FRAME,
			(int) END_FRAME,
			(float) PLAYTHROUGH_TIME,
			(float) LAST_FRAME_TIME,
			(bool) LOOP,
			(Gstring) TEXTURE_FILEPATH,
			(AtlasLayout) TEXTURE_LAYOUT
		)
	};

	template<typename T>
	class StaticVertexArray
	{
		struct ObjectHandle
		{
			int ID;
		};

		using ObjHNDL = ObjectHandle;
	public:

		StaticVertexArray(Geometry geometry) : m_Geometry(geometry) {}

		ObjHNDL add_object(T* vertices)
		{

		}

		void remove_object(const ObjHNDL obj)
		{
			
		}

	private:
		Geometry m_Geometry;

		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

	struct RenderUtils
	{
		static Ref<uint32_t[]> generate_quad_indices(uint32_t objectCount);
		static Ref<uint32_t[]> generate_triangle_indices(uint32_t objectCount);
		static Ref<uint32_t[]> generate_line_indices(uint32_t objectCount);
		static Ref<uint32_t[]> generate_point_indices(uint32_t objectCount);
	};

	//Basic Collector--------------------------------------------------------------------------------------

	template<typename T>
	inline BasicCollector<T>::BasicCollector()
	{
		reset_collector();
	}

	template<typename T>
	inline bool BasicCollector<T>::set_capacity(uint32_t newCapacity)
	{
		m_Capacity = newCapacity;
		return true;
	}

	template<typename T>
	inline bool BasicCollector<T>::set_capacity_increase(uint32_t newCapacityIncrease)
	{
		m_CapacityIncrease = newCapacityIncrease;
		return false;
	}

	template<typename T>
	inline uint32_t BasicCollector<T>::get_capacity()
	{
		return m_Capacity;
	}

	template<typename T>
	inline uint32_t BasicCollector<T>::get_capacity_increase()
	{
		return m_CapacityIncrease;
	}

	template<typename T>
	inline void BasicCollector<T>::recycle_collector()
	{
		m_Iterator = m_Begin;
		m_Index = 0;
	}

	template<typename T>
	inline void BasicCollector<T>::reset_collector()
	{
		m_Capacity = m_DefaultCapacity;
		m_CapacityIncrease = m_DefaultCapacityIncrease;

		delete[] m_Begin;

		m_Begin = new T[m_Capacity];
		m_Iterator = m_Begin;
		m_Index = 0;
	}

	template<typename T>
	inline uint32_t BasicCollector<T>::get_collector_size()
	{
		return m_Index * sizeof(T);
	}

	template<typename T>
	inline T* BasicCollector<T>::add_data()
	{
		if (m_Index == m_Capacity)
		{
			uint32_t newCapacity = m_Capacity + m_CapacityIncrease;
			T* newArray = new T[newCapacity];

			std::memcpy(newArray, m_Begin, m_Capacity * sizeof(T));

			delete[] m_Begin;

			m_Begin = newArray;
			m_Iterator = newArray + m_Index;
			m_Capacity = newCapacity;
		}

		m_Index++;
		auto returnIt = m_Iterator;
		m_Iterator++;

		return returnIt;
	}


	//VertexCollector--------------------------------------------------------------------------------------

	template<typename T>
	inline VertexCollector<T>::VertexCollector(Geometry geometry)
	{
		m_Geometry = geometry;

		m_DefaultCapacity *= (int)m_Geometry;
		m_DefaultCapacityIncrease *= (int)m_Geometry;

		if (m_Geometry == Geometry::QUADS) m_IndicesPerObject = 6;
		if (m_Geometry == Geometry::TRIANGLES) m_IndicesPerObject = 3;
		if (m_Geometry == Geometry::LINES) m_IndicesPerObject = 2;
		if (m_Geometry == Geometry::POINTS) m_IndicesPerObject = 1;

		reset_collector();
	}

	template<typename T>
	inline VertexCollector<T>::~VertexCollector()
	{
		delete[] m_Begin;
	}

	template<typename T>
	inline void VertexCollector<T>::recycle_collector()
	{
		m_Iterator = m_Begin;
		m_Index = 0;
	}

	template<typename T>
	inline bool VertexCollector<T>::set_capacity(uint32_t newCapacity)
	{
		if (newCapacity % (uint32_t)m_Geometry == 0)
		{
			m_Capacity = newCapacity;

			return true;
		}

		return false;
	}

	template<typename T>
	inline bool VertexCollector<T>::set_capacity_increase(uint32_t newCapacityIncrease)
	{
		if (newCapacityIncrease % (uint32_t)m_Geometry == 0)
		{
			m_CapacityIncrease = newCapacityIncrease;

			return true;
		}

		return false;
	}

	template<typename T>
	inline uint32_t VertexCollector<T>::get_capacity()
	{
		return m_Capacity;
	}

	template<typename T>
	inline uint32_t VertexCollector<T>::get_capacity_increase()
	{
		return m_CapacityIncrease;
	}

	template<typename T>
	inline uint32_t VertexCollector<T>::get_count()
	{
		return m_Index / (int)m_Geometry;
	}

	template<typename T>
	inline void VertexCollector<T>::reset_collector()
	{
		m_Capacity = m_DefaultCapacity;
		m_CapacityIncrease = m_DefaultCapacityIncrease;

		delete[] m_Begin;

		m_Begin = new T[m_Capacity];
		m_Iterator = m_Begin;
		m_Index = 0;
	}

	template<typename T>
	inline uint32_t VertexCollector<T>::get_collector_size()
	{
		return m_Index * sizeof(T);
	}

	template<typename T>
	inline uint32_t VertexCollector<T>::get_indices_count()
	{
		return m_IndicesPerObject * (m_Capacity / (int)m_Geometry);
	}

	template<typename T>
	inline T* VertexCollector<T>::add_vertices()
	{
		if (m_Index == m_Capacity)
		{
			uint32_t newCapacity = m_Capacity + m_CapacityIncrease;
			T* newArray = new T[newCapacity];

			std::memcpy(newArray, m_Begin, m_Capacity * sizeof(T));

			delete[] m_Begin;

			m_Begin = newArray;
			m_Iterator = newArray + m_Index;
			m_Capacity = newCapacity;
		}

		m_Index += (int)m_Geometry;

		auto returnIt = m_Iterator;

		m_Iterator += (int)m_Geometry;

		return returnIt;
	}

	template<typename T>
	inline void VertexCollector<T>::validate_vertices(bool backFirst)
	{
		if (backFirst)
		{
			//Sorting vertices back to front
			std::stable_sort(m_Begin, m_Iterator, [](const T& v1, const T& v2) -> bool
			{
				if (v1.POSITION.z > v2.POSITION.z)
				{
					return true;
				}

				return false;
			});
		}
		else
		{
			//Sorting vertices front to back
			std::stable_sort(m_Begin, m_Iterator, [](const T& v1, const T& v2) -> bool
			{
				if (v1.POSITION.z < v2.POSITION.z)
				{
					return true;
				}

				return false;
			});
		}
	}

	//---------------------------------------------------------------------------------------------------
}
