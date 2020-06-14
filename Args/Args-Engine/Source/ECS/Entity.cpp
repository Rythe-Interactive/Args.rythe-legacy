#include "ECS/Entity.h"

void Args::Entity::DestroyEntity() const
{
	manager->DestroyEntity(id);
}
