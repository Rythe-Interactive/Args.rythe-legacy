#pragma once
#include <vector>
#include <tuple>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <typeindex>
#include <memory>
#include <functional>

#include "Utils/Common.h"
#include "ECS/Component.h"
#include "ECS/System.h"
#include "Time/Clock.h"

namespace Args
{
	class SystemManager
	{
		friend class ECS;

	private:
		std::unordered_map<std::type_index, std::unique_ptr<ISystem>> systems;
		std::map<uint32, std::vector<std::type_index>> systemPriorities;

		Clock updateClock;

		void Destroy();

	public:

		SystemManager();

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		void RegisterSystem(ComponentManager* componentManager, uint32 priority = 999);

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		SystemType* GetSystem();

		ISystem* GetSystem(std::type_index systemType);

		std::vector<ISystem*> GetSystems();

		void InitialiseSystems();

		void UpdateSystems();

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		static void EnableSystem(bool enabled);
	};

	template<class SystemType, typename>
	void SystemManager::RegisterSystem(ComponentManager* componentManager, uint32 priority)
	{
		systems[typeid(SystemType)] = std::unique_ptr<ISystem>(new SystemType());
		systems[typeid(SystemType)]->componentManager = componentManager;
		systemPriorities[priority].push_back(typeid(SystemType));
		Debug::Log(DebugInfo, "Registered system of type: %s", GetTypeName<SystemType>().c_str());
	}

	template<class SystemType, typename>
	inline SystemType* SystemManager::GetSystem()
	{
		return systems[typeid(SystemType)].get();
	}

	template<class SystemType, typename>
	void SystemManager::EnableSystem(bool enabled)
	{
		systems[typeid(SystemType)]->Enable(enabled);
	}
}