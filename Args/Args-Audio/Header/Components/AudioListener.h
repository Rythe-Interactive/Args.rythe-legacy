#pragma once
#include <Args-Core.h>

namespace Args
{
	struct AudioListener : public Component<AudioListener>
	{
		AudioListener(Entity* entity) : Component<AudioListener>(entity) {}

		void UpdateListener(Transform* transform);

		// Inherited via Component
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}