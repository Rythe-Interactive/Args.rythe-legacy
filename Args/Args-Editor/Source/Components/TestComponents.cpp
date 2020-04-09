#include "Components/TestComponents.h"

std::string TestComponent::ObjectType()
{
	return "TestComponentA";
}

bool TestComponent::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool TestComponent::GetData(const std::string& name, std::string& value)
{
	return false;
}