#include <Args.h>
#include <cstdlib>

struct test : public Args::component<test>
{
	using Args::component<test>::component;

	float x, y;
};

class TestSystem : public Args::System
{
public:
	using Args::System::System;
	void init() override {}
};


int main(int argc, char* argv[])
{
	Args::ECS ecs;
	ecs.Register<test>();
	ecs.Register<TestSystem>();

	Args::id_type id = 1;

	Args::entity testEntity(ecs, id);

	test testComp(ecs, testEntity, id);

	TestSystem testSystem(ecs);
	testSystem.init();
	system("pause");
}