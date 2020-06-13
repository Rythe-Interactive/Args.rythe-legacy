#pragma once
#include <set>
#include <string>
#include <ECS/Managers/ECS.h>

namespace Args
{
	class Module
	{
	private:
		ECS& ecs;

	protected:
		template<class SystemType, INHERITS_FROM(SystemType, ISystem)>
		void RegisterSystem(uint32 priority = 999) const;

		template<typename ComponentType, INHERITS_FROM(ComponentType, IGlobalComponent)>
		void RegisterGlobalComponentType() const;

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void RegisterComponentType() const;

	public:
		Module(ECS& ecs) : ecs(ecs) {}

		virtual void InitComponents(const std::set<std::string>& arguments) = 0;

		virtual void InitSystems(const std::set<std::string>& arguments) = 0;

	};

	template<class SystemType, typename>
	inline void Module::RegisterSystem(uint32 priority) const
	{
		ecs.RegisterSystem<SystemType>(priority);
	}

	template<typename ComponentType, typename>
	inline void Module::RegisterGlobalComponentType() const
	{
		ecs.RegisterGlobalComponentType<ComponentType>();
	}

	template<typename ComponentType, typename>
	inline void Module::RegisterComponentType() const
	{
		ecs.RegisterComponentType<ComponentType>();
	}
}