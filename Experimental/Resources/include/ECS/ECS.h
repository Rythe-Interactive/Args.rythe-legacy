#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>
#include <Utils/SFINAE.h>
#include <ECS/component.h>
#include <ECS/System.h>
#include <ECS/component_query.h>
#include <iostream>

namespace Args
{
	class ECS
	{
	private:
		std::unordered_map<id_type, std::unique_ptr<component_query_base>> componentQueries;
		std::vector<SystenBase> systems;

	public:
		template<typename query_type, inherits_from<query_type, component_query_base> = 0>
		void Register()
		{
			componentQueries.insert(query_type::id, std::make_unique<query_type>());
			std::cout << "query" << std::endl;
		}

		template<typename component_type, inherits_from<component_type, component_base> = 0>
		void Register()
		{
			std::cout << "component" << std::endl;
		}

		template<typename SystemType, inherits_from<SystemType, SystenBase> = 0>
		void Register()
		{
			systems.push_back(std::move(SystemType()));
			std::cout << "system" << std::endl;
		}
	};
}