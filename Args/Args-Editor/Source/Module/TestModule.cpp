#include "Module/TestModule.h"
#include "Components/TestComponents.h"
#include "Systems/TestMonoUpdateSystem.h"
#include "Systems/ECSTestSystem.h"


void TestModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterComponentType<TestComponent>();
}

void TestModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<TestMonoUpdateSystem>();
	RegisterSystem<ECSTestSystem>();
}
