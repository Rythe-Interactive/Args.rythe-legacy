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
		void RegisterSystem(uint32 priority = 999);

		template<typename ComponentType, INHERITS_FROM(ComponentType, IGlobalComponent)>
		void RegisterGlobalComponentType();

		template<typename ComponentType, INHERITS_FROM(ComponentType, IComponent)>
		void RegisterComponentType();

	public:
		Module(ECS& ecs) : ecs(ecs) {}

		virtual void InitComponents(const std::set<std::string>& arguments) = 0;

		virtual void InitSystems(const std::set<std::string>& arguments) = 0;

	};

	template<class SystemType, typename>
	inline void Module::RegisterSystem(uint32 priority)
	{
		ecs.RegisterSystem<SystemType>(priority);
	}

	template<typename ComponentType, typename>
	inline void Module::RegisterGlobalComponentType()
	{
		ecs.RegisterGlobalComponentType<ComponentType>();
	}

	template<typename ComponentType, typename>
	inline void Module::RegisterComponentType()
	{
		ecs.RegisterComponentType<ComponentType>();
	}
}