#include "Module/TestModule.h"
#include "Components/TestComponent.h"
#include "Components/TestGlobalComponent.h"
#include "Systems/TestSystem.h"
#include "Systems/TestMonoUpdateSystem.h"
#include "Systems/CameraMovementSystem.h"


void TestModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterComponentType<TestComponentA>();
	RegisterComponentType<TestComponentB>();
	RegisterGlobalComponentType<TestGlobalComponent>();
	RegisterComponentType <Args::CameraMovementComponent>();
}

void TestModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<TestSystem>();
	RegisterSystem<TestMonoUpdateSystem>();
	RegisterSystem<CameraMovementSystem>();
}
