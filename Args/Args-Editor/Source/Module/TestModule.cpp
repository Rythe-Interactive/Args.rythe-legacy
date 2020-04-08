#include "Module/TestModule.h"
#include "Systems/TestMonoUpdateSystem.h"


void TestModule::InitComponents(const std::set<std::string>& arguments)
{
}

void TestModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<TestMonoUpdateSystem>();
}
