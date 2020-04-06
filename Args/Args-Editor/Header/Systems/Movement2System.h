#pragma once
#include <ECS/System.h>
#include <Components/Movement2Component.h>
#include <Base Components/Transform.h>
#include <functional>
#include "Components/PushComponent.h"
#include "Module/InputModule.h"

namespace Args
{
	class Movement2System : public EntitySystem<Movement2System, Movement2Component,Transform, PushComponent>
	{
	public:
		Movement2System() : Args::EntitySystem<Movement2System, Movement2Component,Transform, PushComponent>() {}

		virtual void Init() override;

		void Start();

		void Jump(Args::ControllerID controller, Args::AxisValue value);
		
		void Update(float deltaTime);

		void Move(int controllerID, float speed);

		void Rotate(int controllerID,float angleStep);

		void Print(float deltaTime);
	};
}
