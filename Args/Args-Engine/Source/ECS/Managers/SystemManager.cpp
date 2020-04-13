#include "ECS/Managers/SystemManager.h"

void Args::SystemManager::Destroy()
{
	Debug::Log(DebugInfo, "Cleaning up all systems");

	systems.clear();
}

Args::SystemManager::SystemManager()
{
}


Args::ISystem* Args::SystemManager::GetSystem(std::type_index systemType)
{
	return systems[systemType].get();
}

std::vector<Args::ISystem*> Args::SystemManager::GetSystems()
{
	std::vector<ISystem*> ret;

	for (auto& systemData : systems)
		ret.push_back(systemData.second.get());

	return ret;
}

void Args::SystemManager::InitialiseSystems()
{
	for (auto priorityData : systemPriorities)
		for (auto systemType : priorityData.second)
			systems[systemType]->Init();

	updateClock.Start();
}

void Args::SystemManager::UpdateSystems()
{
	float deltaTime = updateClock.Restart().Seconds();
	for (auto priorityData : systemPriorities)
		for (auto systemType : priorityData.second)
			systems[systemType]->UpdateSystem(deltaTime);
}
