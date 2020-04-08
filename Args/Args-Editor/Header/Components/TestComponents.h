#pragma once
#include <Args-Core.h>

struct TestComponentA : public Args::Component<TestComponentA>
{
	using Args::Component<TestComponentA>::Component;

	virtual std::string ObjectType() override;
	virtual bool SetData(const std::string& name, const std::string& value) override;
	virtual bool GetData(const std::string& name, std::string& value) override;
};

struct TestComponentB : public Args::Component<TestComponentB>
{
	using Args::Component<TestComponentB>::Component;

	// Inherited via Component
	virtual std::string ObjectType() override;
	virtual bool SetData(const std::string& name, const std::string& value) override;
	virtual bool GetData(const std::string& name, std::string& value) override;
};