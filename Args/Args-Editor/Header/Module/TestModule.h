#pragma once
#include <Args-Core.h>

class TestModule : public Args::Module
{
public:
	TestModule(Args::ECS& ecs) : Args::Module(ecs) {}

	// Inherited via Module
	virtual void InitComponents(const std::unordered_set<std::string>& arguments) override;
	virtual void InitSystems(const std::unordered_set<std::string>& arguments) override;
};