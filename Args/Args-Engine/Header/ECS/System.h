#pragma once

#include <memory>
#include <array>
#include <set>
#include <functional>
#include "Utils/Common.h"
#include "ECS/Component.h"
#include "ECS/Managers/ComponentManager.h"
#include "ECS/Entity.h"

namespace Args
{
	class ISystem
	{
		friend class SystemManager;
		friend class std::unique_ptr<ISystem>;

	private:
		bool enabled = true;

	protected:
		float lastDeltaTime;

		ComponentManager* componentManager = nullptr;

		std::vector<std::tuple<float, float, std::function<void(float)>>> updateCallbacks;

		virtual void BindForUpdate(std::function<void(float)> func) = 0;
		virtual void BindForFixedUpdate(float interval, std::function<void(float)> func) = 0;
		virtual void Cleanup() {}

	public:
		virtual void Init() = 0;
		virtual void UpdateSystem(float deltaTime) = 0;
		virtual std::set<uint32> GetComponentRequirements() = 0;

		template<typename ComponentType>
		ComponentType* GetGlobalComponent();

		bool Enable(bool enabled) { this->enabled = enabled; }
	};

	template<typename ComponentType>
	inline ComponentType* ISystem::GetGlobalComponent()
	{
		return componentManager->GetGlobalComponent<ComponentType>();
	}

#pragma region Typed System updates once every interval
	template<class Self, class... Components>
	class MonoUpdateSystem : public ISystem
	{
	private:
		// TO DO: add actual component requirements
		static std::set<uint32> componentRequirements;

		template<bool activate>
		void SetComponentRequirements() {}

		template<>
		void SetComponentRequirements<false>()
		{
			static_assert((std::is_base_of_v<IComponent, Components> || ...), "One of the passed components doesn't inherit from Component.");

			(componentRequirements.insert(Components::typeId), ...);
		}

	public:
		virtual std::set<uint32> GetComponentRequirements() override
		{
			return componentRequirements;
		}

	protected:
		MonoUpdateSystem()
		{
			SetComponentRequirements<sizeof...(Components) == 0>();
		};

		std::set<uint32> GetEntityList()
		{
			return componentManager->GetEntityList<Self>();
		}

		template<typename ComponentType>
		ComponentType* GetComponent(uint32 entityID, size_t index = 0)
		{
			return componentManager->GetComponent<ComponentType>(entityID, index);
		}

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		std::vector<ComponentType*> GetComponentsOfType()
		{
			return componentManager->GetComponentsOfType<ComponentType>();
		}

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		std::vector<ComponentType*> GetComponentsOfType(uint32 entity)
		{
			return componentManager->GetComponentsOfType<ComponentType>(entity);
		}

		template<typename ComponentType>
		size_t GetComponentCount(uint32 entityId)
		{
			return componentManager->GetComponentCount<ComponentType>(entityId);
		}

		virtual void BindForUpdate(std::function<void(float)> func) override
		{
			updateCallbacks.push_back(std::make_tuple(0.f, 0.f, func));
		}

		virtual void BindForFixedUpdate(float interval, std::function<void(float)> func) override
		{
			updateCallbacks.push_back(std::make_tuple(interval, 0.f, func));
		}

		virtual void UpdateSystem(float deltaTime) override
		{
			lastDeltaTime = deltaTime;

			for (auto& [interval, timeBuffer, function] : updateCallbacks)
			{
				if (interval <= 0.001f)
				{
					function(deltaTime);

					continue;
				}

				timeBuffer += deltaTime;

				while (timeBuffer >= interval)
				{
					timeBuffer -= interval;

					function(interval);
				}
			}
		}
	};

	template<class Self, class... Components>
	std::set<uint32> MonoUpdateSystem<Self, Components...>::componentRequirements;

#pragma endregion


#pragma region Typed System updates per entity every interval
	template<class Self, class... Components>
	class EntitySystem : public ISystem
	{
	private:
		static std::set<uint32> componentRequirements;

		template<class ComponentType, class... ComponentTypes>
		void GetComponentsInternal(std::unordered_map<std::type_index, uint32>& typeCount, ComponentType** component, ComponentTypes**... components);

		template<class ComponentType>
		void GetComponentsInternal(std::unordered_map<std::type_index, uint32>& typeCount, ComponentType** component);

	public:
		virtual std::set<uint32> GetComponentRequirements() override;

	protected:
		EntitySystem();

		std::set<uint32> GetEntityList()
		{
			return componentManager->GetEntityList<Self>();
		}

