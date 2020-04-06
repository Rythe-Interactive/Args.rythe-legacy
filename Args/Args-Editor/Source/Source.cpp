#include <Args-Core.h>
#include <Args-Physics.h>
#include <Args-Rendering.h>
#include <Args-Math.h>
#include <Args-Window.h>
#include <Args-Input.h>
#include <Args-Audio.h>

#include <Systems/TestSystem.h>

#include <Module/TestModule.h>
#include <Components/TestComponent.h>

#include <Networking/Client.h>
#include <Networking/Server.h>

#include <Hierarchy/System/SceneSystem.h>
#include <Hierarchy/Module/SceneModule.h>
#include "Module/PhysicsModule.h"
#include "Module/GameplayModule.h"
#include "Components/Movement1Component.h"
#include "Components/Movement2Component.h"
#include "Components/CameraMovementComponent.h"
#include "Components/PickupComponent.h"
#include "Components/PickupAbleComponent.h"


int main(int argc, char* argv[])
{
	Args::Debug::ResetColor(SUCCESS);

	try
	{
		Args::Debug::Error(DebugInfo, "Example error");
	}
	catch (std::logic_error e)
	{
		Args::Debug::Log(DebugInfo, "Caught error: %s", e.what());
	}

	Args::Debug::Success(DebugInfo, "Example success");
	Args::Debug::Warning(DebugInfo, "Example warning");

	Args::Engine engine(argc, argv);

	engine.AttachModule<Args::WindowModule>();
	engine.AttachModule<TestModule>();
	engine.AttachModule<Args::SceneModule>();
	engine.AttachModule<Args::RenderingModule>();
	engine.AttachModule<Args::InputModule>();
	engine.AttachModule<Args::AudioModule>();
	engine.AttachModule<Args::PhysicsModule>();
	engine.AttachModule<Args::GamePlayModule>();
	
	engine.Initialise();

	//for (int i = 0; i < 100; i++)
	//{
	//	Args::uint32 entity = engine.CreateEntity();
	//	engine.AddComponent<TestComponentA>(entity);
	//	engine.AddComponent<TestComponentA>(entity);

	//	Args::Renderable* renderable;
	//	engine.AddComponent<Args::Renderable>(entity, &renderable);
	//	renderable->SetMaterial("testMaterial");
	//	renderable->SetMesh("TestMeshSphere");

	//	Args::Transform* transform;
	//	engine.AddComponent<Args::Transform>(entity, &transform);
	//	transform->SetScale(Args::Vector3(0.1f));
	//	transform->position = Args::Vector3(((std::rand() % 200) / 10.f) - 10.f, ((std::rand() % 200) / 10.f) - 10.f, ((std::rand() % 200) / 10.f) - 10.f);
	//}


	Args::uint32 cameraEntity = engine.CreateEntity();

	Args::Renderable* renderable;
	engine.AddComponent<Args::Renderable>(cameraEntity, &renderable);
	renderable->SetMaterial("PBRMat");
	renderable->SetMesh("TestMeshSphere");

	Args::Camera* camera;
	Args::CameraMovementComponent* camMove;
	engine.AddComponent<Args::Camera>(cameraEntity, &camera);
	camera->SetProjection(Args::radians(60.f), 1920.f / 1080.f, 0.001f);


	engine.AddComponent<Args::AudioListener>(cameraEntity);

	Args::Transform* transform;
	engine.AddComponent<Args::Transform>(cameraEntity, &transform);
	//transform->matrix = Args::inverse(Args::lookAtLH(Args::zero, Args::forward, Args::up));
	transform->SetPosition(Args::Vector3(0, 70, 0));
	transform->Rotate(Args::up, Args::radians(180.0f));
	transform->Rotate(Args::right, Args::radians(75.0f));
	engine.AddComponent<Args::CameraMovementComponent>(cameraEntity, &camMove);
	camMove->Height = 25;

	
	Args::Light* light;
	Args::uint32 directionalLight = engine.CreateEntity();
	engine.AddComponent<Args::Light>(directionalLight, &light);
	light->SetType(Args::LightType::DIRECTIONAL);
	light->SetIntensity(0.5f);
	light->SetColour(Args::Vector3(1.0, 0.9, 0.7));

	engine.AddComponent<Args::Transform>(directionalLight, &transform);
	transform->matrix = Args::inverse(Args::lookAtLH(Args::zero, Args::Vector3(0, 1, -1), Args::up));

	Args::uint32 lightEntity = engine.CreateEntity();
	engine.AddComponent<Args::Light>(lightEntity, &light);
	light->SetType(Args::LightType::POINT);
	light->SetColour(Args::Vector3(1.0));

	Args::AudioSource* audioSource;
	engine.AddComponent<Args::AudioSource>(lightEntity, &audioSource);
	audioSource->Load("Moanin.mp3", true);
	audioSource->Play();

	engine.AddComponent<Args::Renderable>(lightEntity, &renderable);
	renderable->SetMaterial("testMaterial");
	renderable->SetMesh("TestMeshSphere");

	engine.AddComponent<Args::Transform>(lightEntity, &transform);
	transform->position = Args::Vector3(15, 5, 0);
	transform->SetScale(Args::Vector3(0.2f));

	Args::uint32 jazzEntity = engine.CreateEntity();
	engine.AddComponent<Args::AudioSource>(jazzEntity, &audioSource);
	audioSource->Load("Dave Brubeck - Take Five.mp3", true);
	audioSource->Play();

	engine.AddComponent<Args::Renderable>(jazzEntity, &renderable);
	renderable->SetMaterial("testMaterial");
	renderable->SetMesh("TestMeshSphere");

	engine.AddComponent<Args::Transform>(jazzEntity, &transform);
	transform->position = Args::Vector3(-15, 0, 0);
	transform->SetScale(Args::Vector3(0.2f));

	Args::uint32 renderEntity;
	Args::uint32 gigbit = engine.CreateEntity();
	
	Args::Collider* collider;
	Args::Rigidbody* rigidbody;
	//kill me
	engine.AddComponent<Args::Renderable>(gigbit, &renderable);
	renderable->SetMaterial("PBRMat");
	renderable->SetMesh("Gigbit");

	engine.AddComponent<Args::Transform>(gigbit, &transform);
	transform->position.z = 10;
	//transform->SetScale(Args::Vector3(2.5f));
	engine.AddComponent<Args::Movement1Component>(gigbit);
	engine.AddComponent<Args::PickupComponent>(gigbit);
	engine.AddComponent<Args::Collider>(gigbit, &collider);
	collider->colliderType = Args::ColliderType::Box;
	collider->size = Args::Vector3(2,1,2);
	engine.AddComponent<Args::Rigidbody>(gigbit, &rigidbody);
	rigidbody->restitution = 0.f;
		
	camMove->Player1 = gigbit;
	
	//Args::uint32 child = engine.CreateEntity();
	//Args::Transform* childTransform;
	//engine.AddComponent<Args::Transform>(child, &childTransform);
	//childTransform->SetParent(transform);
	//childTransform->position = Args::Vector3(0, -1, 0);
	//engine.AddComponent<Args::Collider>(child, &collider);
	//collider->colliderType = Args::ColliderType::Box;
	//collider->size = Args::Vector3(2, 1, 2);

	////sphere
	//renderEntity = engine.CreateEntity();
	//engine.AddComponent<Args::Renderable>(renderEntity, &renderable);
	//renderable->SetMaterial("PBRMat");
	//renderable->SetMesh("TestMeshSphere");
	//engine.AddComponent<Args::Transform>(renderEntity, &transform);
	//transform->SetScale(Args::Vector3(1.0f));
	//transform->SetPosition(Args::Vector3(0, 10, 0));

	//engine.AddComponent<Args::Collider>(renderEntity, &collider);
	//collider->colliderType = Args::ColliderType::Box;
	//collider->isTrigger = false;
	//collider->size = Args::Vector3(2.0f);
	//engine.AddComponent<Args::Rigidbody>(renderEntity, &rigidbody);

	//camMove->Player1 = renderEntity;
	//rigidbody->velocity = Args::Vector3(0.3, 0, 0);
	//rigidbody->restitution = 0.8f;

	//Throwable block
	renderEntity = engine.CreateEntity();
	engine.AddComponent<Args::Renderable>(renderEntity, &renderable);
	renderable->SetMesh("TestMesh");
	renderable->SetMaterial("PBRMat");
	engine.AddComponent<Args::Transform>(renderEntity, &transform);
	transform->position = Args::Vector3(10, 10, 10);
	//engine.AddComponent<Args::PickupAbleComponent>(renderEntity);
	engine.AddComponent<Args::Collider>(renderEntity, &collider);
	collider->colliderType = Args::ColliderType::Box;
	collider->size = Args::Vector3(2);
	engine.AddComponent<Args::Rigidbody>(renderEntity, &rigidbody);

	
	

	//Plane
	renderEntity = engine.CreateEntity();
	engine.AddComponent<Args::Renderable>(renderEntity, &renderable);
	renderable->SetMesh("Plane");
	renderable->SetMaterial("PBRMat");
	engine.AddComponent<Args::Transform>(renderEntity, &transform);
	transform->SetScale(Args::Vector3(100.0f));
	transform->SetPosition(Args::Vector3(0, -10.0f, 0));
	engine.AddComponent<Args::Collider>(renderEntity, &collider);
	collider->colliderType = Args::ColliderType::Box;
	collider->isTrigger = false;
	collider->size = Args::Vector3(2.f, 2.f, 2.f);
	collider->origin = Args::Vector3(0.f, -1.f, 0.f);

	try
	{
		engine.Run();
	}
	catch (std::logic_error e)
	{
		Args::Engine::RaiseEvent<Args::Events::Exit>();
	}

	system("pause");
}
