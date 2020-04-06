#include <Systems/PhysicsSystem.h>
#include <Components/Collider.h>
#include <Collision.h>

void Args::PhysicsSystem::Init()
{
	BindForFixedUpdate((1.f / 80.f), std::bind(&PhysicsSystem::ResolveCollisions, this, std::placeholders::_1));
	Debug::Success(DebugInfo, "Initialised PhysicsSystem");
}



void Args::PhysicsSystem::ResolveCollisions(float deltaTime)
{
	framecount++;

	if (framecount < 160)
		return;

	std::set<uint32> entities = GetEntityList();

	//Collision Resolution
	for (auto entity : entities)
	{
		//Getting colliders per entity
		std::vector<Collider*> colliders = GetComponentsOfType<Collider>(entity);

		//get rigidbody
		Rigidbody* rigidbody = GetComponent<Rigidbody>(entity);
		Transform* transform = GetComponent<Transform>(entity);

		if (transform->position.x != transform->position.x)
			Debug::Log(DebugInfo, "NAN found");

		//calculate reflect
		for (auto collider : colliders)
		{
			if (collider->isTrigger) continue;
			for (auto collision : collider->collisions)
			{
				if (rigidbody->entitiesToIgnore.count(collision.second.other->ownerID))
					continue;

				//Debug::Log(DebugInfo, "Resolving Collision for collider %i", collider->id);
				//Debug::Log(DebugInfo, "Normal %f %f %f", collision.second.normal.x, collision.second.normal.y, collision.second.normal.z);

				rigidbody->impulses.push_back(CalcImpact(collision.second.normal, rigidbody->velocity, rigidbody->restitution));
				//transform->position += CalcImpact(collision.second.normal, transform->position - rigidbody->prevPos, 0.f);
				transform->position += collision.second.normal * collision.second.penetration;
				rigidbody->velocity *= 0.99f;
			}
		}

		for (auto force : rigidbody->forces)
			rigidbody->velocity += force * deltaTime;

		for (auto impulse : rigidbody->impulses)
			rigidbody->velocity += impulse;

		if (rigidbody->velocity.x != rigidbody->velocity.x)
			Debug::Log(DebugInfo, "NAN found");

		//Debug::Log(DebugInfo, "Velocity: %f %f %f", rigidbody->velocity.x, rigidbody->velocity.y, rigidbody->velocity.z);
		rigidbody->impulses.clear();

		rigidbody->prevPos = transform->position;
		transform->position += rigidbody->velocity * deltaTime;

		float speed = length(rigidbody->velocity);
		if (speed != 0 && !isinf(speed))
		{
			Vector3 drag = speed * speed * 0.01f * normalize(rigidbody->velocity);

			rigidbody->velocity += drag * deltaTime;
		}

		//transform->position = clamp(transform->position, Vector3(FLT_MIN * 0.9f), Vector3(FLT_MAX * 0.9f));

		if (rigidbody->velocity.x != rigidbody->velocity.x)
			Debug::Log(DebugInfo, "NAN found");

		if (transform->position.x != transform->position.x)
			Debug::Log(DebugInfo, "NAN found");

		//Debug::Log(DebugInfo, "Position: %f %f %f", transform->position.x, transform->position.y, transform->position.z);

	}
}

Args::Vector3 Args::PhysicsSystem::CalcImpact(Vector3 surfaceNormal, Vector3 incomingVec, float restitution)
{
	Vector3 result = -(1 + restitution) * dot(incomingVec, surfaceNormal) * surfaceNormal;
	if(result.x != result.x)
		Debug::Log(DebugInfo, "NAN found");
	return result;
}



