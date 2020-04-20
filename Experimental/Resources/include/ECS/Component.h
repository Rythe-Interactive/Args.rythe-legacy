#pragma once
#include <ECS/Entity.h>

namespace Args
{
	class ECS;

	struct component_base
	{
	protected:
		ECS& ecs;

	public:
		const type_id type;
		const type_id externalType;
		const entity owner;
		const component_id id;

		component_base(ECS& ecs, const entity& owner, const component_id& id, const type_id& type, const type_id& externalType)
			: ecs(ecs), owner(owner), id(id), type(type), externalType(externalType)
		{
		}
	};

	template<typename component_type>
	struct component : public component_base
	{
		static type_id type;
		static type_id externalType;

		component(ECS& ecs, const entity& owner, const component_id& id)
			: component_base(ecs, owner, id, type, externalType)
		{
		}
	};

	 template<typename component_type>
	 type_id component<component_type>::type = invalid_id;

	 template<typename component_type>
	 type_id component<component_type>::externalType = invalid_id;
}