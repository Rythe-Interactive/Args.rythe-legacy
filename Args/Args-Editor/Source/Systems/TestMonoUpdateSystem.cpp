#include "Systems/TestMonoUpdateSystem.h"

using namespace Args;

float TestMonoUpdateSystem::tortureTime = 0.f;
float TestMonoUpdateSystem::queryTime = 0.f;
int TestMonoUpdateSystem::torturedComponents = 0;

void TestMonoUpdateSystem::Init()
{
	BindForUpdate(std::bind(&TestMonoUpdateSystem::Update, this, std::placeholders::_1));
	BindForFixedUpdate(1.f, std::bind(&TestMonoUpdateSystem::Print, this, std::placeholders::_1));
	BindForFixedUpdate(3600.f, std::bind(&TestMonoUpdateSystem::Shutdown, this, std::placeholders::_1));

	GetGlobalComponent<Args::Input>()->BindAction("Exit", std::bind(&TestMonoUpdateSystem::Exit, this, std::placeholders::_1, std::placeholders::_2));

	Debug::Success(DebugInfo, "Initialised TestMonoUpdateSystem");

	GetGlobalComponent<Args::SceneComponent>()->nextScene = "tortureScene";
}

void TestMonoUpdateSystem::Start()
{
}

void TestMonoUpdateSystem::Update(float deltaTime)
{
	Debug::Log(DEBUG_BLUE, DebugInfo, "queried %i components with an average time of %fms and a total of %fms", torturedComponents, queryTime / torturedComponents, queryTime);
	Debug::Log(DEBUG_BLUE, DebugInfo, "tortured %i components with an average time of %fms and a total of %fms\n", torturedComponents, tortureTime / torturedComponents, tortureTime);
	torturedComponents = 0;
	tortureTime = 0.f;
	queryTime = 0.f;

	updatesSincePrint++;
	accumDeltaTime += deltaTime;
	elapsedTime += deltaTime;
}

void TestMonoUpdateSystem::Print(float deltaTime)
{
	if (updatesSincePrint == 0 && accumDeltaTime > 0)
		Debug::Log(DEBUG_PURPLE, DebugInfo, "%fs since last update.\n", accumDeltaTime);
	else if(updatesSincePrint > 0)
	{
		Debug::Log(DEBUG_PURPLE, DebugInfo, "%fs since last log.", accumDeltaTime);

		double averageDeltaTime = ((double)accumDeltaTime) / updatesSincePrint;
		Debug::Log(DEBUG_PURPLE, DebugInfo, "average delta time: %fs %fms\n\t\t\t\t\t average fps: %ffps\n", (float)averageDeltaTime, (float)(averageDeltaTime * 1000.f), 1.f / averageDeltaTime);
		updatesSincePrint = 0;
		accumDeltaTime = 0;
	}
}

void TestMonoUpdateSystem::Shutdown(float deltaTime)
{
	Debug::Success(DebugInfo, "Elapsed Time: %f", elapsedTime);
	Engine::RaiseEvent<Events::Exit>();
}

void TestMonoUpdateSystem::Exit(Args::ControllerID controller, Args::ActionState action)
{
	if (action == Args::ActionState::PRESS)
	{
		Debug::Success(DebugInfo, "Elapsed Time: %f", elapsedTime);
		Engine::RaiseEvent<Events::Exit>();
	}
}
