#pragma once
#include <unordered_map>
#include <set>
#include <memory>
#include <typeindex>
#include "Utils/Common.h"
#include "ECS/ComponentFamily.h"

namespace Args
{
	class IComponentFamily;
	struct IComponent;

	class ISystem;

	class ComponentManager
	{
		friend class ECS;
	private:
		std::unordered_map<std::string, std::unique_ptr<IComponentFamily>> componentFamilies;
		std::unordered_map<std::string, std::unique_ptr<IGlobalComponent>> staticComponents;
		std::unordered_map<uint32, std::string> staticComponentTypeIds;
		std::unordered_map<uint32, std::string> componentTypeIds;
		std::unordered_map<uint32, std::set<uint32>> entities;
		std::unordered_map<uint32, Entity*> entityProxies;
		std::unordered_map<std::type_index, std::set<uint32>> entityLists;

		std::unordered_map<std::type_index, std::unique_ptr<ISystem>>* systems = nullptr;

		bool SetOverlaps(const std::set<uint32>* lhs, const std::set<uint32>* rhs);

		void UpdateEntityList(uint32 entityID, uint32 componentTypeId, bool erased);

		void Destroy();

	public:
		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void RegisterComponentType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IGlobalComponent)>
		void RegisterGlobalComponentType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent(uint32 entityID);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent(uint32 entityID, ComponentType** componentHandle);

		/// <summary>
		/// Request component creation on component name.
		/// Is less reliable than the templated version since it does not check the component type on compile time.
		/// On success returns pointer to created component. Will return nullptr with unknown types.
		/// </summary>
		uint32 AddComponent(std::string typeName, uint32 entityID);

		void DestroyComponent(IComponent* component);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void DestroyComponent(uint32 entityId, size_t index = 0);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void DestroyComponent(uint32 componentId);

		void DestroyComponentByTypeID(uint32 typeId, uint32 componentId);
		void DestroyComponent(const std::string& typeName, uint32 componentId);
		void DestroyComponent(uint32 entityId, const std::string& typeName, size_t index = 0);


		size_t GetEntityCount();

		Entity* GetEntityProxy(uint32 entityId);

		uint32 CreateEntity();

		void DestroyEntity(uint32 entityId);

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		const std::set<uint32>& GetEntityList();

