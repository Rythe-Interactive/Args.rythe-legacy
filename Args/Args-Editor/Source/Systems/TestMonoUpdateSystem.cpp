#include "Systems/TestMonoUpdateSystem.h"

using namespace Args;

float TestMonoUpdateSystem::lowestQuery = FLT_MAX;
float TestMonoUpdateSystem::highestQuery = 0.f;
float TestMonoUpdateSystem::lowestAdd = FLT_MAX;
float TestMonoUpdateSystem::highestAdd = 0.f;
float TestMonoUpdateSystem::lowestRemove = FLT_MAX;
float TestMonoUpdateSystem::highestRemove = 0.f;

float TestMonoUpdateSystem::addTime = 0.f;
float TestMonoUpdateSystem::removeTime = 0.f;
float TestMonoUpdateSystem::queryTime = 0.f;
int TestMonoUpdateSystem::torturedComponents = 0;

void TestMonoUpdateSystem::Init()
{
	BindForUpdate(std::bind(&TestMonoUpdateSystem::Update, this, std::placeholders::_1));
	BindForFixedUpdate(1.f, std::bind(&TestMonoUpdateSystem::Print, this, std::placeholders::_1));
	BindForFixedUpdate(300.f, std::bind(&TestMonoUpdateSystem::Shutdown, this, std::placeholders::_1));
	//BindForFixedUpdate(30.f, std::bind(&TestMonoUpdateSystem::Shutdown, this, std::placeholders::_1));

	GetGlobalComponent<Args::Input>()->BindAction("Exit", std::bind(&TestMonoUpdateSystem::Exit, this, std::placeholders::_1, std::placeholders::_2));

	Debug::Success(DebugInfo, "Initialised TestMonoUpdateSystem");

	GetGlobalComponent<Args::SceneComponent>()->nextScene = "tortureScene1000";
}

void TestMonoUpdateSystem::Start()
{
}

void TestMonoUpdateSystem::Update(float deltaTime)
{
	totalQuery += queryTime;
	totalAdd += addTime;
	totalRemove += removeTime;
	totalTortured += (uint32)torturedComponents;

	if (queryTime > 0)
		if (queryTime < lowestQueryFrame)
			lowestQueryFrame = queryTime;
	if (queryTime > highestQueryFrame)
		highestQueryFrame = queryTime;

	if (addTime > 0)
		if (addTime < lowestAddFrame)
			lowestAddFrame = addTime;
	if (addTime > highestAddFrame)
		highestAddFrame = addTime;

	if (removeTime > 0)
		if (removeTime < lowestRemoveFrame)
			lowestRemoveFrame = removeTime;
	if (removeTime > highestRemoveFrame)
		highestRemoveFrame = removeTime;

	//Debug::Log(DEBUG_BLUE, DebugInfo, "queried %i components with an average time of %fms and a total of %fms", torturedComponents, queryTime / torturedComponents, queryTime);
	//Debug::Log(DEBUG_BLUE, DebugInfo, "tortured %i components with an average time of %fms and a total of %fms\n", torturedComponents, tortureTime / torturedComponents, tortureTime);
	torturedComponents = 0;
	addTime = 0.f;
	removeTime = 0.f;
	queryTime = 0.f;

	totalFrames++;
	updatesSincePrint++;
	accumDeltaTime += deltaTime;
	elapsedTime += deltaTime;
}

void TestMonoUpdateSystem::Print(float deltaTime)
{
	if (updatesSincePrint == 0 && accumDeltaTime > 0)
		Debug::Log(DEBUG_PURPLE, DebugInfo, "%fs since last update.\n", accumDeltaTime);
	else if (updatesSincePrint > 0)
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
	double elapsedms = elapsedTime * 1000.0;
	std::string benchmarkFormat = "Benchmark Data\n";
	benchmarkFormat += "---Querying---\n";
	benchmarkFormat += "\t-Per Operation-\n";
	benchmarkFormat += "\tLowest: %fms\n\tHighest: %fms\n\tAverage: %fms\n\n";
	benchmarkFormat += "\t-Per Frame-\n";
	benchmarkFormat += "\tLowest: %fms\n\tHighest: %fms\n\tAverage: %fms\nTotal: %ims %.1f%%\n\n";

	benchmarkFormat += "---Adding Components---\n";
	benchmarkFormat += "\t-Per Operation-\n";
	benchmarkFormat += "\tLowest: %fms\n\tHighest: %fms\n\tAverage: %fms\n\n";
	benchmarkFormat += "\t-Per Frame-\n";
	benchmarkFormat += "\tLowest: %fms\n\tHighest: %fms\n\tAverage: %fms\nTotal: %ims %.1f%%\n\n";

	benchmarkFormat += "---Removing Components---\n";
	benchmarkFormat += "\t-Per Operation-\n";
	benchmarkFormat += "\tLowest: %fms\n\tHighest: %fms\n\tAverage: %fms\n\n";
	benchmarkFormat += "\t-Per Frame-\n";
	benchmarkFormat += "\tLowest: %fms\n\tHighest: %fms\n\tAverage: %fms\nTotal: %ims %.1f%%\n\n";
	benchmarkFormat += "Total Elapsed: %ims";

	Debug::Log(DebugInfo, benchmarkFormat,
		lowestQuery, highestQuery, totalQuery / totalTortured,
		lowestQueryFrame, highestQueryFrame, totalQuery / totalFrames, (int)round(totalQuery), (totalQuery / elapsedms) * 100.f,

		lowestAdd, highestAdd, totalAdd / totalTortured,
		lowestAddFrame, highestAddFrame, totalAdd / totalFrames, (int)round(totalAdd), (totalAdd / elapsedms) * 100.f,

		lowestRemove, highestRemove, totalRemove / totalTortured,
		lowestRemoveFrame, highestRemoveFrame, totalRemove / totalFrames, (int)round(totalRemove), (totalRemove / elapsedms) * 100.f,
		(int)round(elapsedms)
	);

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
