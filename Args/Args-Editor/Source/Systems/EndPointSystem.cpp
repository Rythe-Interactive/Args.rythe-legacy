#include <Systems/EndPointSystem.h>
#include <Components/PlayerComponent.h>
#include <Hierarchy/System/SceneSystem.h>


void Args::EndPointSystem::Init()
{
	for (auto entity : GetEntityList())
	{
		GetComponent<Collider>(entity)->OnCollisionStayCallback.push_back(std::bind(&EndPointSystem::OnTriggerStay, this, std::placeholders::_1));
		GetComponent<Collider>(entity)->OnCollisionEndCallback.push_back(std::bind(&EndPointSystem::OnTriggerExit, this, std::placeholders::_1));
	}
}

void Args::EndPointSystem::Update(float deltaTime)
{
	int countGood = 0;
	for (auto entity : GetEntityList())
	{
		if (GetComponent<EndPointComponent>(entity)->Activated)
		{
			countGood++;
		}
	}
	if (countGood >= GetEntityList().size())
	{
		//GetGlobalComponent<SceneManager>().nextScene = "";
	}
}

void Args::EndPointSystem::OnTriggerStay(const Collision& collision)
{
	if (collision.other->owner->GetComponent<PlayerComponent>())
	{
		collision.self->owner->GetComponent<EndPointComponent>()->Activated = true;
	}
}

void Args::EndPointSystem::OnTriggerExit(const Collision& collision)
{
	if (collision.other->owner->GetComponent<PlayerComponent>())
	{
		collision.other->owner->GetComponent<EndPointComponent>()->Activated = false;
	}
}