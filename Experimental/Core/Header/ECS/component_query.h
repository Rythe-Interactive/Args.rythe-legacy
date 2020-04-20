#pragma once
#include <Types/primitives.h>
#include <ASTL/sparse_set.h>
#include <ECS/entity.h>

namespace Args
{
	struct component_query_base
	{
		const id_type id;
		virtual void evaluate(const entity& entity, bool erasure) = 0;
		virtual size_type entityCount() const = 0;
		virtual const stl::sparse_set<type_id>& requirements() const = 0;
	protected:
		static id_type lastId;
	};

	template<typename... component_types>
	struct component_query : public component_query_base
	{
		stl::sparse_set<type_id> componentTypes;
		stl::sparse_set<entity_id> entities;

		component_query() : component_query_base::id(++lastId)
		{
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
		virtual const stl::sparse_set<type_id>& requirements() const override
		{
			return componentTypes;
		}

	private:
		template<size_type component_count, typename component_type, typename... component_types>
		void insert()
		{
			componentTypes.insert(component_type::type);
			insert<sizeof...(component_types), component_types...>();
		}

		template<size_type component_count, typename component_type>
		void insert()
		{
			componentTypes.insert(component_type::type);
		}

		template<size_type component_count>
		void insert() {}
	};

}