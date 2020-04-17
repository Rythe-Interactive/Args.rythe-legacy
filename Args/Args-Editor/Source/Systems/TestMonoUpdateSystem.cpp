#include "Systems/TestMonoUpdateSystem.h"

using namespace Args;

double TestMonoUpdateSystem::lowestQuery = DBL_MAX;
double TestMonoUpdateSystem::highestQuery = 0.0;
double TestMonoUpdateSystem::lowestAdd = DBL_MAX;
double TestMonoUpdateSystem::highestAdd = 0.0;
double TestMonoUpdateSystem::lowestRemove = DBL_MAX;
double TestMonoUpdateSystem::highestRemove = 0.0;

double TestMonoUpdateSystem::addTime = 0.0;
double TestMonoUpdateSystem::removeTime = 0.0;
double TestMonoUpdateSystem::queryTime = 0.0;
int TestMonoUpdateSystem::torturedComponents = 0;

void TestMonoUpdateSystem::Init()
{
	BindForUpdate(std::bind(&TestMonoUpdateSystem::Update, this, std::placeholders::_1));
	BindForFixedUpdate(1.f, std::bind(&TestMonoUpdateSystem::Print, this, std::placeholders::_1));
	BindForFixedUpdate(120.f, std::bind(&TestMonoUpdateSystem::Shutdown, this, std::placeholders::_1));
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
	if (lastDeltaTime == 0)
	{
		lastDeltaTime = deltaTime * 1000.f;
	}
	else
	{
		accumDeviation += abs((deltaTime * 1000.f) - lastDeltaTime);
		lastDeltaTime = deltaTime * 1000.f;
	}

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
	float elapsedms = elapsedTime * 1000.f;
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
		(float)lowestQuery, (float)highestQuery, (float)(totalQuery / totalTortured),
		(float)lowestQueryFrame, (float)highestQueryFrame, (float)(totalQuery / totalFrames), (int)round(totalQuery), (float)((totalQuery / elapsedms) * 100.0),

		(float)lowestAdd, (float)highestAdd, (float)(totalAdd / totalTortured),
		(float)lowestAddFrame, (float)highestAddFrame, (float)(totalAdd / totalFrames), (int)round(totalAdd), (float)((totalAdd / elapsedms) * 100.0),

		(float)lowestRemove, (float)highestRemove, (float)(totalRemove / totalTortured),
		(float)lowestRemoveFrame, (float)highestRemoveFrame, (float)(totalRemove / totalFrames), (int)round(totalRemove), (float)((totalRemove / elapsedms) * 100.0),
		(int)round(elapsedms)
	);


	Debug::Success(DebugInfo, "---Run data---\n\tElapsed time: %fs\n\tAverage frame time: %fms\n\tAverage frame rate: %ffps\n\tAverage deviation: %fms", elapsedTime, elapsedms / totalFrames, totalFrames / elapsedTime, accumDeviation / (totalFrames - 1.0));
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
