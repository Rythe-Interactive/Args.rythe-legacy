#pragma once
#include "Components/Collider.h"
namespace Args
{
	class CollisionAlgorithm
	{
		public:
			virtual Collision CollisionDetect(Collider* _collider1, Transform* _transform1, Collider* _collider2, Transform* _transform2) = 0;
	};
}
