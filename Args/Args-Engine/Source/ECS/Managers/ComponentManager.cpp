#include "ECS/Managers/ComponentManager.h"
#include "ECS/ComponentFamily.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/Entity.h"


bool Args::ComponentManager::SetOverlaps(const std::set<uint32>* systemRequirements, const std::set<uint32>* entityComponents)
{
	if (systemRequirements->empty())
		return true;

	if (entityComponents->empty() || entityComponents->size() < systemRequirements->size())
		return false;

	bool overlap = true;
	for (auto& x : *systemRequirements)
		overlap = overlap && entityComponents->count(x);

	return overlap;
}

void Args::ComponentManager::UpdateEntityList(uint32 entityID, uint32 componentTypeId, bool erased)
{
	if (systems)
		for (auto& systemData : *systems)
		{
			auto* systemRequirements = systemData.second.get()->GetComponentRequirements();
			if (!systemRequirements->count(componentTypeId))
				continue;

			auto* entityList = &entityLists[systemData.first];
			if (erased)
			{
				if (entityList->count(entityID))
					entityList->erase(entityID);
			}
			else
				if (SetOverlaps(systemRequirements, &entities[entityID]))
					entityList->insert(entityID); // takes a lot of time
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

Args::uint32 Args::ComponentManager::GetTypeId(std::string typeName)
{
	uint32 hash = 0x811c9dc5;
	uint32 prime = 0x1000193;

	for (int i = 0; i < typeName.length(); ++i)
	{
		byte value = typeName[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

void Args::ComponentManager::DestroyComponentByID(uint32 typeId, uint32 componentId)
{
	auto& componentFamily = componentFamilies[typeId];
	uint32 entityId = componentFamily->GetComponentByID(componentId)->ownerID;
	componentFamily->DestroyComponentByID(componentId);

	if (componentFamily->GetComponentCount(entityId) == 0)
	{
		entities[entityId].erase(typeId);
		UpdateEntityList(entityId, typeId, true);
	}
}

void Args::ComponentManager::DestroyComponent(uint32 entityId, const std::string& typeName, size_t index)
{
	if (componentTypeIds.count(typeName))
		DestroyComponent(entityId, componentTypeIds[typeName], index);
	else
		Debug::Warning(DebugInfo, "ComponentType %s is unknown.", typeName.c_str());
}

void Args::ComponentManager::DestroyComponent(uint32 entityId, uint32 typeId, size_t index)
{
	if (!componentFamilies.count(typeId))
	{
		Debug::Warning(DebugInfo, "ComponentType id %u is unknown.", typeId);
		return;
	}
	auto& componentFamily = componentFamilies[typeId];
	componentFamily->DestroyComponent(entityId, index);

	if (componentFamily->GetComponentCount(entityId) == 0)
	{
		entities[entityId].erase(typeId);
		UpdateEntityList(entityId, typeId, true);
	}
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
	if (componentTypeIds.count(typeName))
	{
		return AddComponent(componentTypeIds[typeName], entityID);
	}
	Debug::Warning(DebugInfo, "ComponentType %s is unknown.", typeName.c_str());
	return 0;
}

Args::uint32 Args::ComponentManager::AddComponent(uint32 typeId, uint32 entityId)
{
	if (componentFamilies.count(typeId))
	{
		auto& componentFamily = componentFamilies[typeId];
		if (componentFamily)
		{
			auto componentID = componentFamily->CreateComponent(entityProxies[entityId]);
			if (componentID && componentFamily->GetComponentCount(entityId) == 1)
			{
				entities[entityId].insert(typeId);
				UpdateEntityList(entityId, typeId, false);
			}
			return componentID;
		}
	}
	Debug::Warning(DebugInfo, "ComponentType id %u is unknown.", typeId);
	return 0;
}

void Args::ComponentManager::DestroyComponent(IComponent* component)
{
	auto& componentFamily = componentFamilies[component->typeID];
	uint32 entityId = component->ownerID;

	componentFamily->DestroyComponent(component);

	if (componentFamily->GetComponentCount(entityId) == 0)
	{
		uint32 typeId = component->typeID;
		entities[entityId].erase(typeId);
		UpdateEntityList(entityId, typeId, true);
	}
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
	{
		auto& componentFamily = componentFamilies[componentTypeId];
		for (int i = 0; i < componentFamily->GetComponentCount(entityId); i++)
			componentFamily->DestroyComponent(entityId, i);
	}

	entities.erase(entityId);

	delete entityProxies[entityId];
	entityProxies.erase(entityId);

	if (systems)
		for (auto& systemData : *systems)
		{
			auto* entityList = &entityLists[systemData.first];
			if (entityList->count(entityId))
				entityList->erase(entityId);
		}
}

const std::set<Args::uint32>& Args::ComponentManager::GetEntityList(std::type_index systemType)
{
	return entityLists[systemType];
}
