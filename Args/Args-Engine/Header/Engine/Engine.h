#pragma once
#include <vector>
#include <memory>
#include <set>
#include <functional>
#include <unordered_map>
#include <typeindex>

#include "Utils/Common.h"
#include "Module.h"
#include "ECS/Managers/ECS.h"
#include "Events/Event.h"

namespace Args
{
	class Engine
	{
	private:
		std::vector<std::unique_ptr<Module>> modules;
		ECS* ecs;

		std::set<std::string> commandlineArguments;
		static std::set<uint32> events;

		static std::unordered_map<std::type_index, std::vector<std::function<void(Args::IEvent*)>>> eventCallbacks;
	public:

		Engine(int argc, char* argv[]);
		Engine();
		~Engine();

		void Initialise();

		template<typename EventType, typename... Arguments>
		static void RaiseEvent(Arguments... arguments);

		template<typename EventType, INHERITS_FROM(EventType, IEvent)>
		static bool CheckEvent();

		template<typename EventType, INHERITS_FROM(EventType, IEvent)>
		static void BindToEvent(std::function<void(Args::IEvent*)> callback);

		template<typename ModuleType, INHERITS_FROM(ModuleType, Module)>
		void AttachModule();

		void Run();

		template<class ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent(uint32 entityId);

		template<class ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		uint32 AddComponent(uint32 entityId, ComponentType** componentHandle);

		uint32 CreateEntity()
		{
			return ecs->CreateEntity();
		}

		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		void RegisterSystem(uint32 priority = 999);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IGlobalComponent)>
		void RegisterGlobalComponentType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void RegisterComponentType();
	};

	template<typename EventType, typename... Arguments>
	inline void Engine::RaiseEvent(Arguments... arguments)
	{
		events.insert(EventType::id);

		EventType event = EventType(arguments...);

		for (auto callback : eventCallbacks[typeid(EventType)])
			callback(&event);
	}

	template<typename EventType, typename>
	inline bool Engine::CheckEvent()
	{
		return events.count(EventType::id);
	}

	template<typename EventType, typename>
	inline void Engine::BindToEvent(std::function<void(Args::IEvent*)> callback)
	{
		eventCallbacks[typeid(EventType)].push_back(callback);
	}

	template<typename ModuleType, typename>
	inline void Engine::AttachModule()
	{
		modules.push_back(std::make_unique<ModuleType>(*ecs));
	}

	template<typename ComponentType, typename>
	inline uint32 Engine::AddComponent(uint32 entityId)
	{
		return ecs->AddComponent<ComponentType>(entityId);
	}

	template<class ComponentType, typename>
	inline uint32 Engine::AddComponent(uint32 entityId, ComponentType** componentHandle)
	{
		return ecs->AddComponent<ComponentType>(entityId, componentHandle);
	}

	template<class SystemType, typename>
	inline void Engine::RegisterSystem(uint32 priority)
	{
		ecs->RegisterSystem<SystemType>(priority);
	}

	template<typename ComponentType, typename>
	inline void Engine::RegisterGlobalComponentType()
	{
		ecs->RegisterGlobalComponentType<ComponentType>();
	}

	template<typename ComponentType, typename>
	inline void Engine::RegisterComponentType()
	{
		ecs->RegisterComponentType<ComponentType>();
	}
}