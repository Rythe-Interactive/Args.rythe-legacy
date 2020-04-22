#include <Args.h>
#include <cstdlib>

struct test : public Args::component<test>
{
	using Args::component<test>::component;

	float x, y;
};

class TestSystem : public Args::System<TestSystem>
{
public:
	using Args::System<TestSystem>::System;
	void init() override {}
};



int main(int argc, char* argv[])
{ 
	Args::Engine engine;
	engine.Register<test>();
	engine.Register<TestSystem>();

	Args::id_type id = 1;

	Args::entity testEntity(&engine, id);

	test testComp(&engine, testEntity);

	TestSystem testSystem(&engine);
	testSystem.init();
	system("pause");
}