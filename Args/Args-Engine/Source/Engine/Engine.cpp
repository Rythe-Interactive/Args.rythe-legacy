#include "Engine/Engine.h"
#include "Engine/CoreModule.h"
#include "Events/DefaultEvents.h"
#include "Utils/Common.h"

#ifdef ARGS_HIGH_PERFORMANCE
__declspec(dllexport) DWORD NvOptimusEnablement = 0x0000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif

std::set<Args::uint32> Args::Engine::events;
std::unordered_map<std::type_index, std::vector<std::function<void(Args::IEvent*)>>> Args::Engine::eventCallbacks;

Args::Engine::Engine(int argc, char* argv[])
{
	ecs = new ECS();

	for (int i = 1; i < argc; i++)
		commandlineArguments.insert(argv[i]);

	AttachModule<CoreModule>();
}

Args::Engine::Engine()
{
	ecs = new ECS();

	AttachModule<CoreModule>();
}

void Args::Engine::Initialise()
{
	Debug::Log(DEBUG_LIGHTBLUE, DebugInfo, "Initialising Engine...");

#ifdef ARGS_HIGH_PERFORMANCE
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
#endif

	for (auto& module : modules)
		module->InitComponents(commandlineArguments);

	std::fprintf(stdout, "\n");

	for (auto& module : modules)
		module->InitSystems(commandlineArguments);

	ecs->InitialiseSystems();

	if(commandlineArguments.empty())
		Debug::Success(DebugInfo, "Initialised Engine");
	else
	{
		std::string arguments = "";
		for (auto arg : commandlineArguments)
			arguments += std::string("\n\t") + arg;

		Debug::Success(DebugInfo, "Initialised Engine with arguments: %s", arguments.c_str());
	}
}

void Args::Engine::Run()
{
	Debug::Log(DebugInfo, "Started running engine with %i initial entities", (int)ecs->GetEntityCount());
	while (!CheckEvent<Events::Exit>())
	{
		ecs->UpdateSystems();
	}

	delete ecs;
	ecs = nullptr;
}

Args::Engine::~Engine()
{
	Debug::CloseOutputFile();
}

