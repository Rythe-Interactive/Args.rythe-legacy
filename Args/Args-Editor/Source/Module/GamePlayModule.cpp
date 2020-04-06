#include "Module/GamePlayModule.h"
#include "Components/Movement1Component.h"
#include "Components/Movement2Component.h"
#include "Systems/Movement1System.h"
#include "Systems/Movement2System.h"
#include "Systems/DoorSystem.h"
#include "Systems/PickupSystem.h"
#include "Systems/PushSystem.h"
#include "Components/CheckGroundComponent.h"
#include "Components/EndPointComponent.h"
#include "Components/PickupAbleComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PushAbleComponent.h"


void Args::GamePlayModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<Args::Movement1System>(50);
	RegisterSystem<Args::Movement2System>(50);
	RegisterSystem<Args::PickupSystem>(50);
	RegisterSystem<Args::PushSystem>(50);
	RegisterSystem<Args::DoorSystem>(50);
}

void Args::GamePlayModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterComponentType<Args::Movement1Component>();
	RegisterComponentType<Args::Movement2Component>();
	RegisterComponentType<Args::PickupComponent>();
	RegisterComponentType<Args::DoorTriggerComponent>();
	RegisterComponentType<Args::ButtonTriggerComponent>();
	RegisterComponentType<Args::CheckGroundComponent>();
	RegisterComponentType<Args::EndPointComponent>();
	RegisterComponentType<Args::PickupAbleComponent>();
	RegisterComponentType<Args::PlayerComponent>();
	RegisterComponentType<Args::PushAbleComponent>();
	RegisterComponentType<Args::PushComponent>();
}


