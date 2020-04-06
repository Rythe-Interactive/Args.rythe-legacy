#include <Systems/Movement1System.h>
#include <Args-Input.h>

void Args::Movement1System::Init()
{
	BindForUpdate(std::bind(&Movement1System::Update, this, std::placeholders::_1));
	Debug::Success(DebugInfo, "Initialised Movement1System");
	GetGlobalComponent<Args::Input>()->BindAxis("MoveX", std::bind(&Movement1System::DirectionX, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("MoveY", std::bind(&Movement1System::DirectionY, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAction("Exit", std::bind(&Movement1System::Jump, this, std::placeholders::_1, std::placeholders::_2));

}

void Args::Movement1System::DirectionX(Args::ControllerID controller, Args::AxisValue value)
{
	for (auto entity : GetEntityList())
	{
		GetComponent<Movement1Component>(entity)->Direction.x = -value;
	}
}

void Args::Movement1System::DirectionY(Args::ControllerID controller, Args::AxisValue value)
{
	for (auto entity : GetEntityList())
	{
		GetComponent<Movement1Component>(entity)->Direction.z = value;
	}
}

void Args::Movement1System::Jump(Args::ControllerID controller, Args::AxisValue value)
{
	for (auto entity : GetEntityList())
	{
		Movement1Component* movement = GetComponent<Movement1Component>(entity);
		if (movement != nullptr && movement->Grounded)
		{
			Rigidbody* rigidbody = GetComponent<Rigidbody>(entity);

			rigidbody->impulses.push_back(Vector3(0, 1, 0) * movement->JumpSpeed);
			movement->Grounded = false;
		}
	}
}

void Args::Movement1System::Update(float deltaTime)
{
	if (firstframe)
		for (auto entity : GetEntityList())
			GetComponent<Collider>(entity)->OnCollisionCallback.push_back(std::bind(&Movement1System::OnCollision, this, std::placeholders::_1));
	firstframe = false;

	Movement1Component* movement;
	Transform* transform;
	PickupComponent* pickup;
	Rigidbody* rigidbody;
	GetComponents(&movement, &transform, &pickup, &rigidbody);
	//Debug::Log(DebugInfo, "X: %f", transform->position.x);
	//Debug::Log(DebugInfo, "Y: %f", transform->position.y);
	//Debug::Log(DebugInfo, "Z: %f", transform->position.z);

	if (transform->position.x != transform->position.x)
		Debug::Log(DebugInfo, "NAN found");

	if (length2(movement->Direction) > 0.01f)
	{
		movement->DeltaTurn = normalize(transform->GetForward() + movement->Direction * movement->RotateSpeed * deltaTime);
		transform->SetForward(movement->DeltaTurn);
		/*if(!pickup->PickingUp)
		{*/
		transform->Move(transform->GetForward() * deltaTime * movement->MoveSpeed);
		//}
	}

	if (transform->position.x != transform->position.x)
		Debug::Log(DebugInfo, "NAN found");

}

void Args::Movement1System::Start()
{
}

void Args::Movement1System::OnCollision(const Collision& collision)
{
	for (auto entity : GetEntityList())
	{
		Movement1Component* movement = GetComponent<Movement1Component>(entity);
		movement->Grounded = true;
	}
	//Debug::Log(DebugInfo, "SAAAH");
}

