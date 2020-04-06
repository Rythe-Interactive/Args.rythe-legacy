#include <Module/InputModule.h>
#include <Input/InputSystem.h>

void Args::InputModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<InputSystem>(50);
}

void Args::InputModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterGlobalComponentType<Input>();
}
