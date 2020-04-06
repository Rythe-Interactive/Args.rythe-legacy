#pragma once
#include <Args-Core.h>
#include "Components/Renderable.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Args
{
	class Renderer : public MonoUpdateSystem<Renderer, Renderable, Transform>
	{
	private:
		Clock cpuClock;

		float vertices[12] {
			-1.f, -1.f, 0.f,
			1.f, -1.f, 0.f,
			1.f, 1.f, 0.f,
			-1.f, 1.f, 0.f
		};


		float uvs[8] {
			0.f, 0.f,
			1.f, 0.f,
			1.f, 1.f,
			0.f, 1.f
		};

		unsigned short indices[6] {
			3, 2, 1,
			1, 0, 3
		};

	public:
		virtual void Init() override;

		void Render(float deltaTime);

		void RenderLoadScreen(const std::string& texName);

		static void ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}