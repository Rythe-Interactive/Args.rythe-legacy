#include "Hierarchy/Component/SceneComponent.h"

std::string Args::SceneComponent::ObjectType()
{
	return std::string();
}

bool Args::SceneComponent::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::SceneComponent::GetData(const std::string& name, std::string& value)
{
	return false;
}
