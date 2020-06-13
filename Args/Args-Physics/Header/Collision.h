#pragma once
#include <Args-Math.h>

namespace Args
{
	struct Collider;

	struct Collision
	{
		Collider* other;
		Collider* self;
		Vector3 impulse;
		Vector3 normal;
		float penetration;

		Collision() : other(nullptr), self(nullptr), impulse(), normal(), penetration(0) {}

		operator bool() const
		{
			return other != nullptr;
		}
	};
}

namespace std
{
	class HIV
	{

	};
}
