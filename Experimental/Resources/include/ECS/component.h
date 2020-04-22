#pragma once
#include <ECS/Entity.h>
#include <Types/identification.h>

namespace Args
{
	class Engine;

	struct component_base
	{
	protected:
		Engine* engine;
		static inline type_id lastId = invalid_id;
	public:
		type_id type;
		type_id externalType;
		entity owner;

		component_base() = default;
		component_base(Engine* engine, const entity& owner, const type_id& type, const type_id& externalType)
			: engine(engine), owner(owner), type(type), externalType(externalType)
		{
		}
	};

	template<typename component_type>
	struct component : public component_base
	{
		static type_id type;
		static type_id externalType;

		component() = default;
		component(Engine* engine, const entity& owner)
			: component_base(engine, owner, type, externalType)
		{
		}
	};

	template<typename component_type>
	type_id component<component_type>::type = ++lastId;

	template<typename component_type>
	type_id component<component_type>::externalType = GetTypeHash<component_type>();
}