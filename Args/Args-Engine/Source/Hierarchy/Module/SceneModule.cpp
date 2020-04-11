#include <Hierarchy/Module/SceneModule.h>
#include <Hierarchy/System/SceneSystem.h>

void Args::SceneModule::InitSystems(const std::unordered_set<std::string>& arguments)
{
	RegisterSystem<SceneSystem>(100);
}

void Args::SceneModule::InitComponents(const std::unordered_set<std::string>& arguments)
{
	RegisterGlobalComponentType<SceneComponent>();
}
