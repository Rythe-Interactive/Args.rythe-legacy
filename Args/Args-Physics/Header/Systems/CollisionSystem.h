#pragma once
#include <Args-Core.h>
#include "Helpers/CollisionAlgorithm.h"


namespace Args
{
	class CollisionSystem : public Args::MonoUpdateSystem<CollisionSystem, Collider, Transform>
	{
		public:
			std::unordered_map<ColliderType, std::unordered_map<ColliderType, CollisionAlgorithm*>> lookUpAlgorithm;
			CollisionSystem();
			~CollisionSystem();
			virtual void Init() override;
		private:
			void UpdateColliders(float deltaTime);
			
	};
}
