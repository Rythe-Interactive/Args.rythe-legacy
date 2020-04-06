#include "Systems/CameraMovementSystem.h"

void CameraMovementSystem::Init()
{
	/*GetGlobalComponent<Args::Input>()->BindAxis("MoveX", std::bind(&CameraMovementSystem::MoveX, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("MoveY", std::bind(&CameraMovementSystem::MoveY, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("MoveDown", std::bind(&CameraMovementSystem::MoveDown, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("MoveUp", std::bind(&CameraMovementSystem::MoveUp, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("RotateX", std::bind(&CameraMovementSystem::RotateX, this, std::placeholders::_1, std::placeholders::_2));
	GetGlobalComponent<Args::Input>()->BindAxis("RotateY", std::bind(&CameraMovementSystem::RotateY, this, std::placeholders::_1, std::placeholders::_2));
	*/BindForUpdate(std::bind(&CameraMovementSystem::Update, this, std::placeholders::_1));
	Args::Debug::Success(DebugInfo, "Initialised CameraMovementSystem");
}

void CameraMovementSystem::MoveX(Args::ControllerID controller, Args::AxisValue value)
{
	auto entities = GetEntityList();

	for (auto entity : entities)
	{
		Args::Transform* transform = GetComponent<Args::Transform>(entity);

		Args::Vector3 right = transform->GetRight();
		right.y = 0;

		transform->Move(Args::normalize(right) * value * lastDeltaTime * 2.f);
	}
}

void CameraMovementSystem::MoveY(Args::ControllerID controller, Args::AxisValue value)
{
	auto entities = GetEntityList();

	for (auto entity : entities)
	{
		Args::Transform* transform = GetComponent<Args::Transform>(entity);

		Args::Vector3 fwd = transform->GetForward();
		fwd.y = 0;

		transform->Move(Args::normalize(fwd) * value * -lastDeltaTime * 2.f);
	}
}

void CameraMovementSystem::MoveDown(Args::ControllerID controller, Args::AxisValue value)
{
	value = (value + 1.f) * 0.5f;

	auto entities = GetEntityList();

	for (auto entity : entities)
	{
		Args::Transform* transform = GetComponent<Args::Transform>(entity);

		transform->Move(Args::up * value * -lastDeltaTime * 2.f);
	}
}

void CameraMovementSystem::MoveUp(Args::ControllerID controller, Args::AxisValue value)
{
	value = (value + 1.f) * 0.5f;

	auto entities = GetEntityList();

	for (auto entity : entities)
	{
		Args::Transform* transform = GetComponent<Args::Transform>(entity);

		transform->Move(Args::up * value * lastDeltaTime * 2.f);
	}
}

void CameraMovementSystem::RotateX(Args::ControllerID controller, Args::AxisValue value)
{
	auto entities = GetEntityList();

	for (auto entity : entities)
	{
		Args::Transform* transform = GetComponent<Args::Transform>(entity);
		Args::Vector3 fwd = transform->GetForward();
		fwd = Args::rotate(fwd, value * lastDeltaTime * 2.f, Args::up);

		transform->SetRotation((Args::Matrix3)Args::inverse(Args::lookAtLH(Args::zero, fwd, Args::up)));
	}
}

void CameraMovementSystem::RotateY(Args::ControllerID controller, Args::AxisValue value)
{
	auto entities = GetEntityList();

	for (auto entity : entities)
	{
		Args::Transform* transform = GetComponent<Args::Transform>(entity);
		Args::Vector3 fwd = transform->GetForward();
		fwd = Args::rotate(fwd, value * lastDeltaTime * 2.f, transform->GetRight());

		transform->SetRotation((Args::Matrix3)Args::inverse(Args::lookAtLH(Args::zero, fwd, Args::up)));
	}
}

void CameraMovementSystem::Update(float deltaTime)
{
	Args::CameraMovementComponent* camMove;
	Args::Transform* transform;
	for(auto entity: GetEntityList())
	{
		camMove = GetComponent<Args::CameraMovementComponent>(entity);
		Args::Vector3 newCamPos;
		Args::Transform* player1 = GetComponent<Args::Transform>(camMove->Player1);
		Args::Transform* player2 = GetComponent<Args::Transform>(camMove->Player2);
		if(player1 != nullptr && player2 != nullptr)
		{
			newCamPos = (player2->GetPosition() + player1->GetPosition()) * 0.5f;
		}
		else if (player1 != nullptr)
		{
			newCamPos = player1->GetPosition();
		}
		else if(player2 != nullptr)
		{
			newCamPos = player2->GetPosition();
		}
		else
		{
			newCamPos = Args::Vector3(0, 10, 0);
		}
		newCamPos.y = camMove->Height;
		transform = GetComponent<Args::Transform>(entity);

		if (newCamPos.x != newCamPos.x)
			Args::Debug::Log(DebugInfo, "NAN found");

		transform->SetPosition(newCamPos);
	}
	

}
