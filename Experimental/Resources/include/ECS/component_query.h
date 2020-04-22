#pragma once
#include <Types/identification.h>
#include <ECS/entity.h>
#include <ECS/ecs_containers.h>
#include <ECS/component_family.h>
#include <Logging/Debug.h>

namespace Args
{
	struct component_query_base
	{
		const id_type id;
		virtual void evaluate(const entity& entity, bool erasure) = 0;
		virtual size_type entityCount() const = 0;
		virtual const type_set& requirements() const = 0;
	protected:
		static inline id_type lastId = invalid_id;
	};

	template<typename... component_types>
	struct component_query : public component_query_base
	{
		static inline std::unordered_map<type_id, component_family_base*> componentFamilies;
		static inline type_set componentTypes;
		static inline entity_set entities;
		static inline id_type id = ++lastId;

		component_query() : component_query_base::id(id)
		{
			if (componentTypes.size() == 0)
				insert<sizeof...(component_types), component_types...>();
		}

		virtual void evaluate(const entity& entity, bool erasure) override
		{
			if (erasure)
			{
				if (entities.contains(entity) && !entity.componentTypes.contains(componentTypes))
					entities.erase(entity);
			}
			else if (!entities.contains(entity) && entity.componentTypes.contains(componentTypes))
				entities.insert(entity);
		}

		virtual size_type entityCount() const override
		{
			return entities.size();
		}

		template<typename component_type>
		component_container<component_type>& components()
		{
			Debug::assert(componentTypes.contains(component_type::type), "component type is not included in query.");
			return static_cast<component_family<component_type>>(componentFamilies[component_type::type])->components;
		}

		virtual const type_set& requirements() const override
		{
			return componentTypes;
		}

	private:
		template<size_type component_count, typename component_type, typename... component_types>
		inline void insert()
		{
			componentTypes.insert(component_type::type);
			insert<sizeof...(component_types), component_types...>();
		}

		template<size_type component_count, typename component_type>
		inline void insert()
		{
			componentTypes.insert(component_type::type);
		}

		template<size_type component_count>
		inline void insert() {}
	};

}