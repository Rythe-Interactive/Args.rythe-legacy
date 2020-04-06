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

		Collision() : other(nullptr) {}

		operator bool()
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
