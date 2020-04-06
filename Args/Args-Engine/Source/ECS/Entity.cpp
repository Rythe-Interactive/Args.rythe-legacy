#include "ECS/Entity.h"

void Args::Entity::DestroyEntity()
{
	manager->DestroyEntity(id);
}
