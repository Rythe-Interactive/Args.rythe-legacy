#include <Module/InputModule.h>
#include <Input/InputSystem.h>

void Args::InputModule::InitSystems(const std::unordered_set<std::string>& arguments)
{
	RegisterSystem<InputSystem>(50);
}

void Args::InputModule::InitComponents(const std::unordered_set<std::string>& arguments)
{
	RegisterGlobalComponentType<Input>();
}
