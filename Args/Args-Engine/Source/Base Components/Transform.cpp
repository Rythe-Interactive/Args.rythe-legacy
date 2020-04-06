#include "Base Components/Transform.h"
#include "ECS/Managers/ComponentManager.h"
#include "ECS/Entity.h"

Args::Vector3 Args::Transform::GetPosition() const
{
	return position;
}

void Args::Transform::SetPosition(const Vector3& position)
{
	if (this->position.x != this->position.x)
		Debug::Log(DebugInfo, "NAN found");
	this->position = position;
	if (this->position.x != this->position.x)
		Debug::Log(DebugInfo, "NAN found");
}

Args::Vector3 Args::Transform::GetScale() const
{
	return Vector3(length(scaledRotationX), length(scaledRotationY), length(scaledRotationZ));
}

void Args::Transform::SetScale(const Vector3& scale)
{
	matrix[0] = normalize(matrix[0]) * scale.x;
	matrix[1] = normalize(matrix[1]) * scale.y;
	matrix[2] = normalize(matrix[2]) * scale.z;
}

Args::Quaternion Args::Transform::GetRotation() const
{
	return quat_cast(Matrix3(normalize(matrix[0]), normalize(matrix[1]), normalize(matrix[2])));
}

Args::Matrix3 Args::Transform::GetRotationMatrix() const
{
	return Matrix3(normalize(matrix[0]), normalize(matrix[1]), normalize(matrix[2]));
}

void Args::Transform::SetRotation(const Quaternion& rotation)
{
	Vector3 scale = GetScale();
	Matrix3 mat = mat3_cast(rotation);

	matrix[0] = Vector4(normalize(mat[0]) * scale.x, matrix[0].w);
	matrix[1] = Vector4(normalize(mat[1]) * scale.y, matrix[1].w);
	matrix[2] = Vector4(normalize(mat[2]) * scale.z, matrix[2].w);
}

void Args::Transform::SetRotation(const Matrix3& rotation)
{
	Vector3 scale = GetScale();
	matrix[0] = Vector4(normalize(rotation[0]) * scale.x, matrix[0].w);
	/*Debug::Log(DebugInfo,"Scale X: %f",scale.x);
	Debug::Log(DebugInfo, "Rotation X: %f Y:%f Z:%f", normalize(rotation[0]).x,normalize(rotation[0]).y,normalize(rotation[0]).z);*/
	matrix[1] = Vector4(normalize(rotation[1]) * scale.y, matrix[1].w);
	//Debug::Log(DebugInfo, "Scale Y: %f", scale.y);
	//Debug::Log(DebugInfo, "Rotation X: %f Y:%f Z:%f", normalize(rotation[1]).x, normalize(rotation[1]).y, normalize(rotation[1]).z);
	matrix[2] = Vector4(normalize(rotation[2]) * scale.z, matrix[2].w);
	//Debug::Log(DebugInfo, "Scale Z: %f", scale.z);
	//Debug::Log(DebugInfo, "Rotation X: %f Y:%f Z:%f", normalize(rotation[2]).x, normalize(rotation[2]).y, normalize(rotation[2]).z);
}

//Args::Vector3 Args::Transform::RotatePoint(const Vector3& point)
//{
//	return matrix * Vector4(point, 0);
//}
//
//Args::Vector3 Args::Transform::TransformPoint(const Vector3& point)
//{
//	return 	inverse(matrix) * Vector4(point, 1);
//}

Args::Vector3 Args::Transform::GetForward()
{
	return WorldRotatePoint(forward);
}

Args::Vector3 Args::Transform::GetRight()
{
	return WorldRotatePoint(right);
}

Args::Vector3 Args::Transform::GetUp()
{
	return WorldRotatePoint(up);
}

void Args::Transform::SetForward(const Vector3& forward)
{
	Vector3 position = GetPosition();
	Vector3 scale = GetScale();
	matrix = inverse(lookAtLH(zero, normalize(forward), up));

	SetScale(scale);
	SetPosition(position);
}

void Args::Transform::SetRight(const Vector3& right)
{
	Vector3 newRight = normalize(right);
	Vector3 oldRight = GetRight();
	if (newRight != oldRight)
		Rotate(normalize(cross(oldRight, newRight)), angle(oldRight, newRight));
}

