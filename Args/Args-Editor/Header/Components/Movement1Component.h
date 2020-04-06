#pragma once
#include "Args-Math.h"

namespace Args
{
	struct Movement1Component : public Component<Movement1Component>
	{
		float MoveSpeed;
		float RotateSpeed;
		float JumpSpeed;
		bool Grounded;
		Vector3 Direction;
		Vector3 DeltaTurn;

		Movement1Component(Entity* entity) : Args::Component<Movement1Component>(entity)
		{
			MoveSpeed = 5;
			RotateSpeed = 10;
			JumpSpeed = 10.f;
			Direction = Vector3(0);
			DeltaTurn = Vector3(0);
		}

		// Inherited via Component
		virtual std::string ObjectType() override
		{
			return std::string();
		}

		virtual bool SetData(const std::string& name, const std::string& value) override
		{
			return false;
		}

		virtual bool GetData(const std::string& name, std::string& value) override
		{
			return false;
		}
	};
}
