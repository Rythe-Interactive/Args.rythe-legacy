#pragma once
#include <Args-Core.h>
#include <Args-Input.h>

using namespace std;

class TestMonoUpdateSystem : public Args::MonoUpdateSystem<TestMonoUpdateSystem>
{
private:
	float accumDeltaTime = 0;
	int updatesSincePrint = 0;
	float elapsedTime = 0;

	float totalQuery = 0.f;
	float totalAdd = 0.f;
	float totalRemove = 0.f;

	float lowestQueryFrame = FLT_MAX, highestQueryFrame = 0.f;
	float lowestAddFrame = FLT_MAX, highestAddFrame = 0.f;
	float lowestRemoveFrame = FLT_MAX, highestRemoveFrame = 0.f;

	Args::uint32 totalTortured = 0;
	Args::uint32 totalFrames = 0;
	
public:
	static float lowestQuery;
	static float highestQuery;
	static float lowestAdd;
	static float highestAdd;
	static float lowestRemove;
	static float highestRemove;

	static float addTime;
	static float removeTime;
	static float queryTime;
	static int torturedComponents;

	TestMonoUpdateSystem() : Args::MonoUpdateSystem<TestMonoUpdateSystem>() {}

	virtual void Init() override;

	void Start();

	void Update(float deltaTime);
	void Print(float deltaTime);
	void Shutdown(float deltaTime);

	void Exit(Args::ControllerID controller, Args::ActionState action);
};
