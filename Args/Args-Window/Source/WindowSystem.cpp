#include "WindowSystem.h"
#include "Window.h"
#include "WindowEvents.h"


Args::WindowSystem::~WindowSystem()
{
	glfwTerminate();
	Debug::Log(DebugInfo, "Terminating rendering context");
}

void Args::WindowSystem::Init()
{
	Window* window = componentManager->GetGlobalComponent<Window>();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback(WindowSystem::OnError);

	if (!glfwInit())
		return;

	auto monitor = glfwGetPrimaryMonitor();
	auto videoMode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

	glfwWindowHint(GLFW_SAMPLES, 4);

	window->Create(1920, 1080, "<Args> Test-Project");

	int major = glfwGetWindowAttrib(window->handle, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(window->handle, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(window->handle, GLFW_CONTEXT_REVISION);

	if (!window->handle)
	{
		glfwTerminate();
		return;
	}

	window->MakeCurrent();
	window->SetSwapInterval(0);
	window->SetKeyCallback(WindowSystem::OnInput);
	window->SetWindowCloseCallback(WindowSystem::OnClose);
	window->SetJoystickCallback(WindowSystem::OnControllerConnected);

	for (int controllerID = GLFW_JOYSTICK_1; controllerID <= GLFW_JOYSTICK_LAST; controllerID++)
		if(glfwJoystickPresent(controllerID))
			Engine::RaiseEvent<Events::ControllerConnected>(controllerID, GLFW_CONNECTED);

	BindForUpdate(std::bind(&WindowSystem::Update, this, std::placeholders::_1));
	Engine::BindToEvent<Events::Exit>(std::bind(&WindowSystem::OnExit, this, std::placeholders::_1));

	Debug::Success(DebugInfo, "Initialised window with OpenGL version %i.%i%i", major, minor, revision);
}

void Args::WindowSystem::Update(float deltaTime)
{
	glfwPollEvents();
}

void Args::WindowSystem::OnExit(IEvent* event)
{
	Window* window = componentManager->GetGlobalComponent<Window>();
	glfwDestroyWindow(window->handle);
	window->handle = nullptr;
	Debug::Log(DebugInfo, "Closing");
}

void Args::WindowSystem::OnError(int error, const char* description)
{
	Debug::Error(DebugInfo, "Encountered glfw error %i: %s", error, description);
}

void Args::WindowSystem::OnInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Engine::RaiseEvent<Events::KeyInput>(key, scancode, action, mods);
}

void Args::WindowSystem::OnClose(GLFWwindow* window)
{
	Engine::RaiseEvent<Events::WindowClose>();
	Engine::RaiseEvent<Events::Exit>();
}

void Args::WindowSystem::OnControllerConnected(int controllerID, int event)//Initial Connect
{
	Engine::RaiseEvent<Events::ControllerConnected>(controllerID, event);
}
