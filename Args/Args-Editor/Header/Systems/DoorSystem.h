#pragma once
#include "ECS/System.h"
#include "Components/DoorTriggerComponent.h"
#include "Components/PressureTriggerComponent.h"
#include "Components/ButtonTriggerComponent.h"
#include "Components/Collider.h"
#include "Module/InputModule.h"
#include "Base Components/Transform.h"
#include "Components/Rigidbody.h"

namespace Args
{
	class DoorSystem : public EntitySystem<DoorSystem, DoorTriggerComponent,Transform>
	{
	public:
		DoorSystem() : EntitySystem<DoorSystem, DoorTriggerComponent,Transform>() {}

		virtual void Init() override;
		void Update(float deltaTime);
		void Start();
		void OpenDoor();
		void CloseDoor();
	};
}
