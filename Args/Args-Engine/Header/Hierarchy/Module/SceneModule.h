#pragma once
#include "Engine/Module.h"
#include "Hierarchy/System/SceneSystem.h"
#include "Hierarchy/Component/SceneComponent.h"

namespace Args
{
	class SceneModule : public Module
	{
	public:
		SceneModule(ECS& ecs) : Module(ecs) {}

		// Inherited via Module
		virtual void InitSystems(const std::set<std::string>& arguments) override;

		// Inherited via Module
		virtual void InitComponents(const std::set<std::string>& arguments) override;
	};
}
