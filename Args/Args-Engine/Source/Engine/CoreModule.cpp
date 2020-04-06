#include "Engine/CoreModule.h"
#include "Base Components/Transform.h"

void Args::CoreModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterComponentType<Transform>();
}

void Args::CoreModule::InitSystems(const std::set<std::string>& arguments)
{

}
