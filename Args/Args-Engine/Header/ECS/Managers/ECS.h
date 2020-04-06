#pragma once
#include "ComponentManager.h"
#include "SystemManager.h"

namespace Args
{
	class ECS
	{
	private:
		ComponentManager componentManager;
		SystemManager systemManager;
	public:

		ECS() : componentManager(), systemManager()
		{
			componentManager.systems = &(systemManager.systems);
		}

		~ECS()
		{
			componentManager.Destroy();
			systemManager.Destroy();
		}

		size_t GetEntityCount();

		template<class ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent(uint32 entityId);

		template<class ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent(uint32 entityId, ComponentType** componentHandle);

		uint32 CreateEntity();

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		void RegisterSystem(uint32 priority = 999);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IGlobalComponent)>
		void RegisterGlobalComponentType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void RegisterComponentType();

		void InitialiseSystems();

		void UpdateSystems();

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		static void EnableSystem(bool enabled);
	};


	template<typename ComponentType, typename>
	inline uint32 ECS::AddComponent(uint32 entityId)
	{
		return componentManager.AddComponent<ComponentType>(entityId);
	}

	template<class ComponentType, typename>
	inline uint32 ECS::AddComponent(uint32 entityId, ComponentType** componentHandle)
	{
		return componentManager.AddComponent<ComponentType>(entityId, componentHandle);
	}

	template<class SystemType, typename>
	inline void ECS::RegisterSystem(uint32 priority)
	{
		systemManager.RegisterSystem<SystemType>(&componentManager, priority);
	}

	template<typename ComponentType, typename>
	inline void ECS::RegisterGlobalComponentType()
	{
		componentManager.RegisterGlobalComponentType<ComponentType>();
	}

	template<typename ComponentType, typename>
	inline void ECS::RegisterComponentType()
	{
		componentManager.RegisterComponentType<ComponentType>();
	}

	template<class SystemType, typename>
	inline void ECS::EnableSystem(bool enabled)
	{
		systemManager.EnableSystem<SystemType>();
	}
}
