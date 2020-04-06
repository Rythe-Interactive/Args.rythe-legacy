#include "Systems/TestMonoUpdateSystem.h"
#include "Components/TestGlobalComponent.h"

using namespace Args;

void TestMonoUpdateSystem::Init()
{
	BindForUpdate(std::bind(&TestMonoUpdateSystem::Update, this, std::placeholders::_1));
	BindForFixedUpdate(2.f, std::bind(&TestMonoUpdateSystem::Print, this, std::placeholders::_1));
	BindForFixedUpdate(3600.f, std::bind(&TestMonoUpdateSystem::Shutdown, this, std::placeholders::_1));

	GetGlobalComponent<Args::Input>()->BindAction("Exit", std::bind(&TestMonoUpdateSystem::Exit, this, std::placeholders::_1, std::placeholders::_2));

	Debug::Success(DebugInfo, "Initialised TestMonoUpdateSystem");
}

void TestMonoUpdateSystem::Start()
{
	testInt = 0;
}

void TestMonoUpdateSystem::Update(float deltaTime)
{
	TestGlobalComponent* testComponent = GetGlobalComponent<TestGlobalComponent>();

	testComponent->value++;

	testInt++;

	updatesSincePrint++;
	accumDeltaTime += deltaTime;
	elapsedTime += deltaTime;
}

void TestMonoUpdateSystem::Print(float deltaTime)
{
	if (updatesSincePrint == 0)
		Debug::Log(DEBUG_PURPLE, DebugInfo, "0 updates since last log\n");
	else
	{
		float adt = accumDeltaTime / updatesSincePrint;
		Debug::Log(DEBUG_PURPLE, DebugInfo, "average delta time: %fs\n\t\t\t\t\t average fps: %ffps\n", adt, 1.f / adt);
		updatesSincePrint = 0;
		accumDeltaTime = 0;
	}
}

void TestMonoUpdateSystem::Shutdown(float deltaTime)
{
	/*Debug::Success(DebugInfo, "Elapsed Time: %f", elapsedTime);
	Engine::RaiseEvent<Events::Exit>();*/
}

void TestMonoUpdateSystem::Exit(Args::ControllerID controller, Args::ActionState action)
{
	if (action == Args::ActionState::PRESS)
	{
		/*Debug::Success(DebugInfo, "Elapsed Time: %f", elapsedTime);
		Engine::RaiseEvent<Events::Exit>();*/
	}
}
