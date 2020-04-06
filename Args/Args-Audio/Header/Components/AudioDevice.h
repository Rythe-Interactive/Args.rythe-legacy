#pragma once
#include <Args-Core.h>
#include <IrrKlang/irrKlang.h>

namespace Args
{
	struct AudioDevice : public GlobalComponent<AudioDevice>
	{
		AudioDevice();

		irrklang::ISoundEngine* audioDevice;

		virtual void CleanUp() override;

		irrklang::ISound* Play(const std::string& file, Vector3 position, bool loop = false, bool paused = false);
		void UpdateListener(Vector3 position, Vector3 forward, Vector3 up);

		// Inherited via GlobalComponent
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}