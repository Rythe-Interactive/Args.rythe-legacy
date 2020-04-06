#include <Systems/DoorSystem.h>

void Args::DoorSystem::Init()
{
}

void Args::DoorSystem::Start()
{
}


void Args::DoorSystem::Update(float deltaTime)
{
	Args::DoorTriggerComponent* doorTrigger;
	Args::Transform* transform;
	GetComponents(&doorTrigger, &transform);

	if (doorTrigger->PressureToActive.size() > 0)
	{
		int count = 0;
		for (auto plate : doorTrigger->PressureToActive)
		{
			if (plate->BeingPressed)
			{
				count++;
			}
		}
		if (count >= doorTrigger->PressureToActive.size())
		{
			doorTrigger->activated = true;
		}
		else
		{
			doorTrigger->activated = false;
		}
	}
	else if (doorTrigger->PressureToActive.size() <= 0)
	{
		int count = 0;
		for (Args::PressureTriggerComponent* component : GetComponentsOfType<Args::PressureTriggerComponent>())
		{
			if (component->DoorsToClose.size() > 0)
			{
				if (component->BeingPressed)
				{
					count++;
					if (count >= component->DoorsToClose.size())
					{
						for (Args::DoorTriggerComponent* door : component->DoorsToClose)
						{
							door->activated = false;
						}
					}
				}
			}
			if (component->DoorsToOpen.size() > 0)
			{
				if (component->BeingPressed)
				{
					count++;
					if (count >= component->DoorsToOpen.size())
					{
						for (Args::DoorTriggerComponent* door : component->DoorsToOpen)
						{
							door->activated = true;
						}
					}
				}
			}
		}
		for (Args::ButtonTriggerComponent* component : GetComponentsOfType<Args::ButtonTriggerComponent>())
		{
			if (component->DoorsToClose.size() > 0)
			{
				if (component->Pressed)
				{
					count++;
					if (count >= component->DoorsToClose.size())
					{
						for (Args::DoorTriggerComponent* door : component->DoorsToClose)
						{
							door->activated = false;
						}
					}
				}
			}
			if (component->DoorsToOpen.size() > 0)
			{
				if (component->Pressed)
				{
					count++;
					if (count >= component->DoorsToOpen.size())
					{
						for (Args::DoorTriggerComponent* door : component->DoorsToOpen)
						{
							door->activated = true;
						}
					}
				}
			}
		}
	}

	if (doorTrigger->activated)
	{
		OpenDoor();
	}
	else if(!doorTrigger->activated)
	{
		CloseDoor();
	}
}

void Args::DoorSystem::OpenDoor()
{
	Args::DoorTriggerComponent* doorTrigger;
	Args::Transform* transform;
	GetComponents(&doorTrigger, &transform);
	transform->SetPosition(doorTrigger->OpenPosition);
}

void Args::DoorSystem::CloseDoor()
{
	Args::DoorTriggerComponent* doorTrigger;
	Args::Transform* transform;
	GetComponents(&doorTrigger, &transform);
	transform->SetPosition(doorTrigger->ClosedPosition);
}






