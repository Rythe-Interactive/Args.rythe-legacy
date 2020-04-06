#pragma once
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <GLM/glm.hpp>
#include <glm/ext.hpp>

namespace Args
{
	inline namespace Math { using namespace ::glm; }

	typedef vec4 Vector4;
	typedef vec4 Vec4;
	typedef vec3 Vector3;
	typedef vec3 Vec3;
	typedef vec2 Vector2;
	typedef vec2 Vec2;

	typedef mat4 Matrix4;
	typedef mat4 Mat4;
	typedef mat3 Matrix3;
	typedef mat3 Mat3;
	typedef mat2 Matrix2;
	typedef mat2 Mat2;

	typedef ivec4 IVector4;
	typedef ivec4 IVec4;
	typedef ivec3 IVector3;
	typedef ivec3 IVec3;
	typedef ivec2 IVector2;
	typedef ivec2 IVec2;

	typedef bvec4 BVector4;
	typedef bvec4 BVec4;
	typedef bvec3 BVector3;
	typedef bvec3 BVec3;
	typedef bvec2 BVector2;
	typedef bvec2 BVec2;

	typedef quat Quaternion;
	typedef quat Quat;

	const Vector3 zero		= Vector3(0.f, 0.f, 0.f);
	const Vector3 right		= Vector3(1.f, 0.f, 0.f);
	const Vector3 up		= Vector3(0.f, 1.f, 0.f);
	const Vector3 forward	= Vector3(0.f, 0.f, 1.f);
}


