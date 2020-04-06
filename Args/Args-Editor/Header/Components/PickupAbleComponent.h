#pragma once
#include "ECS/Component.h"

namespace Args
{
	struct PickupAbleComponent : public Component<PickupAbleComponent>
	{
		PickupAbleComponent(Entity* entity) : Component(entity)
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
