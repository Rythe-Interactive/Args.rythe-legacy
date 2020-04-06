#pragma once
#include "ECS/Component.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <queue>
#include <cstring>

namespace Args
{
	class Entity;

	class IComponentFamily
	{
	protected:
		uint32 GetEntityID(const Entity* entity) const;
	public:
		virtual uint32 CreateComponent(Entity* entity) = 0;

		virtual void DestroyComponent(IComponent* component) = 0;
		virtual void DestroyComponent(uint32 entityId, size_t index = 0) = 0;
		virtual void DestroyComponentByID(uint32 componentID) = 0;

		virtual IComponent* GetComponentByID(uint32 componentID) = 0;
		virtual IComponent* GetComponent(uint32 entityId, size_t index = 0) = 0;
		virtual size_t GetComponentCount(uint32 entityId) = 0;
		virtual uint32 GetComponentTypeID(uint32 componentID) = 0;
		virtual std::unordered_map<uint32, std::vector<IComponent*>> GetComponents() = 0;
		virtual std::vector<IComponent*> GetComponentsList() = 0;
		virtual void CleanUp() = 0;
	};

	template<class ComponentType, INHERITS_FROM(ComponentType, Component<ComponentType>)>
	class TypedComponentFamily : public IComponentFamily
	{
		std::queue<uint32> componentIdPool;
		std::queue<size_t> componentIndexPool;
		std::vector<ComponentType> components;
		std::vector<size_t> componentIndices;
		std::unordered_map<uint32, std::vector<uint32>> entityToComponentId;

		uint32 componentTypeId;
	public:

		TypedComponentFamily(uint32 id) : componentTypeId(id)
		{
		}

		virtual void CleanUp() override
		{
			for (auto component : components)
				component.CleanUp();
		}


		virtual uint32 CreateComponent(Entity* entity) override
		{
			uint32 id = 0;
			if (componentIdPool.empty())
			{
				size_t index = components.size();
				id = (uint32)componentIndices.size() + 1;

				componentIndices.push_back(index);

				components.push_back(ComponentType(entity));
				components[index].id = id;

				entityToComponentId[GetEntityID(entity)].push_back(id);
			}
			else
			{
				size_t index = componentIndexPool.front();
				componentIndexPool.pop();
				id = componentIdPool.front();
				componentIdPool.pop();

				componentIndices[id - 1] = index;

				std::memmove(&components[index], &ComponentType(entity), sizeof(ComponentType));
				components[index].id = id;

				entityToComponentId[GetEntityID(entity)].push_back(id);
			}

			//Debug::Log(DebugInfo, "Created component %s with id %u for entity %u", GetTypeName<ComponentType>().c_str(), id, entityID);
			return id;
		}

		virtual IComponent* GetComponentByID(uint32 componentID) override
		{
			return &(components[componentIndices[componentID - 1]]);
		}


		virtual std::unordered_map<uint32, std::vector<IComponent*>> GetComponents() override
		{
			std::unordered_map<uint32, std::vector<IComponent*>> ret;

			for (int i = 0; i < components.size(); i++)
				ret[components[i].ownerID].push_back(&(components[i]));

			return ret;
		}

		virtual std::vector<IComponent*> GetComponentsList() override
		{
			std::vector<IComponent*> ret;

			for (int i = 0; i < components.size(); i++)
				ret.push_back(&(components[i]));

			return ret;
		}

		virtual IComponent* GetComponent(uint32 entityId, size_t index = 0) override
		{
			if (entityToComponentId.count(entityId))
				return &(components[componentIndices[entityToComponentId[entityId][index] - 1]]);
			return nullptr;
		}

		virtual uint32 GetComponentTypeID(uint32 componentID) override
		{
			return componentTypeId;
		}

		virtual void DestroyComponent(IComponent* component) override
		{
			entityToComponentId[component->ownerID].erase(std::remove(entityToComponentId[component->ownerID].begin(), entityToComponentId[component->ownerID].end(), component->id));
			componentIndexPool.push(componentIndices[component->id - 1]);
			componentIdPool.push(component->id);
		}

		virtual size_t GetComponentCount(uint32 entityId) override
		{
			return entityToComponentId[entityId].size();
		}

		virtual void DestroyComponent(uint32 entityId, size_t index = 0) override
		{
			componentIndexPool.push(entityToComponentId[entityId][index]);
			componentIdPool.push(entityToComponentId[entityId][index]);
			entityToComponentId[entityId].erase(entityToComponentId[entityId].begin() + index);
		}

		virtual void DestroyComponentByID(uint32 componentID) override
		{
			DestroyComponent(GetComponentByID(componentID));
		}
	};
}
