#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>
#include <Types/SFINAE.h>
#include <ECS/component_family.h>
#include <ECS/entity.h>
#include <ECS/System.h>
#include <ECS/component_query.h>
#include <ECS/EntityComponentSystem.h>
#include <Scheduling/Scheduler.h>
#include <iostream>

namespace Args
{
	class Engine
	{
		friend struct entity;
	private:
		entity* world;
		EntityComponentSystem ecs;
		Scheduler scheduler;

	public:

		Engine() : world(new entity(&ecs, 0)) {}
		~Engine() { delete world; world = nullptr; }

		template<typename query_type, inherits_from<query_type, component_query_base> = 0>
		void Register()
		{
			ecs.componentQueries[query_type::id] = std::make_unique<query_type>();

			std::cout << "query" << std::endl;
		}

		template<typename component_type, inherits_from<component_type, component_base> = 0>
		void Register()
		{
			ecs.componentFamilies[component_type::type] = std::make_unique<component_family<component_type>>(&ecs);
			std::cout << "component" << std::endl;
		}

		template<typename SystemType, inherits_from<SystemType, SystemBase> = 0>
		void Register()
		{
			std::cout << "system" << std::endl;
		}
	};
}