		const std::set<uint32>& GetEntityList(std::type_index systemType);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IGlobalComponent)>
		ComponentType* GetGlobalComponent();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		ComponentType* GetComponent(uint32 entityId, size_t index = 0);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		ComponentType* GetComponentByID(uint32 componentId);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		size_t GetComponentCount(uint32 entityId);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		size_t GetComponentCountOfType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		std::vector<ComponentType*> GetComponentsOfType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		std::vector<ComponentType*> GetComponentsOfType(uint32 entityId);

		template<typename ComponentType, typename... Components>
		std::unordered_map<std::type_index, std::vector<IComponent*>> GetComponents(uint32 entityId)
		{
			std::unordered_map<uint32, std::vector<IComponent*>> componentsPerEntity = componentFamilies[GetTypeName<ComponentType>()]->GetComponents();

			std::unordered_map<std::type_index, std::unordered_map<uint32, std::vector<IComponent*>>> components;

			for (IComponent* component : componentsPerEntity[entityId])
				components[typeid(ComponentType)].push_back(component);

			return components;
		}

		template<typename ComponentType, typename... Components>
		std::unordered_map<std::type_index, std::unordered_map<uint32, std::vector<IComponent*>>> GetComponents(char(*)[(-(int)sizeof...(Components)) + 1] = 0)
		{
			std::unordered_map<uint32, std::vector<IComponent*>> componentsPerEntity = componentFamilies[GetTypeName<ComponentType>()]->GetComponents();

			std::unordered_map<std::type_index, std::unordered_map<uint32, std::vector<IComponent*>>> components;

			for (auto entity : componentsPerEntity)
				for (IComponent* component : entity.second)
					components[typeid(ComponentType)][entity.first].push_back(component);

			return components;
		}

		template<typename ComponentType, typename... Components>
		std::unordered_map<std::type_index, std::unordered_map<uint32, std::vector<IComponent*>>> GetComponents(char(*)[sizeof...(Components)] = 0)
		{
			std::unordered_map<uint32, std::vector<IComponent*>> componentsPerEntity = componentFamilies[GetTypeName<ComponentType>()]->GetComponents();

			std::unordered_map<std::type_index, std::unordered_map<uint32, std::vector<IComponent*>>> components = GetComponents<Components...>();

			for (auto entity : componentsPerEntity)
				for (IComponent* component : entity.second)
					components[typeid(ComponentType)][entity.first].push_back(component);

			return components;
		}
	};

	template<typename ComponentType, typename>
	void ComponentManager::RegisterComponentType()
	{
		std::string typeName = GetTypeName<ComponentType>();

		if (componentFamilies.count(typeName))
		{
			Debug::Warning(DebugInfo, "Registered component type %s multiple times", typeName.c_str());
			return;
		}

		uint32 id = (uint32)componentTypeIds.size() + 1;
		ComponentType::typeId = id;
		componentTypeIds[id] = typeName;
		componentFamilies[typeName] = std::unique_ptr<IComponentFamily>(new TypedComponentFamily<ComponentType>(id));

		Debug::Log(DebugInfo, "Registered component type %s with type id %i", typeName.c_str(), (int)id);
	}

	template<typename ComponentType, typename>
	inline void ComponentManager::RegisterGlobalComponentType()
	{
		std::string typeName = GetTypeName<ComponentType>();
		uint32 id = (uint32)staticComponentTypeIds.size() + 1;
		staticComponentTypeIds[id] = typeName;
		ComponentType::typeId = id;
		staticComponents[typeName] = std::unique_ptr<IGlobalComponent>(new ComponentType());

		Debug::Log(DebugInfo, "Registered static component type %s with type id %i", typeName.c_str(), (int)id);
	}

	template<typename ComponentType, typename>
	uint32 ComponentManager::AddComponent(uint32 entityID)
	{
		std::string typeName = GetTypeName<ComponentType>(); // oof loads of time
		return AddComponent(typeName, entityID); // more pain in here
	}

	template<typename ComponentType, typename>
	inline uint32 ComponentManager::AddComponent(uint32 entityID, ComponentType** componentHandle)
	{
		uint32 componentId = AddComponent<ComponentType>(entityID);
		*componentHandle = GetComponentByID<ComponentType>(componentId);
		return componentId;
	}

	template<typename ComponentType, typename>
	inline void ComponentManager::DestroyComponent(uint32 entityId, size_t index)
	{
		DestroyComponent(entityId, GetTypeName<ComponentType>(), index);
	}

	template<typename ComponentType, typename>
	inline void ComponentManager::DestroyComponent(uint32 componentId)
	{
		DestroyComponent(GetTypeName<ComponentType>(), componentId);
	}

	template<class SystemType, typename>
	const std::set<uint32>& ComponentManager::GetEntityList()
	{
		return entityLists[typeid(SystemType)];
	}

	template<typename ComponentType, typename>
	inline ComponentType* ComponentManager::GetGlobalComponent()
	{
		std::string typeName = GetTypeName<ComponentType>();
		if (staticComponents.find(typeName) != staticComponents.end())
			return static_cast<ComponentType*>(staticComponents[typeName].get());

		Debug::Error(DebugInfo, "Static component %s does not exist.", typeName.c_str());
		return nullptr;
	}

	template<typename ComponentType, typename>
	inline ComponentType* ComponentManager::GetComponent(uint32 entityId, size_t index)
	{
		std::string typeName = GetTypeName<ComponentType>();
#ifdef _DEBUG
		if (componentFamilies.count(typeName)) //GetTypeName is BIG oof
		{
			auto& componentFamily = componentFamilies[typeName];
			if (componentFamily)
				return static_cast<ComponentType*>(componentFamily->GetComponent(entityId, index));
		}

		Debug::Error(DebugInfo, "ComponentType %s was never registered", typeName.c_str());

		return nullptr;
#else
		return static_cast<ComponentType*>(componentFamilies[typeName]->GetComponent(entityId, index));
#endif // _DEBUG
		}

	template<typename ComponentType, typename>
	inline ComponentType* ComponentManager::GetComponentByID(uint32 componentId)
	{
		return static_cast<ComponentType*>(componentFamilies[GetTypeName<ComponentType>()]->GetComponentByID(componentId));
	}

	template<typename ComponentType, typename>
	inline size_t ComponentManager::GetComponentCount(uint32 entityId)
	{
		return componentFamilies[GetTypeName<ComponentType>()]->GetComponentCount(entityId);
	}

	template<typename ComponentType, typename>
	inline size_t ComponentManager::GetComponentCountOfType()
	{
		std::string typeName = GetTypeName<ComponentType>();
		if (!componentFamilies.count(typeName))
		{
			Debug::Error(DebugInfo, "Component type: \"%s\" does not exist or wasn't registered in the engine.", typeName.c_str());
			return 0;
		}

		return componentFamilies[typeName]->GetComponentCount();
	}

	template<typename ComponentType, typename>
	inline std::vector<ComponentType*> ComponentManager::GetComponentsOfType()
	{
		std::string typeName = GetTypeName<ComponentType>();
		if (GetComponentCountOfType<ComponentType>() == 0)
			return std::vector<ComponentType*>();

		std::vector<ComponentType*> ret;
		for (IComponent* component : componentFamilies[typeName]->GetComponentsList())
			ret.push_back(static_cast<ComponentType*>(component));
		return ret;
	}

	template<typename ComponentType, typename>
	inline std::vector<ComponentType*> ComponentManager::GetComponentsOfType(uint32 entityId)
	{
		std::vector<ComponentType*> ret;
		auto components = componentFamilies[GetTypeName<ComponentType>()]->GetComponents()[entityId];
		for (auto ptr : components)
			ret.push_back(static_cast<ComponentType*>(ptr));
		return ret;
	}

	}