#include "ECS/Managers/ECS.h"

size_t Args::ECS::GetEntityCount()
{
	return componentManager.GetEntityCount();
}

Args::uint32 Args::ECS::CreateEntity()
{
	return componentManager.CreateEntity();
}

void Args::ECS::InitialiseSystems()
{
	systemManager.InitialiseSystems();
}

void Args::ECS::UpdateSystems()
{
	systemManager.UpdateSystems();
}
