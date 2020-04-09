#include "Systems/ECSTestSystem.h"
#include "Systems/TestMonoUpdateSystem.h"

using namespace Args;

void ECSTestSystem::Init()
{
	BindForUpdate(std::bind(&ECSTestSystem::Update, this, std::placeholders::_1));
}

void ECSTestSystem::Update(float deltaTime)
{
	Transform* transform;
	float time;

	for (int i = 0; i < 10; i++)
	{
		clock.Start();
		transform = entity->GetComponent<Transform>();
		time = clock.End().Milliseconds();

		if (time < TestMonoUpdateSystem::lowestQuery)
			TestMonoUpdateSystem::lowestQuery = time;
		if (time > TestMonoUpdateSystem::highestQuery)
			TestMonoUpdateSystem::highestQuery = time;

		TestMonoUpdateSystem::queryTime += time;
	}

	transform->Rotate(up, deltaTime);

	for (int i = 0; i < 10; i++)
	{
		clock.Start();
		entity->AddComponent<TestComponent>();
		time = clock.End().Milliseconds();

		if (time < TestMonoUpdateSystem::lowestAdd)
			TestMonoUpdateSystem::lowestAdd = time;
		if (time > TestMonoUpdateSystem::highestAdd)
			TestMonoUpdateSystem::highestAdd = time;

		TestMonoUpdateSystem::addTime += time;

		clock.Start();
		entity->DestroyComponent<TestComponent>();
		time = clock.End().Milliseconds();

		if (time < TestMonoUpdateSystem::lowestRemove)
			TestMonoUpdateSystem::lowestRemove = time;
		if (time > TestMonoUpdateSystem::highestRemove)
			TestMonoUpdateSystem::highestRemove = time;

		TestMonoUpdateSystem::removeTime += time;
	}

	TestMonoUpdateSystem::torturedComponents += 10;
}