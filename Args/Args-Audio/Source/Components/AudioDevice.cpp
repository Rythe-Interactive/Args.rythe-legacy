#include "Components/AudioDevice.h"

Args::AudioDevice::AudioDevice() : Args::GlobalComponent<AudioDevice>(), audioDevice()
{
	audioDevice = irrklang::createIrrKlangDevice();
	audioDevice->setDefault3DSoundMinDistance(3.f);
	audioDevice->setSoundVolume(100.f);
}

void Args::AudioDevice::CleanUp()
{
	audioDevice->drop();
}

irrklang::ISound* Args::AudioDevice::Play(const std::string& file, Vector3 position, bool loop, bool paused)
{
	return audioDevice->play3D(file.c_str(), irrklang::vec3df(position.x, position.y, position.z), loop, paused);
}

void Args::AudioDevice::UpdateListener(Vector3 position, Vector3 forward, Vector3 up)
{
	audioDevice->setListenerPosition(irrklang::vec3df(position.x, position.y, position.z), irrklang::vec3df(forward.x, forward.y, forward.z), irrklang::vec3df(0, 0, 0), irrklang::vec3df(up.x, up.y, up.z));
}

std::string Args::AudioDevice::ObjectType()
{
	return std::string();
}

bool Args::AudioDevice::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::AudioDevice::GetData(const std::string& name, std::string& value)
{
	return false;
}
