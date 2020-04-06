#include "Systems/AudioSourceSystem.h"

void Args::AudioSourceSystem::Init()
{
	BindForUpdate(std::bind(&AudioSourceSystem::Update, this, std::placeholders::_1));
}

void Args::AudioSourceSystem::Update(float deltaTime)
{
	AudioSource* source;
	Transform* transform;
	GetComponents(&source, &transform);

	if (source->IsFinished())
		source->CleanUp();
	else
		source->SetPosition(transform->GetWorldPosition());
}
