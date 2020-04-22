#pragma once
#include <map>
#include <memory>
#include <ECS/System.h>
#include <Types/prioritization.h>
#include <Types/SFINAE.h>

namespace Args
{
	class Engine;
	class SystemGroup
	{
		// maybe not priority but interval instead?
		std::multimap<priority, std::unique_ptr<SystemBase>> systems;

		Engine* engine;

		SystemGroup() = default;
		SystemGroup(Engine* engine) : engine(engine){}


		template<typename SystemType, inherits_from<SystemType, SystemBase> = 0>
		void AddSystem(priority priority = default_priority)
		{
			systems.emplace(std::make_pair<priority, std::make_unique<SystemType>(engine)>)
		}


	};
}