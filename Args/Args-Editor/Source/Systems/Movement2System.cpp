#include <Systems/Movement2System.h>
#include <Components/Input.h>
#include "Components/Rigidbody.h"

void Args::Movement2System::Init()
{
	BindForUpdate(std::bind(&Movement2System::Update, this, std::placeholders::_1));
	GetGlobalComponent <Args::Input>()->BindAxis("RotateX",std::bind(&Movement2System::Rotate,this,std::placeholders::_1,std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("MoveY", std::bind(&Movement2System::Move,this,std::placeholders::_1,std::placeholders::_2));
}

void Args::Movement2System::Start()
{
}


void Args::Movement2System::Update(float deltaTime)
{
	Movement2Component* movement;
	Transform* transform;
	PushComponent* pushComp;
	GetComponents(&movement, &transform, &pushComp);
	transform->Move(movement->CurrentVel*deltaTime);
	if(!pushComp->Pushing)
	{
		transform->SetRotation((Args::Matrix3)Args::inverse(Args::lookAtLH(Args::zero, transform->GetForward(), Args::up)));
	}
	transform->SetForward(Args::rotate(transform->GetForward(), movement->angle * deltaTime, Args::up));
}

void Args::Movement2System::Jump(Args::ControllerID controller, Args::AxisValue value)
{
	for (auto entity : GetEntityList())
	{
		Movement2Component* movement = GetComponent<Movement2Component>(entity);
		if (movement != nullptr)
		{
			Rigidbody* rigidbody = GetComponent<Rigidbody>(entity);
			rigidbody->velocity += (Vector3(0, 1, 0) * movement->JumpSpeed);
		}
	}
}

void Args::Movement2System::Move(int controllerID, float dir)
{
	for (auto entity : GetEntityList())
	{
		Movement2Component* movement = GetComponent<Movement2Component>(entity);
		Transform* transform = GetComponent<Transform>(entity);
		Args::Vector3 fwd = transform->GetForward();
		fwd.y = 0;
		movement->CurrentVel = Args::normalize(fwd) * -dir;
	}
}

void Args::Movement2System::Rotate(int controlllerID,float angleStep)
{
	for (auto entity : GetEntityList())
	{
		Movement2Component* movement = GetComponent<Movement2Component>(entity);
		Transform* transform = GetComponent<Transform>(entity);

		movement->angle = -angleStep * movement->RotateSpeed;
	}
}

void Args::Movement2System::Print(float deltaTime)
{
}
