#include "Renderer/Renderer.h"
#include "Components/Camera.h"
#include "Components/Light.h"
#include "Components/RenderData.h"
#include "Args-Window.h"
#include <Args-Physics.h>
#include <sstream>

void Args::Renderer::Init()
{
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		Debug::Error(DebugInfo, "Failed to initialize OpenGL context");
		return;
	}

	BindForUpdate(std::bind(&Renderer::Render, this, std::placeholders::_1));

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)Renderer::ErrorCallback, 0);

	glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_GREATER);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_MULTISAMPLE);

	IVector2 viewportSize = GetGlobalComponent<Window>()->GetFramebufferSize();
	glViewport(0, 0, viewportSize.x, viewportSize.y);

	std::stringstream ss;
	ss << "Initialised Renderer\n";
	ss << "\tCONTEXT INFO\n";
	ss << "\t----------------------------------\n";
	ss << "\tGPU Vendor:\t%s\n";
	ss << "\tGPU:\t\t%s\n";
	ss << "\tGL Version:\t%s\n";
	ss << "\tGLSL Version:\t%s\n";
	ss << "\t----------------------------------";

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	Debug::Success(DebugInfo, ss.str(), vendor, renderer, version, glslVersion);

	Texture::CreateTexture("loadscreen", "loadscreen.png");
	RenderLoadScreen("loadscreen");

	cpuClock.Start();
}

void Args::Renderer::Render(float deltaTime)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_GREATER);

	SceneComponent* sceneManager = GetGlobalComponent<SceneComponent>();
	if (sceneManager->nextScene != "null")
	{
		//RenderLoadScreen("loadscreen");
		return;
	}

	//float cpuTime = cpuClock.End().Milliseconds();
	//Debug::Log(DebugInfo, "CPU time: %fms", cpuTime);

	//Clock renderClock;
	//renderClock.Start();

	//glClearColor(0.3f, 0.5f, 1.0f, 1.0f);

	std::vector<LightData> lights;
	for (auto light : GetComponentsOfType<Light>())
		lights.push_back(light->GetLightData());

	Camera* camera = GetComponentsOfType<Camera>()[0];

	RenderData* renderData = GetGlobalComponent<RenderData>();

	for (auto& batch : renderData->batches)
	{
		Mesh* mesh = Mesh::GetMesh(batch.first);
		for (auto& materialGroup : batch.second)
		{
			Material* material = Material::GetMaterial(materialGroup.first);

			material->Bind(mesh, lights);

			std::vector<Matrix4> modelMatrices = std::vector<Matrix4>();

			for (Entity* instance : materialGroup.second)
				modelMatrices.push_back(instance->GetComponent<Transform>()->GetWorldTransform());

			material->Render(modelMatrices, mesh, camera);

			material->Release(mesh);
		}
	}
	
	for (auto collider : GetComponentsOfType<Collider>())
		if (collider->debugRender)
		{
			Transform* transform = collider->owner->GetComponent<Transform>();

			glDepthFunc(GL_ALWAYS);
			glUseProgram(0);
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(value_ptr(camera->projection));
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(value_ptr(camera->GetView() * transform->GetWorldTransform()));

			Vector3 center = collider->origin;
			Vector3 extends;
			if (collider->colliderType == ColliderType::Sphere)
				extends = Vector3(collider->size.x) * 0.5;
			else
				extends = collider->size * 0.5;

			glBegin(GL_LINES);

			glColor3fv(value_ptr(Vector3(1, 0, 0)));
			glVertex3fv(value_ptr(center - Vector3(extends.x, 0, 0)));
			glVertex3fv(value_ptr(center + Vector3(extends.x, 0, 0)));

			glColor3fv(value_ptr(Vector3(0, 1, 0)));
			glVertex3fv(value_ptr(center - Vector3(0, extends.y, 0)));
			glVertex3fv(value_ptr(center + Vector3(0, extends.y, 0)));

			glColor3fv(value_ptr(Vector3(0, 0, 1)));
			glVertex3fv(value_ptr(center - Vector3(0, 0, extends.z)));
			glVertex3fv(value_ptr(center + Vector3(0, 0, extends.z)));

			glEnd();
			glDepthFunc(GL_GREATER);
		}

	GetGlobalComponent<Window>()->Display();

	//float renderTime = renderClock.End().Milliseconds();
	//Debug::Log(DebugInfo, "Render time: %fms", renderTime);
	//Debug::Log(DebugInfo, "Combined time: %fms", cpuTime + renderTime);
	//cpuClock.Start();
}

void Args::Renderer::RenderLoadScreen(const std::string& texName)
{
	Debug::Log(DebugInfo, "Displaying loadscreen");
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearDepth(0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_CULL_FACE);

	glDepthFunc(GL_ALWAYS);

	Shader* shader = Shader::CreateShader("2D Unlit", "Basic2D.vert", "Basic2D.frag");
	shader->Use();

	shader->GetSampler("tex")->SetTexture(Texture::GetTexture(texName));

	Attribute* vertexAttrib = shader->GetAttribute("vertex");
	Attribute* uvAttrib = shader->GetAttribute("uv");

	vertexAttrib->SetAttributePointer(3, GL_FLOAT, GL_FALSE, 0, vertices);
	uvAttrib->SetAttributePointer(2, GL_FLOAT, GL_FALSE, 0, uvs);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	uvAttrib->DisableAttributePointer();
	vertexAttrib->DisableAttributePointer();

	glUseProgram(0);

	GetGlobalComponent<Window>()->Display();
}

void Args::Renderer::ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		try
		{
			Debug::Error(DebugInfo, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s", (type == GL_DEBUG_TYPE_ERROR ? " GL ERROR " : ""), type, severity, message);
		}
		catch (std::logic_error e)
		{

		}
	}
}
