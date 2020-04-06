#pragma once
#include <Args-Core.h>

namespace Args
{
	class RenderingModule : public Module
	{
	public:
		RenderingModule(Args::ECS& ecs) : Args::Module(ecs) {}

		// Inherited via Module
		virtual void InitComponents(const std::set<std::string>& arguments) override;
		virtual void InitSystems(const std::set<std::string>& arguments) override;
	};
}