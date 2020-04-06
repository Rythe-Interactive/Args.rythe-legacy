#include "Module/AudioModule.h"
#include "Components/AudioDevice.h"
#include "Components/AudioListener.h"
#include "Systems/AudioListenerSystem.h"
#include "Components/AudioSource.h"
#include "Systems/AudioSourceSystem.h"


void Args::AudioModule::InitComponents(const std::set<std::string>& arguments)
{
	RegisterGlobalComponentType<AudioDevice>();
	RegisterComponentType<AudioListener>();
	RegisterComponentType<AudioSource>();
}

void Args::AudioModule::InitSystems(const std::set<std::string>& arguments)
{
	RegisterSystem<AudioListenerSystem>();
	RegisterSystem<AudioSourceSystem>();
}
