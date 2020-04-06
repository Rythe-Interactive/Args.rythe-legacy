#pragma once
#include <vector>
#include "ECS/Component.h"

struct DoorTriggerComponent;

namespace Args
{
	struct PressureTriggerComponent : public Component<PressureTriggerComponent>
	{
		bool BeingPressed;
		std::vector<DoorTriggerComponent*> DoorsToOpen;
		std::vector<DoorTriggerComponent*> DoorsToClose;

		PressureTriggerComponent(Entity* entity) : Component(entity)
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
