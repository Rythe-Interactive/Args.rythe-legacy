#pragma once
#include<ECS/Component.h>
#include <array>
#include <Args-Math.h>
#include <vector>

namespace Args
{
	struct Rigidbody : public Component<Rigidbody>
	{
		float restitution;
		Vector3 prevPos;
		Vector3 velocity;
		std::vector<Vector3> forces;
		std::vector<Vector3> impulses;
		
		std::set<uint32> entitiesToIgnore;

		Rigidbody(Entity* entity) : Component<Rigidbody>(entity), velocity(), forces(), impulses(), restitution(0.5f)
		{
			//forces.push_back(Vector3(0, -4.905f, 0));
			forces.push_back(Vector3(0, -9.81f, 0));
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
