#pragma once
#include <Args-Core.h>


namespace Args
{
	class AudioModule : public Module
	{
	public:
		AudioModule(ECS& ecs) : Module(ecs) {}

		// Inherited via Module
		virtual void InitComponents(const std::unordered_set<std::string>& arguments) override;
		virtual void InitSystems(const std::unordered_set<std::string>& arguments) override;
	};
}