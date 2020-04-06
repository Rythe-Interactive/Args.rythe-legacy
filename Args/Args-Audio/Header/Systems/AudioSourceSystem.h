#pragma once
#include <Args-Core.h>
#include "Components/AudioSource.h"

namespace Args
{
	class AudioSourceSystem : public EntitySystem<AudioSourceSystem, AudioSource, Transform>
	{
	public:
		AudioSourceSystem() : EntitySystem<AudioSourceSystem, AudioSource, Transform>() {}

		virtual void Init() override;

		void Update(float deltaTime);
	};
}