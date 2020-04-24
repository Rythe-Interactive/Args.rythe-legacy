#pragma once
#include <Scheduling/process.h>
#include <Types/identification.h>
#include <map>
#include <memory>

namespace Args
{
	class EntityComponentSystem;

	class SystemBase
	{
	public:
		virtual void init() = 0;
	};

	template<class SystemType>
	class System : public SystemBase
	{
	protected:
		EntityComponentSystem* ecs;

	public:
		std::map<process_id, std::unique_ptr<process>> processes;

		System() = default;
		System(EntityComponentSystem* ecs) : ecs(ecs) {}
	};
}