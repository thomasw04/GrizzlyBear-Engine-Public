#pragma once

#include <tuple>
#include <array>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <stdio.h>
#include <nlohmann/json.hpp>

#include "SerializeUtils.h"

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/Core/Log.h"

#include "GrizzlyBear/Core/Reflection/Reflector.h"

#include "GrizzlyBear/Misc/Utils.h"

#include "Components/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformationComponent.h"
#include "Components/WidgetComponent.h"

namespace GrizzlyBear {

	#define GRIZZLYBEAR_VERSION_GCS_MAJOR 0x00
	#define GRIZZLYBEAR_VERSION_GCS_MINOR 0x01

	enum class MemoryType
	{
		Static = 0, Dynamic = 1
	};

	class EntityHandle
	{
	public:

		EntityHandle(uint32_t id, MemoryType type = MemoryType::Dynamic)
			: m_ID(id), m_Type(type) {}

		~EntityHandle() {}

		EntityHandle(const EntityHandle& entityHandle)
		{
			this->m_ID = entityHandle.m_ID;
			this->m_Type = entityHandle.m_Type;
		}

		EntityHandle& operator=(const EntityHandle& rhs)
		{
			this->m_ID = rhs.m_ID;
			this->m_Type = rhs.m_Type;

			return *this;
		}

		bool operator==(const EntityHandle& rhs) const
		{
			if (this->m_ID == rhs.m_ID && this->m_Type == rhs.m_Type)
			{
				return true;
			}

			return false;
		}

		uint32_t get_id() const { return m_ID; }
		MemoryType get_type() const { return m_Type; }

		static EntityHandle none()
		{
			return EntityHandle(0);
		}

	private:
		uint32_t m_ID;
		MemoryType m_Type;
	};

	template<typename... Components>
	class GCSIterator;

	template<typename... Components>
	class ComponentManager;

	template<typename... Components>
	class EntityTable
	{
	public:
		//Must be a multiple of eight.
		static constexpr uint32_t MAX_ENTITIES = 8192;

	private:
		//Get index of the component type in the variadic template ---------------------

		template<typename first, typename second, size_t index = 0>
		struct GetComponentIndex {};

		template<size_t index, bool same>
		struct ComponentEqual : public std::integral_constant<size_t, index> {};

		template<size_t index>
		struct ComponentEqual<index, false> : public std::integral_constant<size_t, 0>
		{
			//	GRIZZLY_ASSERT(false, "Error: Unknown Component Type");
		};

		template<typename first, typename second, size_t index>
		struct GetComponentIndex<std::tuple<first>, second, index>
			: public ComponentEqual<index, std::is_same<first, second>::value> {};

		template<typename first, typename... other, typename second, size_t index>
		struct GetComponentIndex<std::tuple<first, other...>, second, index>
			: public std::conditional<std::is_same<first, second>::value, std::integral_constant<size_t, index>, GetComponentIndex<std::tuple<other...>, second, index + 1>>::type {};

		//Get minimum at compile time--------------------------------------------------------------------------

		template<int first, int second>
		struct min { static const int value = first < second ? true : false; };

		//Get type at index in tuple---------------------------------------------------------------------------

		template<size_t index, typename... other>
		struct GetComponentType;

		template<size_t index, typename first, typename... other>
		struct GetComponentType<index, std::tuple<first, other...>>
		{
			using type = typename GetComponentType<index - 1, std::tuple<other...>>::type;
		};

		template<typename first, typename... other>
		struct GetComponentType<0, std::tuple<first, other...>>
		{
			using type = first;
		};

		//-----------------------------------------------------------------------------------------------------
		static constexpr uint64_t SIZE = sizeof...(Components);

		static constexpr uint64_t STATE_SIZE = (MAX_ENTITIES / 8) * SIZE;
		static constexpr uint64_t TOTAL_SIZE = (sizeof(Components) + ...) * MAX_ENTITIES + STATE_SIZE;

		using TupleType = std::tuple<Components...>;
		using TupleTypePtr = std::tuple<Components*...>;

		using StaticType = std::unordered_map<uint32_t, std::array<Gstring, sizeof...(Components)>>;

