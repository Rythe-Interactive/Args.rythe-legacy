#pragma once
#include <Types/identification.h>
#include <ECS/ecs_containers.h>

namespace Args
{
	class Engine;
	struct entity
	{
		entity_id id;
		type_set componentTypes;
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