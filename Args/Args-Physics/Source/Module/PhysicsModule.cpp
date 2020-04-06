#include <Module/PhysicsModule.h>
#include "Components/Collider.h"
#include "Components/Rigidbody.h"
#include "Systems/CollisionSystem.h"
#include "Systems/PhysicsSystem.h"

void Args::PhysicsModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterComponentType<Collider>();
	RegisterComponentType<Rigidbody>();
}

void Args::PhysicsModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<CollisionSystem>();
	RegisterSystem<PhysicsSystem>();
}