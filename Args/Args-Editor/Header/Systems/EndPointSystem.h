#pragma once
#include <Args-Physics.h>
#include "ECS/System.h"
#include <Components/EndPointComponent.h>
#include <Args-Core.h>


namespace Args
{
	class EndPointSystem : public MonoUpdateSystem<EndPointSystem, EndPointComponent, Collider>
	{
	private:
	public:
		EndPointSystem() : MonoUpdateSystem<EndPointSystem, EndPointComponent, Collider>() {}
		virtual void Init() override;
		void Update(float deltaTime);
		void OnTriggerStay(const Collision& collision);
		void OnTriggerExit(const Collision& collision);
		void Start();
	};
}
