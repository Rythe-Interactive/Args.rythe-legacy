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

Args::Vector3 Args::Camera::GetPosition()
{
	return owner->GetComponent<Transform>()->GetPosition();
}

Args::Matrix4 Args::Camera::GetViewProjection()
{
	return projection * inverse(owner->GetComponent<Transform>()->GetWorldTransform());
}

Args::Matrix4 Args::Camera::GetView()
{
	return inverse(owner->GetComponent<Transform>()->GetWorldTransform());
}

std::string Args::Camera::ObjectType()
{
	return GetTypeName<Camera>();
}

bool Args::Camera::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::Camera::GetData(const std::string& name, std::string& value)
{
	return false;
}
