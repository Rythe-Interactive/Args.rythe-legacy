#pragma once
#include <unordered_map>
#include <memory>

namespace Args
{
	struct component_query_base;
	class component_family_base;

	class EntityComponentSystem
	{
	public:
		std::unordered_map<id_type, std::unique_ptr<component_query_base>> componentQueries;
		std::unordered_map<type_id, std::unique_ptr<component_family_base>> componentFamilies;
		std::unordered_map<id_type, entity_set> entityQueries;

		std::unordered_map<entity_id, type_set> entityContainedComponents;

		//template<typename... component_types>
		//component_query<component_types...>* GetQuery()
		//{
		//	return static_cast<component_query<component_types...>*>(componentQueries[component_query<component_types...>::id].get());
		//}
	};
}