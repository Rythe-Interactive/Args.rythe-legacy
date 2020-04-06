#pragma once
#include <Systems/PushSystem.h>
#include "Components/PushAbleComponent.h"

void Args::PushSystem::Init()
{
	for(auto entity: GetEntityList())
	{
		GetComponent<Collider>(entity)->OnCollisionStayCallback.push_back(std::bind(&PushSystem::OnTriggerStay, this, std::placeholders::_1));
		GetComponent<Collider>(entity)->OnCollisionStayCallback.push_back(std::bind(&PushSystem::OnTriggerExit, this, std::placeholders::_1));
	}
}

void Args::PushSystem::OnTriggerStay(const Collision& collision)
{
	if(collision.other->owner->GetComponent<PushAbleComponent>())
	{
		for(auto entity : GetEntityList())
		{
			GetComponent<PushComponent>(entity)->Pushing = true;
		}
	}
}

void Args::PushSystem::OnTriggerExit(const Collision& collision)
{
	if (collision.other->owner->GetComponent<PushAbleComponent>())
	{
		for (auto entity : GetEntityList())
		{
			GetComponent<PushComponent>(entity)->Pushing = false;
		}
	}
}

