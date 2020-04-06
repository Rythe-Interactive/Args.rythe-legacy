#pragma once
#include <unordered_map>
#include <queue>
#include <stack>
#include <memory>
#include <set>
#include <string>
#include <rapidjson/document.h>
#include "ECS/System.h"

using namespace rapidjson;
namespace Args
{
	class SceneSystem : public MonoUpdateSystem<Args::SceneSystem>
	{
	public:
		SceneSystem();

		void Update(float deltaTime);

		void LoadScene(std::string json);
		void UnloadScene();

		// Inherited via MonoUpdateSystem
		virtual void Init() override;
	};
}

