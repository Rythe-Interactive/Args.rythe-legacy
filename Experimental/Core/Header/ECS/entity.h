#pragma once
#include <Types/identification.h>
#include <ECS/ecs_containers.h>
#include <ECS/EntityComponentSystem.h>

namespace Args
{
	struct entity
	{
		entity_id id = invalid_id;
		EntityComponentSystem* ecs = nullptr;

		entity() = default;
		entity(const entity&) = default;
		entity(EntityComponentSystem* ecs, const entity_id& id) : ecs(ecs), id(id) {}

		type_set& containedComponents()
		{
			return ecs->entityContainedComponents[id];
		}

		operator entity_id() const { return id; }
		operator const entity_id& () const { return id; }
		operator entity_id& () { return id; }
		entity& operator=(const entity_id& id) { this->id = id; return *this; }
		entity& operator=(const entity& other) { this->id = other.id; return *this; }
	};
}