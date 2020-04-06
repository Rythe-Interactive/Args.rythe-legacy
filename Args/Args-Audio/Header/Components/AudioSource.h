#pragma once
#include <Args-Core.h>
#include <IrrKlang/irrKlang.h>

namespace Args
{
	struct AudioSource : public Component<AudioSource>
	{
		AudioSource(Entity* entity) : Component<AudioSource>(entity), sound() {}

		irrklang::ISound* sound;

		void Load(const std::string& file, bool loop = false);

		void SetPosition(Vector3 position);

		void Play();

		bool IsFinished();
		virtual void CleanUp() override;

		// Inherited via Component
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}