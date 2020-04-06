#pragma once
#include <vector>
#include "ECS/Component.h"

struct DoorTriggerComponent;

namespace Args
{
	struct ButtonTriggerComponent : public Component<ButtonTriggerComponent>
	{
		std::vector<DoorTriggerComponent*> DoorsToOpen;
		std::vector<DoorTriggerComponent*> DoorsToClose;
		bool Pressed;
		ButtonTriggerComponent(Entity* entity) : Component(entity)
		{
			
		}
		// Inherited via Component
		virtual std::string ObjectType() override
		{
			return std::string();
		}

		virtual bool SetData(const std::string& name, const std::string& value) override
		{
			return false;
		}

		virtual bool GetData(const std::string& name, std::string& value) override
		{
			return false;
		}
	};
}
