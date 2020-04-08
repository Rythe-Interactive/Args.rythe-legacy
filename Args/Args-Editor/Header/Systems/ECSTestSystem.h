#pragma once
#include <Args-Core.h>
#include "Components/TestComponents.h"

class ECSTestSystemA : public Args::EntitySystem<ECSTestSystemA, Args::Transform, TestComponentA>
{
	using Args::EntitySystem<ECSTestSystemA, Args::Transform, TestComponentA>::EntitySystem;
	
	Args::Clock clock;

	virtual void Init() override;

	void Update(float deltaTime);
};


class ECSTestSystemB : public Args::EntitySystem<ECSTestSystemB, Args::Transform, TestComponentB>
{
	using Args::EntitySystem<ECSTestSystemB, Args::Transform, TestComponentB>::EntitySystem;

	Args::Clock clock;

	virtual void Init() override;

	void Update(float deltaTime);
};