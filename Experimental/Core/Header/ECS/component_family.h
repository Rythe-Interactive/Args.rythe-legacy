#pragma once
#include <Types/SFINAE.h>
#include <Types/identification.h>
#include <ECS/component.h>
#include <ECS/ecs_containers.h>
#include <ECS/entity.h>

namespace Args
{
	class Engine;
	class component_family_base
	{
	public:
		const type_id componentType;
		const type_id componentExternalType;
		Engine* engine;

		component_family_base(Engine* engine, const type_id& componentType, const type_id& componentExternalType) : engine(engine), componentType(componentType), componentExternalType(componentExternalType) {}

		virtual void emplace(const entity& entity) = 0;
		virtual size_type erase(const entity& entity) = 0;
		virtual size_type size() = 0;
	};

	template<typename component_type, inherits_from<component_type, component_base> = 0>
	class component_family : public component_family_base
	{
	public:
		static inline type_id componentType = component_type::type;
		static inline type_id componentExternalType = component_type::externalType;

		component_family(Engine* engine) : component_family_base(engine, componentType, componentExternalType) {}

		component_container<component_type> components;

		virtual void emplace(const entity& entity) override
		{
			components.emplace(entity, engine, entity);
		}

		virtual size_type erase(const entity& entity) override
		{
			return components.erase(entity);
		}
	
		virtual size_type size() override
		{
			return components.size();
		}
	};
}