#pragma once
#include <Args-Core.h>
#include "Components/TestComponent.h"

using namespace std;

class TestSystem : public Args::EntitySystem<TestSystem, TestComponentA, TestComponentB>
{
private:
	int testInt = 0;

public:
	TestSystem() : Args::EntitySystem<TestSystem, TestComponentA, TestComponentB>() {}

	virtual void Init() override;

	void Start();

	void Update(float deltaTime);

	void Print(float deltaTime);
};
