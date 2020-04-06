#pragma once
#include <Args-Core.h>

namespace Args
{
	class PhysicsModule : public Module
	{
	public:
		PhysicsModule(Args::ECS& ecs) : Args::Module(ecs) {}

		// Inherited via Module
		virtual void InitComponents(const std::set<std::string>& arguments) override;
		virtual void InitSystems(const std::set<std::string>& arguments) override;
	};
}