#pragma once
#include <Types/identification.h>
#include <ASTL/sparse_set.h>

namespace Args
{
	class ECS;
	struct entity
	{
		entity_id id;
		stl::sparse_set<type_id> componentTypes;
		ECS& ecs;

		entity(ECS& ecs, const entity_id& id) : ecs(ecs), id(id){}

		operator entity_id() const { return id; }
		operator const entity_id&() const { return id; }
		operator entity_id&() { return id; }
		entity& operator=(const entity_id& id) { this->id = id; return *this; }
		entity& operator=(const entity& other) { this->id = other.id; return *this; }
	};
}