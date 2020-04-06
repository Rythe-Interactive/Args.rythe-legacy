#pragma once
#include <Args-Core.h>
#include "Components/AudioListener.h"

namespace Args
{
	class AudioListenerSystem : public MonoUpdateSystem<AudioListenerSystem, AudioListener, Transform>
	{
	public:
		AudioListenerSystem() : MonoUpdateSystem<AudioListenerSystem, AudioListener, Transform>() {}

		virtual void Init() override;

		void Update(float deltaTime);
	};
}