#include <Systems/CollisionSystem.h>
#include "Helpers/AABB_AABB.h"
#include "Helpers/AABB_Sphere.h"
#include "Helpers/Sphere_Sphere.h"
#include <ECS/Managers/ComponentManager.h>
#include <algorithm>


void Args::CollisionSystem::Init()
{
	BindForFixedUpdate(1.f / 160.f, std::bind(&CollisionSystem::UpdateColliders, this, std::placeholders::_1));

	Debug::Success(DebugInfo, "Initialised CollisionSystem");
}

Args::CollisionSystem::CollisionSystem() : Args::MonoUpdateSystem<CollisionSystem, Collider, Transform>()
{
	lookUpAlgorithm[ColliderType::Box][ColliderType::Box] = new AABB_AABB();
	lookUpAlgorithm[ColliderType::Box][ColliderType::Sphere] = new AABB_Sphere();
	lookUpAlgorithm[ColliderType::Sphere][ColliderType::Sphere] = new Sphere_Sphere();
}

Args::CollisionSystem::~CollisionSystem()
{
	for (auto map : lookUpAlgorithm)
		for (auto algorithm : map.second)
		{
			delete algorithm.second;
			algorithm.second = nullptr;
		}

	lookUpAlgorithm.clear();
}


void Args::CollisionSystem::UpdateColliders(float deltaTime)
{
	std::vector<Collider*> colliders;
	std::unordered_map<uint32, Transform*> transforms;
	auto entities = GetEntityList();
	for (auto entity : entities)
	{
		transforms[entity] = GetComponent<Transform>(entity);

		for (int i = 0; i < (int)GetComponentCount<Collider>(entity); i++)
			colliders.push_back(GetComponent<Collider>(entity, i));
	}

	std::unordered_map<uint32, std::set<uint32>> checkedPairs;

	for (auto collider : colliders)
	{
		for (auto otherCollider : colliders)
		{
			//Check to not collision check itself
			if (collider->ownerID == otherCollider->ownerID)
				continue;

			//Try and find an algorithm for checking
			CollisionAlgorithm* algorithm = lookUpAlgorithm[collider->colliderType][otherCollider->colliderType];
			//If it doesn't find one, try by switching the 2 around
			if (!algorithm)
			{
				algorithm = lookUpAlgorithm[otherCollider->colliderType][collider->colliderType];

				if (!algorithm)
					continue;

				Collider* temp = collider;
				collider = otherCollider;
				otherCollider = temp;
			}

			if (checkedPairs.count(collider->id))
				if (checkedPairs[collider->id].count(otherCollider->id))
					continue;

			checkedPairs[collider->id].insert(otherCollider->id);
			//Debug::Log(DebugInfo, "Checking %i %i", collider->id, otherCollider->id);
			//check collision
			Collision collision = algorithm->CollisionDetect(collider, GetComponent<Transform>(collider->ownerID), otherCollider, GetComponent<Transform>(otherCollider->ownerID));
			collision.penetration *= -1;

			//Check if already collided. If so, remove it
			if (!collision)
			{
				if (collider->collidedWith.count(otherCollider->id))
				{
					//OnTriggerEnd
					for (auto callback : collider->OnCollisionEndCallback)
						callback(collider->collisions[otherCollider->id]);

					for (auto callback : otherCollider->OnCollisionEndCallback)
						callback(otherCollider->collisions[collider->id]);

					collider->collisions.erase(otherCollider->id);
					collider->collidedWith.erase(otherCollider->id);
					otherCollider->collisions.erase(collider->id);
					otherCollider->collidedWith.erase(collider->id);
				}
				continue;
			}

			//Debug::Log(DebugInfo, "Collision %i and %i", collider->id, otherCollider->id);

			// collided before
			if (collider->collidedWith.count(otherCollider->id))
			{
				//OnTriggerStay
				for (auto callback : collider->OnCollisionStayCallback)
					callback(collision);

				collider->collisions[otherCollider->id] = collision;

				collision.other = collider;
				collision.self = otherCollider;
				collision.penetration *= -1;

				for (auto callback : otherCollider->OnCollisionStayCallback)
					callback(collision);

				otherCollider->collisions[collider->id] = collision;
				continue;
			}

			//Fill in collision list
			collider->collisions[otherCollider->id] = collision;
			collider->collidedWith.insert(otherCollider->id);

			//OnCollisionEnter
			for (auto callback : collider->OnCollisionCallback)
				callback(collision);

			collision.other = collider;
			collision.self = otherCollider;
			collision.penetration *= -1;
			otherCollider->collisions[collider->id] = collision;
			otherCollider->collidedWith.insert(collider->id);

			for (auto callback : otherCollider->OnCollisionCallback)
				callback(collision);
		}
	}
}

