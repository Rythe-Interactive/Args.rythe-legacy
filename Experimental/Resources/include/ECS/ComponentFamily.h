#pragma once
#include <Utils/SFINAE.h>
#include <Types/identification.h>
#include <ECS/component.h>
#include <ECS/entity.h>
#include <ASTL/sparse_map.h>
#include <ASTL/default_index_list.h>

namespace Args
{
	class Engine;
	class ComponentFamilyBase
	{
	public:
		const type_id componentType;
		const type_id componentExternalType;
		Engine* engine;

		ComponentFamilyBase(Engine* engine, const type_id& componentType, const type_id& componentExternalType) : engine(engine), componentType(componentType), componentExternalType(componentExternalType) {}

		virtual void emplace(const entity& entity) = 0;
		virtual size_type erase(const entity& entity) = 0;
		virtual size_type size() = 0;
	};

	template<typename component_type, inherits_from<component_type, component_base> = 0>
	class ComponentFamily : public ComponentFamilyBase
	{
	public:
		static inline type_id componentType = component_type::type;
		static inline type_id componentExternalType = component_type::externalType;

		ComponentFamily(Engine* engine) : ComponentFamilyBase(engine, componentType, componentExternalType) {}

		stl::sparse_map<entity_id, component_type> components;

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