void Args::Transform::SetUp(const Vector3& up)
{
	Vector3 newUp = normalize(up);
	Vector3 oldUp = GetUp();
	if (newUp != oldUp)
		Rotate(normalize(cross(oldUp, newUp)), angle(oldUp, newUp));
}

void Args::Transform::Rotate(const Vector3& axis, float angle)
{
	Matrix3 rotation = GetRotationMatrix();

	rotation = rotation * (Matrix3)rotate(angle, axis);

	SetRotation(rotation);
}

void Args::Transform::Move(const Vector3& translation)
{
	position += translation;
}

void Args::Transform::Scale(const Vector3& scale)
{
	SetScale(GetScale() * scale);
}

Args::Matrix4 Args::Transform::GetWorldTransform()
{
	if (parent == 0)
		return matrix;

	return manager->GetComponent<Transform>(parent)->GetWorldTransform() * matrix;
}

Args::Vector3 Args::Transform::GetWorldPosition()
{
	if (parent == 0)
		return position;

	return manager->GetComponent<Transform>(parent)->GetWorldTransform() * Vector4(position, 1);
}

void Args::Transform::SetWorldPosition(const Vector3& position)
{
	if (parent == 0)
	{
		SetPosition(position);
		return;
	}

	this->position = inverse(manager->GetComponent<Transform>(parent)->GetWorldTransform()) * Vector4(position, 1);
}

Args::Vector3 Args::Transform::GetWorldScale()
{
	if (parent == 0)
		return GetScale();

	Matrix3 worldRotationScale = GetWorldTransform();
	return Vector3(length(worldRotationScale[0]), length(worldRotationScale[1]), length(worldRotationScale[2]));
}

void Args::Transform::SetWorldScale(const Vector3& scale)
{
	if (parent == 0)
	{
		SetScale(scale);
		return;
	}

	Matrix3 parentWorldRotationScale = manager->GetComponent<Transform>(parent)->GetWorldTransform();
	Vector3 parentWorldScale = Vector3(length(parentWorldRotationScale[0]), length(parentWorldRotationScale[1]), length(parentWorldRotationScale[2]));
	SetScale(scale / parentWorldScale);
}

Args::Quaternion Args::Transform::GetWorldRotation()
{
	if (parent == 0)
		GetRotation();

	Matrix3 worldRotationScale = GetWorldTransform();
	return quat_cast(Matrix3(normalize(worldRotationScale[0]), normalize(worldRotationScale[1]), normalize(worldRotationScale[2])));
}

void Args::Transform::SetWorldRotation(const Quaternion& rotation)
{
	if (parent == 0)
	{
		SetRotation(rotation);
		return;
	}

	Matrix3 parentWorldRotationScale = manager->GetComponent<Transform>(parent)->GetWorldTransform();
	Quaternion parentWorldRotation = quat_cast(Matrix3(normalize(parentWorldRotationScale[0]), normalize(parentWorldRotationScale[1]), normalize(parentWorldRotationScale[2])));
	SetRotation(inverse(parentWorldRotation) * rotation);
}

Args::Vector3 Args::Transform::WorldRotatePoint(const Vector3& point)
{
	return GetWorldTransform() * Vector4(point, 0);
}

Args::Vector3 Args::Transform::WorldTransformPoint(const Vector3& point)
{
	return GetWorldTransform() * Vector4(point, 1);
}

Args::Vector3 Args::Transform::GetWorldForward()
{
	return WorldRotatePoint(forward);
}

Args::Vector3 Args::Transform::GetWorldRight()
{
	return WorldRotatePoint(right);
}

Args::Vector3 Args::Transform::GetWorldUp()
{
	return WorldRotatePoint(up);
}

Args::uint32 Args::Transform::GetParent()
{
	return parent;
}

std::vector<Args::uint32> Args::Transform::GetChildren()
{
	return children;
}

void Args::Transform::SetParent(Transform* transform)
{
	if (parent > 0)
		owner->manager->GetComponent<Transform>(parent)->RemoveChild(this);

	if (transform)
		transform->AddChild(this);
}

void Args::Transform::AddChild(Transform* transform)
{
	children.push_back(transform->ownerID);
	transform->parent = ownerID;
}

void Args::Transform::RemoveChild(Transform* transform)
{
	children.erase(std::remove(children.begin(), children.end(), transform->ownerID));
}
