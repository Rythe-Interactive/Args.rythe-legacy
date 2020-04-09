#pragma once
#include <Args-Core.h>

struct TestComponent : public Args::Component<TestComponent>
{
	using Args::Component<TestComponent>::Component;

	virtual std::string ObjectType() override;
	virtual bool SetData(const std::string& name, const std::string& value) override;
	virtual bool GetData(const std::string& name, std::string& value) override;
};