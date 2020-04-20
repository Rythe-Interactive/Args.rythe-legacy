#pragma once
#include <ECS/Operation.h>
#include <unordered_map>
#include <memory>

namespace Args
{
	class ECS;

	class SystenBase
	{

	};

	template<class SystemType>
	class System : public SystenBase
	{
	protected:
		ECS& ecs;

	public:
		std::unordered_map<id_type, std::unique_ptr<operation_base>> operations;

		System(ECS& ecs) : ecs(ecs){}
		System(const System&) = delete;

		virtual void init() = 0;

	/*	template <typename return_type, typename... component_types, return_type(SystemType::* func_type)(fast_seconds, component_types...)>
		void registerOperation()
		{
			using delegate_type = stl::delegate<return_type(fast_seconds, component_types...)>;
			using func_type = return_type(SystemType::* func_type)(fast_seconds, component_types...);

			if (!operations.count(operation<component_types...>::id))
				operations.insert(operation<component_types...>::id, std::make_unique<operation<component_types...>>());

			operations[operation<component_types...>::id]->insert(delegate_type::create<SystemType, func_type>(this))
		}*/
		
	};
}