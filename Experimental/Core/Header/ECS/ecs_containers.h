#pragma once
#include <ASTL/sparse_map.h>
#include <ASTL/sparse_set.h>
#include <Types/identification.h>
#include <Types/SFINAE.h>

namespace Args
{
	struct component_base;

	template<typename component_type, inherits_from<component_type, component_base> = 0>
	using component_container = stl::sparse_map<entity_id, component_type>;

	using entity_set = stl::sparse_set<entity_id>;
	using type_set = stl::sparse_set<type_id>;
}