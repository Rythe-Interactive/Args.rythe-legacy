#include <Helpers/AABB_AABB.h>

Args::Collision Args::AABB_AABB::CollisionDetect(Collider* _collider1, Transform* _transform1, Collider* _collider2, Transform* _transform2)
{
	Vector3 origin1 = _transform1->WorldTransformPoint(_collider1->origin);
	Vector3 origin2 = _transform2->WorldTransformPoint(_collider2->origin);


	Vector3 scale1 = _transform1->GetWorldScale();
	// Calculate half extents along each axis
	Vector3 maxExtents1 = origin1 + _collider1->size * 0.5f * scale1;
	Vector3 minExtents1 = origin1 - _collider1->size * 0.5f * scale1;

	Vector3 scale2 = _transform2->GetWorldScale();
	// Calculate half extents along each axis
	Vector3 maxExtents2 = origin2 + _collider2->size * 0.5f * scale2;
	Vector3 minExtents2 = origin2 - _collider2->size * 0.5f * scale2;

	Vector3 lengthBetweenObjects = origin2 - Args::clamp(origin2, minExtents1, maxExtents1);

	Vector3 overlaps;

	overlaps.x = Args::min(maxExtents1.x - minExtents2.x, maxExtents2.x - minExtents1.x);
	overlaps.y = Args::min(maxExtents1.y - minExtents2.y, maxExtents2.y - minExtents1.y);
	overlaps.z = Args::min(maxExtents1.z - minExtents2.z, maxExtents2.z - minExtents1.z);


	//normal calculation
	Vector3 normal;
	Args::Collision collision;

	if (Args::abs(lengthBetweenObjects.x) > Args::abs(lengthBetweenObjects.y))
	{
		normal = Vector3(lengthBetweenObjects.x, 0, 0);
		collision.penetration = overlaps.x;
	}
	else
	{
		normal = Vector3(0, lengthBetweenObjects.y, 0);
		collision.penetration = overlaps.y;
	}

	if (Args::length(normal) < Args::abs(lengthBetweenObjects.z))
	{
		normal = Vector3(0, 0, lengthBetweenObjects.z);
		collision.penetration = overlaps.z;
	}
	if (Args::length(normal) != 0)
		normal = Args::normalize(normal);

	if (normal.x != normal.x)
		Debug::Log(DebugInfo, "NAN found");

	if (overlaps.x > 0 && overlaps.y > 0 && overlaps.z > 0)
	{
		collision.normal = normal;
		collision.self = _collider1;
		collision.other = _collider2;
		collision.self = _collider1;
		return collision;
	}
	return collision;
}
