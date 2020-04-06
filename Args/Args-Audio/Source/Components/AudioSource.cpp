#include "Components/AudioSource.h"
#include "Components/AudioDevice.h"

void Args::AudioSource::Load(const std::string& file, bool loop)
{
	AudioDevice* device = owner->manager->GetGlobalComponent<AudioDevice>();

	Transform* transform = owner->GetComponent<Transform>();
	if (transform)
		sound = device->Play(AudioDir + file, transform->GetWorldPosition(), loop, true);
	else
		sound = device->Play(AudioDir + file, zero, loop, true);
}

void Args::AudioSource::SetPosition(Vector3 position)
{
	if (sound)
		sound->setPosition(irrklang::vec3df(position.x, position.y, position.z));
}

void Args::AudioSource::Play()
{
	if (sound)
		sound->setIsPaused(false);
}

bool Args::AudioSource::IsFinished()
{
	if (sound)
		return sound->isFinished();
	return true;
}

void Args::AudioSource::CleanUp()
{
	if (sound)
	{
		sound->drop();
		sound = nullptr;
	}
}

std::string Args::AudioSource::ObjectType()
{
	return std::string();
}

bool Args::AudioSource::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::AudioSource::GetData(const std::string& name, std::string& value)
{
	return false;
}
