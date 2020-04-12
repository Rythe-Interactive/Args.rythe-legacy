#pragma once
#include <Args-Core.h>
#include "Components/TestComponents.h"

class ECSTestSystem : public Args::EntitySystem<ECSTestSystem, Args::Transform, TestComponent>
{
	using Args::EntitySystem<ECSTestSystem, Args::Transform, TestComponent>::EntitySystem;
	
	Args::Clock clock;

	virtual void Init() override;

	void Update(double deltaTime);
};