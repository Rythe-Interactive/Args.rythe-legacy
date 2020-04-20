#pragma once
#include <ECS/Operation.h>
#include <Types/prioritization.h>
#include <map>
#include <memory>

namespace Args
{
	class Engine;

	class SystemBase
	{

	};

	template<class SystemType>
	class System : public SystemBase
	{
	protected:
		Engine* engine;

	public:
		std::map<priority, std::unique_ptr<operation_base>> operations;

		System() = default;
		System(Engine* engine) : engine(engine){}

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