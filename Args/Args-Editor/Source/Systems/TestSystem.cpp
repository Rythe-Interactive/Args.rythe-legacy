#include "Systems/TestSystem.h"
#include "Components/TestGlobalComponent.h"

using namespace Args;

void TestSystem::Init()
{
		BindForUpdate(std::bind(&TestSystem::Update, this, std::placeholders::_1));
		//BindForFixedUpdate(0.5f, std::bind(&TestSystem::Print, this, std::placeholders::_1));

		testInt = 0;
		Debug::Success(DebugInfo, "Initialised TestSystem");
}

void TestSystem::Start()
{
	testInt = 0;
}

void TestSystem::Update(float deltaTime)
{
	TestComponentA* testComponentA;
	TestComponentB* testComponentB;

	GetComponents(&testComponentA, &testComponentB);

	testComponentA->value += 0.1f;

	testComponentB->value += 0.5f;

	TestGlobalComponent* testComponent = GetGlobalComponent<TestGlobalComponent>();

	testComponent->value++;

	for (int i = 0; i < 100; i++)
	{
		testComponentA->value = (testComponent->value / testComponentB->value) * testComponentA->value + perlin(Vector3(testComponentA->value, testComponentB->value, testComponent->value));
	}

	testInt++;
}

void TestSystem::Print(float deltaTime)
{
	TestComponentA* testComponentA;
	TestComponentB* testComponentB;

	GetComponents(&testComponentA, &testComponentB);
	TestGlobalComponent* testComponent = GetGlobalComponent<TestGlobalComponent>();

	Debug::Log(DebugInfo, "Update call %i", testInt);
	Debug::Log(DebugInfo, "testComponentA: %f", testComponentA->value);
	Debug::Log(DebugInfo, "testComponentB: %f", testComponentB->value);
	Debug::Log(DebugInfo, "TestStaticComponent: %f\n", testComponent->value);
}
