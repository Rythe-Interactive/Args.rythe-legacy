#pragma once
#include <Args-Core.h>
#include <Input/InputSystem.h>
#include "Components/input.h"

namespace Args
{
	class InputModule : public Module
	{
	public:
		InputModule(ECS& ecs) : Module(ecs) {}

		// Inherited via Module
		virtual void InitSystems(const std::set<std::string>& arguments) override;

		// Inherited via Module
		virtual void InitComponents(const std::set<std::string>& arguments) override;
	};
}
