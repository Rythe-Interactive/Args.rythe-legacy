#pragma once
#include <Args-Core.h>

struct GLFWwindow;

namespace Args
{
	class WindowSystem : public MonoUpdateSystem<WindowSystem>
	{
	public:
		WindowSystem() : MonoUpdateSystem<WindowSystem>() {}
		virtual ~WindowSystem();

		// Inherited via GlobalSystem
		virtual void Init() override;

		void Update(float deltaTime);
		void OnExit(IEvent* event);

		static void OnError(int error, const char* description);
		static void OnInput(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void OnClose(GLFWwindow* window);
		static void OnControllerConnected(int controllerID, int event);
	};
}