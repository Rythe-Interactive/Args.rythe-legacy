#pragma once
#include "ECS/System.h"
#include "Components/Collider.h"
#include "Base Components/Transform.h"
#include <Args-Input.h>
#include <Args-Physics.h>
#include "Components/PushComponent.h"

namespace Args
{
	class PushSystem : public EntitySystem<PushSystem, Collider, Transform, PushComponent>
	{
	public:
		PushSystem() : EntitySystem<PushSystem, Collider, Transform, PushComponent>() {}

		virtual void Init() override;
		//void Update(float deltaTime);
		void OnTriggerStay(const Collision& collision);
		void OnTriggerExit(const Collision& collision);
	};
}