	public:

		EntityTable()
		{
			//Prepare memory to store all components in one big array, accessible via the pointers in the tuple, which are pointing at the starting memory position of each component type

			//Calculate the size of each component type in bytes
			calc_component_size();

			//Allocate the big array
			m_ComponentSpace = std::malloc(TOTAL_SIZE);

			for (size_t i = 0; i < TOTAL_SIZE - STATE_SIZE; i++)
			{
				*(static_cast<char*>(m_ComponentSpace) + i) = (char)0;
			}

			//Assign all pointers in the tuple to their memory adress.
			assign_memory();
		}

		EntityTable(const Gstring& body)
			: EntityTable()
		{
			uint64_t stateSection = body.find(GCSTokens::BEGIN_STATE_SECTION);
			uint64_t sizeBegin = body.substr(stateSection).find("Size: ") + 6;
			uint64_t stateBegin = sizeBegin + 8;

			unsigned char size[8];
			
			for (int i = 0; i < 8; i++)
			{
				size[i] = body[sizeBegin + i];
			}

			uint64_t stateSize = conv_char_to_uint64(size);

			//Make sure that the engines does not crash on wrong state size.
			if (stateSize > STATE_SIZE)
			{
				//Get State Information
				char* bytes = static_cast<char*>(m_ComponentSpace);
				std::memcpy(bytes, body.substr(stateBegin).c_str(), STATE_SIZE);
			}
			else
			{
				//Get State Information
				char* bytes = static_cast<char*>(m_ComponentSpace);
				std::memcpy(bytes, body.substr(stateBegin).c_str(), stateSize);
			}

			deserialize(body.substr(body.substr(stateBegin + stateSize).find(GCSTokens::BEGIN_COMPONENT_SECTION)));
		}

		~EntityTable()
		{
			std::free(m_ComponentSpace);
		}

		template<size_t index = 0>
		inline typename std::enable_if<min<index, SIZE>::value, void>::type deserializeComponents(nlohmann::json& json)
		{
			std::string type = std::string(" ") + std::string(typeid(typename GetComponentType<index, TupleType>::type).name());

			for (auto it = begin<typename GetComponentType<index, TupleType>::type>(); it != end(); it++)
			{
				if (json.find(std::to_string(it.get_index()) + type) == json.end()) continue;

				nlohmann::json comp = json[std::to_string(it.get_index()) + type];

				auto component = it.template get<typename GetComponentType<index, TupleType>::type>();

				GrizzlyBear::Reflector::deserialize(*component, comp);
			}

			deserializeComponents<index + 1>(json);
		}

		template<size_t index = 0>
		inline typename std::enable_if<index == SIZE, void>::type deserializeComponents(nlohmann::json& json) {}


		template<size_t index = 0>
		inline typename std::enable_if<min<index, SIZE>::value, void>::type serializeComponents(nlohmann::json& json)
		{
			std::string type = std::string(" ") + std::string(typeid(typename GetComponentType<index, TupleType>::type).name());

			for (auto it = begin<typename GetComponentType<index, TupleType>::type>(); it != end(); it++)
			{
				nlohmann::json comp;

				auto component = it.template get<typename GetComponentType<index, TupleType>::type>();

				GrizzlyBear::Reflector::serialize(*component, comp);

				json[std::to_string(it.get_index()) + type] = comp;
			}

			serializeComponents<index + 1>(json);
		}

		template<size_t index = 0>
		inline typename std::enable_if<index == SIZE, void>::type serializeComponents(nlohmann::json& json) {}

