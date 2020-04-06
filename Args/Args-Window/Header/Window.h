#pragma once
#include <Args-Core.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace Args
{
	struct Window : public GlobalComponent<Window>
	{
		GLFWwindow* handle;

		void Create(int width, int height, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr)
		{
			handle = glfwCreateWindow(width, height, title, monitor, share);
		}

		operator GLFWwindow* () const { return handle; }

		IVector2 GetFramebufferSize()
		{
			int width, height;
			glfwGetFramebufferSize(handle, &width, &height);
			return IVector2(width, height);
		}

		float GetAspectRatio()
		{
			IVector2 size = GetFramebufferSize();
			return size.x / (float)size.y;
		}

		void Display()
		{
			if (handle)
				glfwSwapBuffers(handle);
			else
				Debug::Warning(DebugInfo, "Can't display non existing window");
		}

		void SetSwapInterval(int interval)
		{
			glfwSwapInterval(interval);
		}

		void MakeCurrent()
		{
			glfwMakeContextCurrent(handle);
		}

		void SetKeyCallback(GLFWkeyfun callback)
		{
			glfwSetKeyCallback(handle, callback);
		}

		void SetWindowCloseCallback(GLFWwindowclosefun callback)
		{
			glfwSetWindowCloseCallback(handle, callback);
		}

		void SetJoystickCallback(GLFWjoystickfun callback)
		{
			glfwSetJoystickCallback(callback);
		}

		// Inherited via GlobalComponent
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	};
}