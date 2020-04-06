#include "ECS/Managers/ComponentManager.h"
#include "ECS/ComponentFamily.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/Entity.h"


bool Args::ComponentManager::SetOverlaps(const std::set<uint32>& systemRequirements, const std::set<uint32>& entityComponents)
{
	if (systemRequirements.empty())
		return true;

	if (entityComponents.empty() || entityComponents.size() < systemRequirements.size())
		return false;

	bool overlap = true;
	for (auto& x : systemRequirements)
		overlap = overlap && entityComponents.count(x);

	return overlap;
}

void Args::ComponentManager::UpdateEntityList(uint32 entityID, uint32 componentTypeId)
{
	if (systems)
		for (auto& systemData : *systems)
		{
			if (!systemData.second.get()->GetComponentRequirements().count(componentTypeId))
				continue;

			if (entityLists[systemData.first].count(entityID))
				entityLists[systemData.first].erase(entityID);

			if (SetOverlaps(systemData.second.get()->GetComponentRequirements(), entities[entityID]))
				entityLists[systemData.first].insert(entityID);
		}
}

void Args::ComponentManager::Destroy()
{
	Debug::Log(DebugInfo, "Cleaning up all components");

	for (auto& family : componentFamilies)
		if (family.second)
			family.second->CleanUp();

	componentFamilies.clear();

	for (auto entity : entityProxies)
		delete entity.second;

	entityProxies.clear();

	for (auto& globalComponent : staticComponents)
		globalComponent.second->CleanUp();

	staticComponents.clear();
}

void Args::ComponentManager::DestroyComponentByTypeID(uint32 typeId, uint32 componentId)
{
	componentFamilies[componentTypeIds[typeId]]->DestroyComponentByID(componentId);
}

void Args::ComponentManager::DestroyComponent(const std::string& typeName, uint32 componentId)
{
	componentFamilies[typeName]->DestroyComponentByID(componentId);
}

void Args::ComponentManager::DestroyComponent(uint32 entityId, const std::string& typeName, size_t index)
{
	componentFamilies[typeName]->DestroyComponent(entityId, index);
}

size_t Args::ComponentManager::GetEntityCount()
{
	return entities.size();
}

Args::Entity* Args::ComponentManager::GetEntityProxy(uint32 entityId)
{
	return entityProxies[entityId];
}

Args::uint32 Args::ComponentManager::AddComponent(std::string typeName, Args::uint32 entityID)
{
	if (componentFamilies[typeName])
	{
		auto componentID = componentFamilies[typeName]->CreateComponent(entityProxies[entityID]);
		if (componentID)
		{
			uint32 typeId = componentFamilies[typeName]->GetComponentTypeID(componentID);
			entities[entityID].insert(typeId);
			UpdateEntityList(entityID, typeId);
		}
		return componentID;
	}
	return 0;
}

void Args::ComponentManager::DestroyComponent(IComponent* component)
{
	componentFamilies[componentTypeIds[component->typeID]]->DestroyComponent(component);
}

Args::uint32 Args::ComponentManager::CreateEntity()
{
	uint32 id = (uint32)entities.size() + 1;
	entities[id];
	entityProxies[id] = new Entity(id, this);

	//Debug::Log(DebugInfo, "Created entity with id %u", id);
	return id;
}

void Args::ComponentManager::DestroyEntity(uint32 entityId)
{
	for (uint32 componentTypeId : entities[entityId])
		for (int i = 0; i < componentFamilies[componentTypeIds[componentTypeId]]->GetComponentCount(entityId); i++)
			componentFamilies[componentTypeIds[componentTypeId]]->DestroyComponent(entityId, i);

	entities.erase(entityId);

	delete entityProxies[entityId];
	entityProxies.erase(entityId);

	if (systems)
		for (auto& systemData : *systems)
			if (entityLists[systemData.first].count(entityId))
				entityLists[systemData.first].erase(entityId);
}

std::set<Args::uint32> Args::ComponentManager::GetEntityList(std::type_index systemType)
{
	return entityLists[systemType];
}
