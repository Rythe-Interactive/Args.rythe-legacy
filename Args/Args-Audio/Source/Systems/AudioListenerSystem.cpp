#include "Systems/AudioListenerSystem.h"
#include "Components/AudioDevice.h"

void Args::AudioListenerSystem::Init()
{
	BindForUpdate(std::bind(&AudioListenerSystem::Update, this, std::placeholders::_1));
}

void Args::AudioListenerSystem::Update(float deltaTime)
{
	auto entities = GetEntityList();
	if (entities.size() > 1)
		Debug::Warning(DebugInfo, "More than one audio listener found! Scenes can only have 1 audio listener");

	for (auto entity : entities)
	{
		AudioListener* listener = GetComponent<AudioListener>(entity);
		listener->UpdateListener(GetComponent<Transform>(entity));
		break;
	}
}