		void serialize(std::stringstream& sstream)
		{
			nlohmann::json json;

			serializeComponents(json);

			GrizzlyBear::Reflector::serialize(m_DynamicEntityStaticComponents, json["DynamicEntitiesStaticComponents"]);
			GrizzlyBear::Reflector::serialize(m_StaticEntityStaticComponents, json["StaticEntitiesStaticComponents"]);

			//Save state information
			char* bytes = static_cast<char*>(m_ComponentSpace);

			std::string state(STATE_SIZE, '\0');

			std::memcpy(&state[0], bytes, STATE_SIZE);

			unsigned char size[8];
			conv_uint64_to_char(size, STATE_SIZE);

			sstream << GCSTokens::BEGIN_STATE_SECTION << '\n' << "Size: ";
			sstream << size[0] << size[1] << size[2] << size[3] << size[4] << size[5] << size[6] << size[7];
			sstream << state << '\n';

			sstream << GCSTokens::BEGIN_COMPONENT_SECTION << '\n' << "Components: " << json.dump(GCSTokens::JSON_FORMAT) << '\n';
		}


		void deserialize(const Gstring& body)
		{
			nlohmann::json json = nlohmann::json::parse(body.substr(body.find("Components: ") + 12));

			deserializeComponents(json);

			GrizzlyBear::Reflector::deserialize(m_DynamicEntityStaticComponents, json["DynamicEntitiesStaticComponents"]);
			GrizzlyBear::Reflector::deserialize(m_StaticEntityStaticComponents, json["StaticEntitiesStaticComponents"]);
		}

