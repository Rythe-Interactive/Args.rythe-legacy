#pragma once
#include "ECS/System.h"
#include "Components/Movement1Component.h"
#include "Base Components/Transform.h"
#include "Module/InputModule.h"
#include "Components/PickupComponent.h"
#include "Components/Rigidbody.h"
#include <Args-Physics.h>

namespace Args
{
	class Movement1System : public EntitySystem<Movement1System, Movement1Component, Transform, PickupComponent, Rigidbody>
	{
	private:
		bool firstframe = true;
	public:
		Movement1System() : EntitySystem<Movement1System, Movement1Component, Transform, PickupComponent, Rigidbody>() {}

		virtual void Init() override;
		void Update(float deltaTime);
		void DirectionX(Args::ControllerID controller, Args::AxisValue value);
		void DirectionY(Args::ControllerID controller, Args::AxisValue value);
		void Jump(Args::ControllerID controller, Args::AxisValue value);
		void Start();

		void OnCollision(const Collision& collision);
	};
}
