#pragma once
#include <Types/identification.h>
#include <ASTL/sparse_set.h>

namespace Args
{
	class Engine;
	struct entity
	{
		entity_id id;
		stl::sparse_set<type_id> componentTypes;
		Engine* engine;

		entity() = default;
		entity(Engine* engine, const entity_id& id) : engine(engine), id(id) {}

		operator entity_id() const { return id; }
		operator const entity_id& () const { return id; }
		operator entity_id& () { return id; }
		entity& operator=(const entity_id& id) { this->id = id; return *this; }
		entity& operator=(const entity& other) { this->id = other.id; return *this; }
	};
}