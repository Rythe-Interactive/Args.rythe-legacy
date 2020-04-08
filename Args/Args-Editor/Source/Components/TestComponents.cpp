#include "Components/TestComponents.h"

std::string TestComponentA::ObjectType()
{
	return "TestComponentA";
}

bool TestComponentA::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool TestComponentA::GetData(const std::string& name, std::string& value)
{
	return false;
}

std::string TestComponentB::ObjectType()
{
	return "TestComponentB";
}

bool TestComponentB::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool TestComponentB::GetData(const std::string& name, std::string& value)
{
	return false;
}
