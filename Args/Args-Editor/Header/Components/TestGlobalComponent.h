#pragma once
#include<ECS/Component.h>


struct TestGlobalComponent : public Args::GlobalComponent<TestGlobalComponent>
{
	TestGlobalComponent() : Args::GlobalComponent<TestGlobalComponent>() {}
	float value = 0;

	// Inherited via Component
	virtual std::string ObjectType() override
	{
		return Args::GetTypeName<TestGlobalComponent>();
	}

	virtual bool SetData(const std::string& name, const std::string& value) override
	{
		return false;
	}

	virtual bool GetData(const std::string& name, std::string& value) override
	{
		return false;
	}
};
