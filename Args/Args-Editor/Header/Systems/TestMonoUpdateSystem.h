#pragma once
#include <Args-Core.h>
#include <Args-Input.h>

using namespace std;

class TestMonoUpdateSystem : public Args::MonoUpdateSystem<TestMonoUpdateSystem>
{
private:
	double accumDeltaTime = 0;
	int updatesSincePrint = 0;
	double elapsedTime = 0;

	double totalQuery = 0.0;
	double totalAdd = 0.0;
	double totalRemove = 0.0;

	double lowestQueryFrame = DBL_MAX, highestQueryFrame = 0.0;
	double lowestAddFrame = DBL_MAX, highestAddFrame = 0.0;
	double lowestRemoveFrame = DBL_MAX, highestRemoveFrame = 0.0;

	Args::uint32 totalTortured = 0;
	Args::uint32 totalFrames = 0;
	
public:
	static double lowestQuery;
	static double highestQuery;
	static double lowestAdd;
	static double highestAdd;
	static double lowestRemove;
	static double highestRemove;

	static double addTime;
	static double removeTime;
	static double queryTime;
	static int torturedComponents;

	TestMonoUpdateSystem() : Args::MonoUpdateSystem<TestMonoUpdateSystem>() {}

	virtual void Init() override;

	void Start();

	void Update(double deltaTime);
	void Print(double deltaTime);
	void Shutdown(double deltaTime);

	void Exit(Args::ControllerID controller, Args::ActionState action);
};
