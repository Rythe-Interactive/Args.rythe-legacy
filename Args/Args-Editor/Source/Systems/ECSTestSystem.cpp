#include "Systems/ECSTestSystem.h"
#include "Systems/TestMonoUpdateSystem.h"

using namespace Args;

void ECSTestSystemA::Init()
{
	BindForUpdate(std::bind(&ECSTestSystemA::Update, this, std::placeholders::_1));
}

void ECSTestSystemA::Update(float deltaTime)
{
	clock.Start();
	Transform* transform = entity->GetComponent<Transform>();
	TestMonoUpdateSystem::queryTime += clock.End().Milliseconds();

	transform->Rotate(up, deltaTime);

	clock.Start();
	entity->DestroyComponent<TestComponentA>();
	entity->AddComponent<TestComponentB>();
	
	TestMonoUpdateSystem::tortureTime += clock.End().Milliseconds();
	TestMonoUpdateSystem::torturedComponents++;
}

void ECSTestSystemB::Init()
{
	BindForUpdate(std::bind(&ECSTestSystemB::Update, this, std::placeholders::_1));
}

void ECSTestSystemB::Update(float deltaTime)
{
	clock.Start();
	Transform* transform = entity->GetComponent<Transform>();
	transform->Rotate(up, deltaTime);
	TestMonoUpdateSystem::queryTime += clock.End().Milliseconds();

	clock.Start();
	entity->AddComponent<TestComponentA>();
	entity->DestroyComponent<TestComponentB>();

	TestMonoUpdateSystem::tortureTime += clock.End().Milliseconds();
	TestMonoUpdateSystem::torturedComponents++;
}
