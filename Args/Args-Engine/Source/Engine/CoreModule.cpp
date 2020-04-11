#include "Engine/CoreModule.h"
#include "Base Components/Transform.h"

void Args::CoreModule::InitComponents(const std::unordered_set<std::string>& arguments)
{
	RegisterComponentType<Transform>();
}

void Args::CoreModule::InitSystems(const std::unordered_set<std::string>& arguments)
{

}