		template<typename Component>
		void set_static_component(const EntityHandle& entityHandle, const Gstring& componentName)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return;

			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				m_DynamicEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value] = componentName;
			}
			else if (entityHandle.get_type() == MemoryType::Static)
			{
				m_StaticEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value] = componentName;
			}
		}

		template<typename Component>
		Gstring get_static_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return "";

			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				return m_DynamicEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value];
			}
			else if (entityHandle.get_type() == MemoryType::Static)
			{
				return m_StaticEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value];
			}

			return "";
		}

		template<typename Component>
		bool has_static_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return false;

			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				return m_DynamicEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value] != "";
			}
			else if (entityHandle.get_type() == MemoryType::Static)
			{
				return m_StaticEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value] != "";
			}

			return false;
		}

		template<typename Component>
		void remove_static_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return;

			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				m_DynamicEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value] = "";
			}
			else if (entityHandle.get_type() == MemoryType::Static)
			{
				m_StaticEntityStaticComponents[entityHandle.get_id()][GetComponentIndex<TupleType, Component>::value] = "";
			}
		}

		template<typename Component, typename... InitArgs>
		inline Component* set_component(const EntityHandle& entityHandle, InitArgs... args)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return nullptr;

			if (entityHandle.get_id() >= MAX_ENTITIES)
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Invalid entity index: {0}", entityHandle.get_id());
				return nullptr;
			}

			activate_component<Component>(entityHandle);

			Component* otherComp = std::get<GetComponentIndex<TupleType, Component>::value>(m_Components);

			return &(otherComp[entityHandle.get_id()] = Component( args... ));
		}

		template<typename Component>
		inline bool activate_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return false;

			if (entityHandle.get_id() >= MAX_ENTITIES)
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Invalid entity index: {0}", entityHandle.get_id());
				return false;
			}

			bool b[8];
			get_state_pack<Component>((int)entityHandle.get_id(), b);

			uint32_t subIndex = entityHandle.get_id() % 8;

			if (b[subIndex])
			{
				return false;
			}

			b[subIndex] = true;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, Component>::value) + ((int)entityHandle.get_id() / 8);

			*(static_cast<char*>(m_ComponentSpace) + byteIndex) = pack_bools(b);

			return true;
		}

		template<typename Component>
		inline bool deactivate_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return false;

			if (entityHandle.get_id() >= MAX_ENTITIES)
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Invalid entity index: {0}", entityHandle.get_id());
				return false;
			}

			bool b[8];
			get_state_pack<Component>((int)entityHandle.get_id(), b);

			uint32_t subIndex = entityHandle.get_id() % 8;

			if (!b[subIndex])
			{
				return false;
			}

			b[subIndex] = false;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, Component>::value) + ((int)entityHandle.get_id() / 8);

			*(static_cast<char*>(m_ComponentSpace) + byteIndex) = pack_bools(b);

			return true;
		}

		template<typename Component>
		inline bool remove_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return false;

			if (entityHandle.get_id() >= MAX_ENTITIES)
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Invalid entity index: {0}", entityHandle.get_id());
				return false;
			}

			if(!deactivate_component<Component>(entityHandle)) return false;

			Component* otherComp = std::get<GetComponentIndex<TupleType, Component>::value>(m_Components);

			otherComp[entityHandle.get_id()] = Component();

			return true;
		}

		template<typename Component>
		inline Component* get_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return nullptr;

			if (entityHandle.get_id() >= MAX_ENTITIES)
			{
				GRIZZLY_CORE_LOG(LogLevel::Error, "Invalid entity index: {0}", entityHandle.get_id());
				return nullptr;
			}

			return std::get<GetComponentIndex<TupleType, Component>::value>(m_Components) + entityHandle.get_id();
		}

		template<typename Component>
		inline bool has_component(const EntityHandle& entityHandle)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return false;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, Component>::value) + (entityHandle.get_id() / 8);

			bool b[8];

			unpack_bools(*(static_cast<char*>(m_ComponentSpace) + byteIndex), b);

			return b[entityHandle.get_id() % 8];
		}

		/*
		 * Set parent of Component if the Component type has the ComponentNode base class (with the m_Children field)
		 * Warning: This operation can be very heavyweight. All components of the children entity and the whole subtree of this children will be swapped  
		 */
		template<typename Component>
		inline void set_entity_parent(const EntityHandle& newParent, const EntityHandle& child) requires std::is_base_of_v<ComponentNode, Component>
		{
			if (newParent != EntityHandle::none())
			{
				if (has_component<Component>(newParent))
				{
					auto comp = get_component<Component>(newParent);


				}
			}
		}

		template<typename Component>
		inline void get_state_pack(uint32_t entityIndex, bool* b)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, Component>::value) + ((int)entityIndex / 8);

			unpack_bools(*(static_cast<char*>(m_ComponentSpace) + byteIndex), b);
		}

		template<typename Component>
		inline void get_state_pack_by_index(uint32_t packIndex, bool* b)
		{
			if (GetComponentIndex<TupleType, Component>::value == -1) return;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, Component>::value) + ((int)packIndex);

			unpack_bools(*(static_cast<char*>(m_ComponentSpace) + byteIndex), b);
		}

		template<typename First, typename... Other>
		inline uint8_t get_active_component(uint32_t packIndex, uint8_t startIndex, bool backward)
		{
			if (GetComponentIndex<TupleType, First>::value == -1) return 8;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, First>::value) + ((int)packIndex);

			bool b[8];

			unpack_bools(*(static_cast<char*>(m_ComponentSpace) + byteIndex), b);

			if (!backward)
			{
				for (int i = startIndex; i < 8; i++)
				{
					if (b[i] && is_valid_for<Other...>(packIndex, i))
					{
						return i;
					}
				}
			}
			else
			{
				for (int i = startIndex; i >= 0; i--)
				{
					if (b[i] && is_valid_for<Other...>(packIndex, i))
					{
						return i;
					}
				}
			}

			return 8;
		}

		template<typename First, typename... Other>
		inline bool is_valid_for(uint32_t packIndex, int index) requires (!std::is_void_v<First>)
		{
			if (GetComponentIndex<TupleType, First>::value == -1) return false;

			uint32_t byteIndex = ((MAX_ENTITIES / 8) * GetComponentIndex<TupleType, First>::value) + ((int)packIndex);

			bool b[8];

			unpack_bools(*(static_cast<char*>(m_ComponentSpace) + byteIndex), b);

			if (b[index])
			{
				return is_valid_for<Other...>(packIndex, index);
			}
			else
			{
				return false;
			}
		}

		template<typename First = void>
		inline bool is_valid_for(uint32_t packIndex, int index) requires std::is_void_v<First>
		{
			return true; 
		}


		template<size_t index = 0>
		inline typename std::enable_if<min<index, SIZE>::value, void>::type remove_all_components(uint32_t entityIndex)
		{
			remove_component<typename GetComponentType<index, TupleType>::type>(entityIndex);

			remove_all_components<index + 1>(entityIndex);
		}

		template<size_t index = 0>
		inline typename std::enable_if<index == SIZE, void>::type remove_all_components(uint32_t entityIndex) {}

		template<typename... Types>
		inline GCSIterator<Types...> begin()
		{
			uint32_t pool = 0;
			uint8_t rIndex;
			
			while(true)
			{
				if (pool >= (MAX_ENTITIES / 8) - 1) return { this, MAX_ENTITIES };

				rIndex = get_active_component<Types...>(pool++, 0, false);

				if (rIndex != 8)
				{
					return { this, ((pool - 1) * 8) + rIndex };
				}
			}
		}

		inline int end() const
		{
			return MAX_ENTITIES;
		}

	private:

		inline unsigned char pack_bools(bool b[8]) const
		{
			unsigned char c = 0;

			for (int i = 0; i < 8; i++)
			{
				if (b[i]) c |= 1 << i;
			}

			return c;
		}

		inline void unpack_bools(unsigned char c, bool* b)
		{
			for (int i = 0; i < 8; i++)
			{
				b[i] = (c & (1 << i)) != 0;
			}
		}

		//Recursive calculation of the component size
		template<size_t index = 0>
		inline typename std::enable_if<min<index, SIZE>::value, void>::type calc_component_size()
		{
			m_ComponentSize[index] = sizeof(typename GetComponentType<index, TupleType>::type);
			calc_component_size<index + 1>();
		}

		template<size_t index = 0>
		inline typename std::enable_if<index == SIZE, void>::type calc_component_size() {}

		//Recursive assignment of the pointers
		template<size_t index = 0>
		inline typename std::enable_if<min<index, SIZE>::value, void>::type assign_memory()
		{
			//Begin after the component states
			size_t bytes = STATE_SIZE;

			for (int i = 0; i < index; i++)
			{
				bytes += m_ComponentSize[i] * MAX_ENTITIES;
			}

			auto start = (typename GetComponentType<index, TupleTypePtr>::type)(static_cast<char*>(m_ComponentSpace) + bytes);

			std::uninitialized_value_construct_n(start, MAX_ENTITIES);

			//Get the component type from the current index and cast the char* to it
			std::get<index>(m_Components) = start;

			assign_memory<index + 1>();
		}

		template<std::size_t index = 0>
		inline typename std::enable_if<index == SIZE, void>::type assign_memory() {}

	public:
		
	private:
		//Dynamic Entities
		TupleTypePtr m_Components;
		std::array<size_t, SIZE> m_ComponentSize;
		void* m_ComponentSpace;

		std::unordered_map<uint32_t, bool> m_DeadReferences;

		StaticType m_DynamicEntityStaticComponents;

		//Static Entities
		StaticType m_StaticEntityStaticComponents;
	};

	//TODO Add components here
	using TableType = EntityTable<CompTransformation, CompCamera, CompSprite, CompScript>;
	using CompManagerType = ComponentManager<CompTransformation, CompCamera, CompSprite, CompScript>;

	template<typename... Components>
	class GCSIterator
	{
	public:

		GCSIterator(TableType* registry, uint32_t index = 0)
			: m_IndexCounter(index), m_Registry(registry)
		{
			if (index == 0) forward();
		}

		uint32_t forward()
		{
			if (m_IndexCounter == TableType::MAX_ENTITIES) return TableType::MAX_ENTITIES;

			uint32_t pool = m_IndexCounter / 8;
			uint8_t rIndex;

			while (true)
			{
				if (pool >= (TableType::MAX_ENTITIES / 8) - 1) return m_IndexCounter = TableType::MAX_ENTITIES;

				if(m_IndexCounter != -1)
					rIndex = m_Registry->get_active_component<Components...>(pool++, (m_IndexCounter % 8) + 1, false);
				else
					rIndex = m_Registry->get_active_component<Components...>(pool++, 0, false);

				if (rIndex != 8)
				{
					return m_IndexCounter = ((pool - 1) * 8) + rIndex;
				}

				m_IndexCounter = -1;
			}
		}

		uint32_t backward()
		{
			if (m_IndexCounter == 0) return 0;

			uint32_t pool = m_IndexCounter / 8;
			uint8_t rIndex;

			while (true)
			{
				if (pool >= (TableType::MAX_ENTITIES / 8) - 1) return m_IndexCounter = TableType::MAX_ENTITIES;

				rIndex = m_Registry->get_active_component<Components...>(pool--, m_IndexCounter % 8, true);

				if (rIndex != 8)
				{
					return m_IndexCounter = (pool * 8) + rIndex;
				}
			}
		}

		template<typename Component>
		Component* get()
		{
			return m_Registry->get_component<Component>(m_IndexCounter);
		}

		uint32_t get_index()
		{
			return m_IndexCounter;
		}

		GCSIterator<Components...>& operator++()
		{
			forward();
			return *this;
		}

		GCSIterator<Components...> operator++(int)
		{
			GCSIterator result(*this);
			++(*this);
			return result;
		}

		GCSIterator<Components...>& operator--()
		{
			backward();
			return *this;
		}

		GCSIterator<Components...> operator--(int)
		{
			GCSIterator result(*this);
			--(*this);
			return result;
		}

		GCSIterator<Components...>& operator+=(int right)
		{
			for (int i = 0; i < right; i++)
			{
				++(*this);
			}

			return *this;
		}

		GCSIterator<Components...>& operator-=(int right)
		{
			for (int i = 0; i < right; i++)
			{
				--(*this);
			}

			return *this;
		}

		bool operator==(const uint32_t& right)
		{
			return this->m_IndexCounter == right;
		}

		bool operator!=(const uint32_t& right)
		{
			return this->m_IndexCounter != right;
		}

		bool operator<(const uint32_t& right)
		{
			return this->m_IndexCounter < right;
		}

		bool operator>(const uint32_t& right)
		{
			return this->m_IndexCounter > right;
		}

		bool operator>=(const uint32_t& right)
		{
			return this->m_IndexCounter >= right;
		}

		bool operator<=(const uint32_t& right)
		{
			return this->m_IndexCounter <= right;
		}

		GCSIterator<Components...> operator=(const uint32_t& right)
		{
			this->m_IndexCounter = right;

			return *this;
		}

	private:
		uint32_t m_IndexCounter;

		TableType* m_Registry;
	};


	template<typename... Components>
	class ComponentManager
	{
		template<typename first, typename second, size_t index = 0>
		struct GetComponentIndex {};

		template<size_t index, bool same>
		struct ComponentEqual : public std::integral_constant<size_t, index> {};

		template<size_t index>
		struct ComponentEqual<index, false> : public std::integral_constant<size_t, 0>
		{
			//	GRIZZLY_ASSERT(false, "Error: Unknown Component Type");
		};

		template<typename first, typename second, size_t index>
		struct GetComponentIndex<std::tuple<first>, second, index>
			: public ComponentEqual<index, std::is_same<first, second>::value> {};

		template<typename first, typename... other, typename second, size_t index>
		struct GetComponentIndex<std::tuple<first, other...>, second, index>
			: public std::conditional<std::is_same<first, second>::value, std::integral_constant<size_t, index>, GetComponentIndex<std::tuple<other...>, second, index + 1>>::type {};

		template<size_t index, typename... other>
		struct GetComponentType;

		template<size_t index, typename first, typename... other>
		struct GetComponentType<index, std::tuple<first, other...>>
		{
			using type = typename GetComponentType<index - 1, std::tuple<other...>>::type;
		};

		template<typename first, typename... other>
		struct GetComponentType<0, std::tuple<first, other...>>
		{
			using type = first;
		};

		template<int first, int second>
		struct min { static const int value = first < second ? true : false; };

		using TupleType = std::tuple<std::unordered_map<Gstring, Components>...>;
		using TupleTypeRaw = std::tuple<Components...>;

		using AllComponentsType = std::tuple<Components...>;

		template<typename Component>
		using MapType = std::unordered_map<Gstring, Component>;

	public:

		ComponentManager(const Gstring& body)
		{
			deserialize(body);
		}

		ComponentManager()
		{
			m_Registry = CreateRef<TableType>();
		}

		void serialize(std::stringstream& sstream)
		{
			nlohmann::json json;

			GrizzlyBear::Reflector::serialize(m_Templates, json["ComponentTemplates"]);
			GrizzlyBear::Reflector::serialize(m_TemplateEntities, json["EntityTemplates"]);

			sstream << GCSTokens::BEGIN_TEMPLATE_SECTION << '\n' << "Templates: " << json.dump(GCSTokens::JSON_FORMAT) << '\n';

			m_Registry->serialize(sstream);
		}

		void deserialize(const Gstring& body)
		{
			uint64_t templateSectionBegin = body.find(GCSTokens::BEGIN_TEMPLATE_SECTION);

			uint64_t templateBegin = body.substr(templateSectionBegin).find("Templates: ") + 11;

			nlohmann::json json = nlohmann::json::parse(body.substr(templateBegin, body.find(GCSTokens::BEGIN_STATE_SECTION) - templateBegin));

			GrizzlyBear::Reflector::deserialize(m_Templates, json["ComponentTemplates"]);
			GrizzlyBear::Reflector::deserialize(m_TemplateEntities, json["EntityTemplates"]);

			m_Registry = CreateRef<TableType>(body.substr(body.substr(templateSectionBegin).find(GCSTokens::BEGIN_STATE_SECTION)));
		}

		template<typename Component>
		inline void set_template_component(const Gstring& name, const Component& comp)
		{
			MapType<Component>& map = std::get<GetComponentIndex<TupleType, MapType<Component>>::value>(m_Templates);
			map.insert({ name, comp });
		}

		template<typename First, typename... AttachedComponents>
		inline void set_template_entity(const Gstring& entityName, std::initializer_list<std::pair<MemoryType, Gstring>> other, int index = 0)
		{
			m_TemplateEntities[entityName][GetComponentIndex<AllComponentsType, First>::value] = *(other.begin() + index);

			set_template_entity<AttachedComponents...>(entityName, other, ++index);
		}

		template<typename... T>
		inline void set_template_entity(const Gstring& entityName, std::initializer_list<std::pair<MemoryType, Gstring>> other, int index = 0) requires (sizeof...(T) == 0) {}

		template<size_t index = 0>
		inline typename std::enable_if<min<index, sizeof...(Components)>::value, void>::type set_entity(const EntityHandle& entityHandle, const Gstring& name)
		{
			auto& arr = m_TemplateEntities[name];

			set_component<typename GetComponentType<index, AllComponentsType>::type>(std::get<1>(arr[index]), (MemoryType)std::get<0>(arr[index]), entityHandle);

			set_entity<index + 1>(entityHandle, name);
		}

		template<size_t index = 0>
		inline typename std::enable_if<index == sizeof...(Components), void>::type set_entity(const EntityHandle& entityHandle, const Gstring& name) {}

		template<typename Component>
		inline void set_component(const Gstring& name, const MemoryType componentType, const EntityHandle& entityHandle)
		{
			if (entityHandle == EntityHandle::none()) return;

			MapType<Component>& map = std::get<GetComponentIndex<TupleType, MapType<Component>>::value>(m_Templates);

			//For dynamic entities copy the component in the corresponding field
			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				if (componentType == MemoryType::Dynamic)
				{
					m_Registry->activate_component<Component>(entityHandle);

					if (map.contains(name))
					{
						*m_Registry->get_component<Component>(entityHandle) = map[name];
					}
					else
					{
						//If the template is not present just construct a default component
						m_Registry->set_component<Component>(entityHandle);
					}
				}
				else if(componentType == MemoryType::Static)
				{
					m_Registry->set_static_component<Component>(entityHandle, name);
				}
			}
			else
			{
				if (componentType == MemoryType::Dynamic)
				{
					GRIZZLY_CORE_LOG(LogLevel::Error, "Module: EntityManager, Message: Static Entities can�t have dynamic components!");
					return;
				}
				else
				{
					m_Registry->set_static_component<Component>(entityHandle, name);
				}
			}
		}

		template<typename Component>
		inline void remove_component(const MemoryType componentType, const EntityHandle& entityHandle)
		{
			//For dynamic entities copy the component in the corresponding field
			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				if (componentType == MemoryType::Dynamic)
				{
					m_Registry->deactivate_component<Component>(entityHandle);
				}
				else if (componentType == MemoryType::Static)
				{
					m_Registry->remove_static_component<Component>(entityHandle);
				}
			}
			else
			{
				if (componentType == MemoryType::Dynamic)
				{
					GRIZZLY_CORE_LOG(LogLevel::Error, "Module: EntityManager, Message: Static Entities can�t have dynamic components!");
					return;
				}
				else
				{
					m_Registry->remove_static_component<Component>(entityHandle);
				}
			}
		}

		template<typename Component>
		inline Component* get_component(const MemoryType componentType, const EntityHandle& entityHandle)
		{
			if (entityHandle == EntityHandle::none()) return nullptr;

			//For dynamic entities copy the component in the corresponding field
			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				if (componentType == MemoryType::Dynamic)
				{
					return m_Registry->get_component<Component>(entityHandle);
				}
				else if(componentType == MemoryType::Static)
				{
					if (!m_Registry->has_static_component<Component>(entityHandle)) return nullptr;

					return &std::get<GetComponentIndex<TupleType, MapType<Component>>::value>(m_Templates)[m_Registry->get_static_component<Component>(entityHandle)];
				}

				return nullptr;
			}
			else
			{
				if (componentType == MemoryType::Dynamic)
				{
					GRIZZLY_CORE_LOG(LogLevel::Error, "Module: EntityManager, Message: Static Entities can�t have dynamic components!");
					return nullptr;
				}
				else
				{
					if (!m_Registry->has_static_component<Component>(entityHandle)) return nullptr;

					return &std::get<GetComponentIndex<TupleType, MapType<Component>>::value>(m_Templates)[m_Registry->get_static_component<Component>(entityHandle)];
				}
			}
		}

		template<typename Component>
		inline bool has_component(const MemoryType componentType, const EntityHandle& entityHandle)
		{
			if (entityHandle == EntityHandle::none()) return false;

			//For dynamic entities copy the component in the corresponding field
			if (entityHandle.get_type() == MemoryType::Dynamic)
			{
				if (componentType == MemoryType::Dynamic)
				{
					return m_Registry->has_component<Component>(entityHandle);
				}
				else if (componentType == MemoryType::Static)
				{
					return m_Registry->has_static_component<Component>(entityHandle);
				}

				return false;
			}
			else
			{
				if (componentType == MemoryType::Dynamic)
				{
					GRIZZLY_CORE_LOG(LogLevel::Error, "Module: EntityManager, Message: Static Entities can�t have dynamic components!");
					return false;
				}
				else
				{
					return m_Registry->has_static_component<Component>(entityHandle);
				}				
			}
		}

		template<typename Component>
		inline bool attach_component(const EntityHandle& entityHandle, bool replace = true)
		{
			if (entityHandle == EntityHandle::none()) return false;

			if (entityHandle.get_type() != MemoryType::Dynamic) return false;

			if(replace)
				return m_Registry->set_component<Component>(entityHandle) != nullptr;

			return m_Registry->activate_component<Component>(entityHandle);
		}

		template<typename Component>
		inline bool detach_component(const EntityHandle& entityHandle)
		{
			if (entityHandle == EntityHandle::none()) return false;

			if (entityHandle.get_type() != MemoryType::Dynamic) return false;

			return m_Registry->deactivate_component<Component>(entityHandle);
		}

		void remove_all_components(const EntityHandle& entityHandle)
		{
			if (entityHandle == EntityHandle::none()) return;

			m_Registry->remove_all_components(entityHandle.get_id());
		}
		
		template<typename... NewComponents>
		inline GCSIterator<NewComponents...> create_iterator()
		{
			return GCSIterator<NewComponents...>(m_Registry.get());
		}

		inline int get_end()
		{
			return m_Registry->end();
		}

	private:
		Ref<TableType> m_Registry;
		
		TupleType m_Templates;

		std::unordered_map<Gstring, std::array<std::pair<MemoryType, Gstring>, sizeof...(Components)>> m_TemplateEntities;
	};
}






