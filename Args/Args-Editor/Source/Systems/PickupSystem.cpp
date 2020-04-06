#include <Systems/PickupSystem.h>
#include "Components/PickupAbleComponent.h"

void Args::PickupSystem::Init()
{
	BindForUpdate(std::bind(&PickupSystem::Update, this, std::placeholders::_1));
	GetGlobalComponent<Args::Input>()->BindAxis("MoveUp", std::bind(&PickupSystem::Grab, this, std::placeholders::_1, std::placeholders::_2));
	

}

void Args::PickupSystem::Update(float deltaTime)
{
	if (firstFrame)
	{
		Collider* collider;
		for (auto entity : GetEntityList())
		{
			collider = GetComponent<Collider>(entity);
			if (GetComponent<PickupComponent>(entity))
			{
				collider->OnCollisionStayCallback.push_back(std::bind(&PickupSystem::OnTriggerStay, this, std::placeholders::_1));
				collider->OnCollisionEndCallback.push_back(std::bind(&PickupSystem::OnTriggerExit, this, std::placeholders::_1));
			}
		}
		firstFrame = false;
	}
}

void Args::PickupSystem::Grab(Args::ControllerID controller, Args::AxisValue value)
{
	for (auto entity : GetEntityList())
	{
		PickupComponent* pickup = GetComponent<PickupComponent>(entity);
		Transform* transform = GetComponent<Transform>(entity);
		Rigidbody* rigidbody = GetComponent<Rigidbody>(entity);
		if (pickup->inFront)
		{
			value = (value + 1.f) * 0.5f;
			if (value != 0)
			{
				Debug::Log(DebugInfo, "Button amount Pressed: %f", value);		
				if (pickup->PickedUpObject != NULL)
				{
					GetComponent<PickupComponent>(entity)->PickingUp = true;
					GetComponent<Rigidbody>(pickup->PickedUpObject)->entitiesToIgnore.insert(entity);
					rigidbody->entitiesToIgnore.insert(pickup->PickedUpObject);
					GetComponent<Transform>(pickup->PickedUpObject)->SetPosition(transform->position + transform->GetForward() * 5.0f);
				}
				
			}
			else if (value <= 0 && GetComponent<PickupComponent>(entity)->PickingUp)
			{
				if (pickup->PickedUpObject != NULL)
				{
					GetComponent<Rigidbody>(pickup->PickedUpObject)->entitiesToIgnore.erase(entity);
					rigidbody->entitiesToIgnore.erase(pickup->PickedUpObject);
					rigidbody->velocity += transform->GetForward() * pickup->HorThrowPow;
					rigidbody->velocity += transform->GetUp() * pickup->VertThrowPow;
					pickup->PickedUpObject = NULL;
				}
			}
		}
	}
}

void Args::PickupSystem::OnTriggerStay(const Collision& collision)
{
	if (collision.other->owner->GetComponent<PickupAbleComponent>())
	{
		collision.self->owner->GetComponent<PickupComponent>()->inFront = true;
		collision.self->owner->GetComponent<PickupComponent>()->PickedUpObject = collision.other->ownerID;
		GetComponent<Rigidbody>(collision.self->owner->GetComponent<PickupComponent>()->PickedUpObject)->entitiesToIgnore.insert(collision.self->ownerID);
		GetComponent<Rigidbody>(collision.self->id)->entitiesToIgnore.insert(collision.self->owner->GetComponent<PickupComponent>()->PickedUpObject);
	}
}

void Args::PickupSystem::OnTriggerExit(const Collision& collision)
{
	if (collision.other->owner->GetComponent<PickupAbleComponent>())
	{
		collision.self->owner->GetComponent<PickupComponent>()->inFront = false;
		//GetComponent<Rigidbody>(collision.self->owner->GetComponent<PickupComponent>()->PickedUpObject)->entitiesToIgnore.erase(collision.self->id);
		//GetComponent<Rigidbody>(collision.self->id)->entitiesToIgnore.erase(collision.self->id);
	}
}