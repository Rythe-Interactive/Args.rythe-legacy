#include <Helpers/Sphere_Sphere.h>
#include <Args-Math.h>

Args::Collision Args::Sphere_Sphere::CollisionDetect(Collider* _collider1, Transform* _transform1, Collider* _collider2, Transform* _transform2)
{
	Vector3 origin1 = _transform1->WorldTransformPoint(_collider1->origin);
	Vector3 origin2 = _transform2->WorldTransformPoint(_collider2->origin);

	float sqrDistance = distance2(origin1, origin2);

	Args::Collision collision;
	float combinedRadius = _collider1->size.x * _transform1->GetWorldScale().x + _collider2->size.x * _transform2->GetWorldScale().x;
	if(sqrDistance < (combinedRadius * combinedRadius))
	{
		Vector3 normal = origin1 - origin2;
		if (Args::length(normal) != 0)
			normal = Args::normalize(normal);
		collision.normal = normal;
		collision.other = _collider2;
		collision.penetration = combinedRadius - sqrt(sqrDistance);
		return collision;
	}
	collision.other = nullptr;
	return collision;
}
