#pragma once
#include "Module.h"

namespace Args
{
	class CoreModule : public Module
	{
	public:
		CoreModule(Args::ECS& ecs) : Args::Module(ecs) {}

		virtual void InitComponents(const std::set<std::string>& arguments) override;
		virtual void InitSystems(const std::set<std::string>& arguments) override;
	};
}
