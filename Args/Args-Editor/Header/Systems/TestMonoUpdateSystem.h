#pragma once
#include <Args-Core.h>
#include <Args-Input.h>

using namespace std;

class TestMonoUpdateSystem : public Args::MonoUpdateSystem<TestMonoUpdateSystem>
{
private:
	int testInt = 0;
	float accumDeltaTime = 0;
	int updatesSincePrint = 0;
	float elapsedTime = 0;

public:
	TestMonoUpdateSystem() : Args::MonoUpdateSystem<TestMonoUpdateSystem>() {}

	virtual void Init() override;

	void Start();

	void Update(float deltaTime);
	void Print(float deltaTime);
	void Shutdown(float deltaTime);

	void Exit(Args::ControllerID controller, Args::ActionState action);
};
