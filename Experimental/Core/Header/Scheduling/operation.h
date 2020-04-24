#pragma once
#include <ASTL/delegate.h>
#include <ECS/ecs_containers.h>
#include <ECS/EntityComponentSystem.h>
#include <ECS/component_query.h>
#include <Types/time_types.h>
#include <Types/identification.h>
#include <type_traits>

namespace Args
{
	struct operation_base
	{
		EntityComponentSystem* ecs;

		operation_base(EntityComponentSystem* ecs) : ecs(ecs) {}

		virtual void operator()(fast_seconds delta_time) = 0;
		virtual void invoke(fast_seconds delta_time) = 0;
	};

	template<typename... argument_types>
	struct typed_operation_base : public operation_base
	{
	protected:
		stl::delegate<void(fast_seconds, argument_types...)> operationFunc;

	public:
		typed_operation_base(EntityComponentSystem* ecs, const stl::delegate<void(fast_seconds, argument_types...)>& delegate) : operation_base(ecs)
		{
			operationFunc = delegate;
		}

		typed_operation_base(const typed_operation_base<argument_types...>&) = default;
	};

	template<typename... component_types>
	struct sync_operation : public typed_operation_base<component_container<component_types>&...>
	{
		static_assert((std::is_base_of<component_base, component_types>::value || ...), "One of the passed components doesn't inherit from Component.");

		using typed_operation_base<component_container<component_types>&...>::typed_operation_base;

		virtual void operator()(fast_seconds delta_time) override
		{
			component_query<component_types...>* query = &operation_base::ecs->componentQueries[component_query<component_types...>::id];
			this->operationFunc.invoke(delta_time, query->components<component_types>()...);
		}

		virtual void invoke(fast_seconds delta_time) override
		{
			component_query<component_types...>* query = &operation_base::ecs->componentQueries[component_query<component_types...>::id];
			this->operationFunc.invoke(delta_time, query->components<component_types>()...);
		}
	};

	template<typename... component_types>
	using operation = sync_operation<component_types...>;

	template<typename... component_types>
	struct async_operation : public typed_operation_base<component_types&...>
	{
		static_assert((std::is_base_of<component_base, component_types>::value || ...), "One of the passed components doesn't inherit from Component.");

		using typed_operation_base<component_types&...>::typed_operation_base;

		virtual void operator()(fast_seconds delta_time) override
		{
			component_query<component_types...>* query = &operation_base::ecs->componentQueries[component_query<component_types...>::id];
			for (entity_id entity : query->entities())
				this->operationFunc.invoke(delta_time, query->components<component_types>()[entity]...); // TODO: change to job request instead of linear iteration
		}

		virtual void invoke(fast_seconds delta_time) override
		{
			component_query<component_types...>* query = &operation_base::ecs->componentQueries[component_query<component_types...>::id];
			for (entity_id entity : query->entities())
				this->operationFunc.invoke(delta_time, query->components<component_types>()[entity]...); // TODO: change to job request instead of linear iteration
		}
	};
}