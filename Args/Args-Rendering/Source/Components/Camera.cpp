#include "Components/Camera.h"

void Args::Camera::SetProjection(float fovy, float ratio, float nearz)
{
	float f = 1.0f / tan(fovy / 2.0f);
	projection = Matrix4(
		f / ratio, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, nearz, 0.0f);
}

Args::Vector3 Args::Camera::GetPosition() const
{
	return owner->GetComponent<Transform>()->GetPosition();
}

Args::Matrix4 Args::Camera::GetViewProjection() const
{
	return projection * inverse(owner->GetComponent<Transform>()->GetWorldTransform());
}

Args::Matrix4 Args::Camera::GetView() const
{
	return inverse(owner->GetComponent<Transform>()->GetWorldTransform());
}

std::string Args::Camera::ObjectType()
{
	return componentName;
}

bool Args::Camera::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::Camera::GetData(const std::string& name, std::string& value)
{
	return false;
}
