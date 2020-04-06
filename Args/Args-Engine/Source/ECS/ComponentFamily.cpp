#include "ECS/ComponentFamily.h"
#include "ECS/Entity.h"

Args::uint32 Args::IComponentFamily::GetEntityID(const Entity* entity) const
{
	return entity->id;
}
