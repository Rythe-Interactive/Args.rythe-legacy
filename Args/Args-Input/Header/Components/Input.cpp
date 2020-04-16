#include "Input.h"

std::string Args::Input::ObjectType()
{
	return componentName;
}

bool Args::Input::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::Input::GetData(const std::string& name, std::string& value)
{
	return false;
}