		void GetComponents(Components**... components);

		template<typename ComponentType>
		size_t GetComponentCount()
		{
			return componentManager->GetComponentCount(currentEntityID);
		}

		template<typename ComponentType>
		ComponentType* GetComponent(uint32 entityID, size_t index = 0)
		{
			return componentManager->GetComponent<ComponentType>(entityID, index);
		}

		template<typename ComponentType>
		size_t GetComponentCount(uint32 entityId)
		{
			return componentManager->GetComponentCount<ComponentType>(entityId);
		}

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		std::vector<ComponentType*> GetComponentsOfType(uint32 entity)
		{
			return componentManager->GetComponentsOfType<ComponentType>(entity);
		}

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		std::vector<ComponentType*> GetComponentsOfType()
		{
			return componentManager->GetComponentsOfType<ComponentType>();
		}

		virtual void BindForUpdate(std::function<void(float)> func) override;
		virtual void BindForFixedUpdate(float interval, std::function<void(float)> func) override;

		uint32 currentEntityID = 0;
		Entity* entity;

		virtual void UpdateSystem(float deltaTime) override;
	};

	template<class Self, class... Components>
	std::set<uint32> EntitySystem<Self, Components...>::componentRequirements;

	template<class Self, class... Components>
	EntitySystem<Self, Components...>::EntitySystem()
	{
		static_assert((std::is_base_of_v<IComponent, Components> || ...), "One of the passed components doesn't inherit from Component.");

		(componentRequirements.insert(Components::typeId), ...);
	}

	template<class Self, class ...Components>
	void EntitySystem<Self, Components...>::UpdateSystem(float deltaTime)
	{
		lastDeltaTime = deltaTime;

		std::set<uint32> entities = componentManager->GetEntityList<Self>();

		for (auto& [interval, timeBuffer, function] : updateCallbacks)
		{
			if (interval <= 0.001f)
			{
				for (uint32 entityId : entities)
				{
					currentEntityID = entityId;
					entity = componentManager->GetEntityProxy(entityId);
					function(deltaTime);
					currentEntityID = 0;
					entity = nullptr;
				}

				continue;
			}

			timeBuffer += deltaTime;

			while (timeBuffer >= interval)
			{
				timeBuffer -= interval;

				for (uint32 entityId : entities)
				{
					currentEntityID = entityId;
					entity = componentManager->GetEntityProxy(entityId);
					function(interval);
					currentEntityID = 0;
					entity = nullptr;
				}
			}
		}
	}

	template<class Self, class ...Components>
	inline std::set<uint32> EntitySystem<Self, Components...>::GetComponentRequirements()
	{
		return componentRequirements;
	}

	template<class Self, class ...Components>
	void EntitySystem<Self, Components...>::GetComponents(Components**... components)
	{
		if (currentEntityID == 0)
		{
			Debug::Warning(DebugInfo, "%s: GetComponents should not be used outside the update loop, please use GetEntityList to itterate of relevant entities.", GetTypeName<Self>().c_str());
			return;
		}
		std::unordered_map<std::type_index, uint32> typeCount;

		GetComponentsInternal(typeCount, components...);
	}

	template<class Self, class ...Components>
	void EntitySystem<Self, Components...>::BindForUpdate(std::function<void(float)> func)
	{
		updateCallbacks.push_back(std::make_tuple(0.f, 0.f, func));
	}

	template<class Self, class ...Components>
	void EntitySystem<Self, Components...>::BindForFixedUpdate(float interval, std::function<void(float)> func)
	{
		updateCallbacks.push_back(std::make_tuple(interval, 0.f, func));
	}

	template<class Self, class ...Components>
	template<class ComponentType, class ...ComponentTypes>
	inline void EntitySystem<Self, Components...>::GetComponentsInternal(std::unordered_map<std::type_index, uint32>& typeCount, ComponentType** component, ComponentTypes** ...components)
	{
		*component = componentManager->GetComponent<ComponentType>(currentEntityID, typeCount[typeid(ComponentType)]);
		typeCount[typeid(ComponentType)]++;

		GetComponentsInternal(typeCount, components...);
	}

	template<class Self, class ...Components>
	template<class ComponentType>
	inline void EntitySystem<Self, Components...>::GetComponentsInternal(std::unordered_map<std::type_index, uint32>& typeCount, ComponentType** component)
	{
		*component = componentManager->GetComponent<ComponentType>(currentEntityID, typeCount[typeid(ComponentType)]);
		typeCount[typeid(ComponentType)]++;
	}
#pragma endregion
}
