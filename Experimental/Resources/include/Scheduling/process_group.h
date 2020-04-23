#pragma once
#include <map>
#include <memory>
#include <ECS/System.h>
#include <Types/prioritization.h>
#include <Types/SFINAE.h>

namespace Args
{
	class Engine;
	class process_group
	{
		// maybe not priority but interval instead?
		std::multimap<type_id, process> systems;

		Engine* engine;

		process_group() = default;
		process_group(Engine* engine) : engine(engine){}



	};
}