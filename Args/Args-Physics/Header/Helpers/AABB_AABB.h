#pragma once
#include <Helpers/CollisionAlgorithm.h>

namespace Args
{
	class AABB_AABB : public CollisionAlgorithm
	{
		Collision CollisionDetect(Collider* _collider1, Transform* _transform1, Collider* _collider2, Transform* _transform2) override;
	};
}
