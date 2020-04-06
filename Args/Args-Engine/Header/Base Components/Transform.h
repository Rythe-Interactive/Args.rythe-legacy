#pragma once
#include<ECS/Component.h>
#include <array>
#include <Args-Math.h>
#include <Utils/Common.h>
#include <vector>


namespace Args
{
	struct Transform : public Component<Transform>
	{
	private:
		uint32 parent;
		std::vector<uint32> children;

	public:
		Transform(Entity* entity) : Component<Transform>(entity), parent(0), children(), matrix()
		{
			scaledRotationX = Vector3(1, 0, 0);
			scaledRotationY = Vector3(0, 1, 0);
			scaledRotationZ = Vector3(0, 0, 1);
			position = Vector3(0, 0, 0);
			one = 1;
		}

		union
		{
			struct
			{
				Matrix4 matrix;
			};

			struct
			{
				Vector3 scaledRotationX;
				float wX;
				Vector3 scaledRotationY;
				float wY;
				Vector3 scaledRotationZ;
				float wZ;
				Vector3 position;
				float one;
			};
		};

		Vector3 GetPosition() const;
		void SetPosition(const Vector3& position);

		Vector3 GetScale() const;
		void SetScale(const Vector3& scale);

		Quaternion GetRotation() const;
		Matrix3 GetRotationMatrix() const;
		void SetRotation(const Quaternion& rotation);
		void SetRotation(const Matrix3& rotation);

		Vector3 GetForward();
		Vector3 GetRight();
		Vector3 GetUp();

		void SetForward(const Vector3& forward);
		void SetRight(const Vector3& right);
		void SetUp(const Vector3& up);

		/// Rotate locally
		void Rotate(const Vector3& axis, float angle);
		/// Move locally
		void Move(const Vector3& translation);
		// Scale locally
		void Scale(const Vector3& scale);

		Matrix4 GetWorldTransform();

		Vector3 GetWorldPosition();
		void SetWorldPosition(const Vector3& position);

		Vector3 GetWorldScale();
		void SetWorldScale(const Vector3& scale);

		Quaternion GetWorldRotation();
		void SetWorldRotation(const Quaternion& rotation);

		Vector3 WorldRotatePoint(const Vector3& point);
		Vector3 WorldTransformPoint(const Vector3& point);

		Vector3 GetWorldForward();
		Vector3 GetWorldRight();
		Vector3 GetWorldUp();
		
		uint32 GetParent();
		std::vector<uint32> GetChildren();
		void SetParent(Transform* transform);
		void AddChild(Transform* transform);
		void RemoveChild(Transform* transform);

		// Inherited via Component
		std::string ObjectType() override
		{
			return GetTypeName<Transform>();
		}

		bool SetData(const std::string& name, const std::string& value) override
		{
			/*if (name.find("Matrix") != std::string::npos)
			{
				size_t divider = 0;
				for (int i = 0; i < 4; i++)
				{
					Matrix[i].x = std::stof(value.substr(divider), &divider);
					Matrix[i].y = std::stof(value.substr(divider), &divider);
					Matrix[i].z = std::stof(value.substr(divider), &divider);
					Matrix[i].w = std::stof(value.substr(divider), &divider);
				}
				return true;
			}
			if (name.find("rotationX") != std::string::npos)
			{
				size_t divider;
				scaledRotationX.x = std::stof(value, &divider);
				scaledRotationX.y = std::stof(value.substr(divider), &divider);
				scaledRotationX.z = std::stof(value.substr(divider), &divider);
				return true;
			}
			if (name.find("rotationY") != std::string::npos)
			{
				size_t divider;
				scaledRotationY.x = std::stof(value, &divider);
				scaledRotationY.y = std::stof(value.substr(divider), &divider);
				scaledRotationY.z = std::stof(value.substr(divider));
				return true;
			}
			if (name.find("rotationZ") != std::string::npos)
			{
				size_t divider;
				scaledRotationZ.x = std::stof(value, &divider);
				scaledRotationZ.y = std::stof(value.substr(divider), &divider);
				scaledRotationZ.z = std::stof(value.substr(divider));
				return true;
			}
			if (name.find("wX") != std::string::npos)
			{
				wX = std::stof(value);
				return true;
			}
			if (name.find("wY") != std::string::npos)
			{
				wY = std::stof(value);
				return true;
			}
			if (name.find("wZ") != std::string::npos)
			{
				wZ = std::stof(value);
				return true;
			}
			if (name.find("one") != std::string::npos)
			{
				one = std::stof(value);
				return true;
			}
			if (name.find("translation") != std::string::npos)
			{
				size_t divider;
				position.x = std::stof(value, &divider);
				position.y = std::stof(value.substr(divider));
				position.z = std::stof(value.substr(divider));
				return true;
			}*/
			return false;
		}

		bool GetData(const std::string& name, std::string& value) override
		{
			//TODO
			/*if (name.find("name") != std::string::npos)
			{
				value = std::to_string();
				return true;
			}*/
			return false;
		}
	};
}