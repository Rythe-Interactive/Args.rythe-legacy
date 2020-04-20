#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>
#include <Utils/SFINAE.h>
#include <ECS/component.h>
#include <ECS/System.h>
#include <ECS/component_query.h>
#include <Scheduling/Scheduler.h>
#include <iostream>

namespace Args
{
	class Engine
	{
	private:
		entity world;
		std::unordered_map<id_type, std::unique_ptr<component_query_base>> componentQueries;
		std::unordered_map<type_id, std::unique_ptr<ComponentFamilyBase>> componentFamilies;
		
		Scheduler scheduler;

	public:

		Engine() : world(this, 0) {}

		template<typename query_type, inherits_from<query_type, component_query_base> = 0>
		void Register()
		{
			componentQueries[query_type::id] = std::make_unique<query_type>();
			std::cout << "query" << std::endl;
		}

		template<typename component_type, inherits_from<component_type, component_base> = 0>
		void Register()
		{
			componentFamilies[component_type::type] = std::make_unique<ComponentFamily<component_type>>(this);
			std::cout << "component" << std::endl;
		}

		template<typename SystemType, inherits_from<SystemType, SystemBase> = 0>
		void Register()
		{
			//systems.push_back(std::move(SystemType(*this)));
			std::cout << "system" << std::endl;
		}
	};
}