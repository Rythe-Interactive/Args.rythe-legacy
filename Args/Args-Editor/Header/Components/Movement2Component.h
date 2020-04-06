#pragma once
#include <Args-Core.h>
namespace Args
{
	struct Movement2Component : public Component<Movement2Component>
	{
		float MoveSpeed;
		Args::Vector3 CurrentVel;
		Args::Vector3 foward;
		float RotateSpeed;
		float JumpSpeed;
		bool Grounded;
		float DistToGround;
		float angle;
		float translation;

		Movement2Component(Entity* entity) : Args::Component<Movement2Component>(entity) 
		{
			CurrentVel = Args::Vector3(0,0,0);
			MoveSpeed = 20;
			RotateSpeed = 1;
			JumpSpeed = 10;
			Grounded = false;
			DistToGround = 1;
			angle = 1;
			translation = 1;
		};

		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}