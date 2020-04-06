#include "Components/AudioListener.h"
#include "Components/AudioDevice.h"

void Args::AudioListener::UpdateListener(Transform* transform)
{
	AudioDevice* device = owner->manager->GetGlobalComponent<AudioDevice>();
	device->UpdateListener(transform->GetWorldPosition(), transform->GetWorldForward(), up);
}

std::string Args::AudioListener::ObjectType()
{
	return std::string();
}

bool Args::AudioListener::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::AudioListener::GetData(const std::string& name, std::string& value)
{
	return false;
}
