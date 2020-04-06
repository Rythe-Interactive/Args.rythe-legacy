#pragma once
#include <unordered_map>
#include <set>
#include <typeindex>
#include <type_traits>

#include <ECS/Managers/ComponentManager.h>


namespace Args
{
	struct IComponent;

	class Entity
	{
	public:
		ComponentManager* manager;
	
		Entity() : id(0), manager(nullptr) {};
		Entity(uint32 id, ComponentManager* manager) : id(id), manager(manager) {};

		const uint32 id;

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent()
		{
			return manager->AddComponent<ComponentType>(id);
		}

		uint32 AddComponent(std::string typeName)
		{
			return manager->AddComponent(typeName, id);
		}

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void DestroyComponent(size_t index = 0)
		{
			manager->DestroyComponent<ComponentType>(id, index);
		}

		void DestroyComponent(std::string typeName, size_t index = 0)
		{
			manager->DestroyComponent(id, typeName, index);
		}

		void DestroyEntity();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		ComponentType* GetComponent(size_t index = 0)
		{
			return manager->GetComponent<ComponentType>(id, index);
		}

		template<typename ComponentType, typename... Components>
		std::unordered_map<std::type_index, std::vector<IComponent*>> GetComponents()
		{
			return manager->GetComponents<ComponentType, Components...>(id);
		}
	};

}