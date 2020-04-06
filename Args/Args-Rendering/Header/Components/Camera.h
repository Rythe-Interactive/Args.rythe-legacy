#pragma once
#include <Args-Core.h>

namespace Args
{
	struct Camera : public Component<Camera>
	{
		Camera(Entity* entity) : Component(entity) {}

		Matrix4 projection;

		void SetProjection(float fovy, float ratio, float nearz);

		Vector3 GetPosition();
		Matrix4 GetViewProjection();
		Matrix4 GetView();

		// Inherited via Component
		virtual std::string ObjectType() override;

		virtual bool SetData(const std::string& name, const std::string& value) override;

		virtual bool GetData(const std::string& name, std::string& value) override;

	};
}