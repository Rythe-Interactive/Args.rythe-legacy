#pragma once
#include "ECS/System.h"
#include "Components/Collider.h"
#include "Base Components/Transform.h"
#include <Args-Input.h>
#include <Components/PickupComponent.h>
#include <Args-Physics.h>

namespace Args
{
	class PickupSystem : public EntitySystem<PickupSystem, Collider, Transform, PickupComponent>
	{
	private:
		bool firstFrame = true;
	public:
		PickupSystem() : EntitySystem<PickupSystem, Collider, Transform, PickupComponent>() {}

		virtual void Init() override;
		void Update(float deltaTime);
		void OnTriggerStay(const Collision& collision);
		void OnTriggerExit(const Collision& collision);
		void Grab(Args::ControllerID controller, Args::AxisValue value);
	};
}