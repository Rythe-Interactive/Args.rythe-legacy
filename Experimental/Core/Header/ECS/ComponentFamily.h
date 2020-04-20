#pragma once
#include <Utils/SFINAE.h>
#include <Types/identification.h>
#include <ECS/component.h>
#include <ECS/entity.h>
#include <ASTL/sparse_map.h>
#include <ASTL/default_index_list.h>

namespace Args
{
	class ComponentFamilyBase
	{
		virtual void CreateComponent(entity* entity) = 0;
		virtual void DestroyComponent(uint32 entityId, size_t index = 0) = 0;
	};

	template<typename component_type, inherits_from<component_type, component_base> = 0>
	class ComponentFamily : public ComponentFamilyBase
	{
	public:
		stl::sparse_map<entity_id, stl::default_index_list<component_type>> components;


	};
}