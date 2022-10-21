#include "transform.h"

namespace Rehenz
{
	Transform::Transform() : pos(0, 0, 0), axes(0, 0, 0), scale(1, 1, 1)
	{
	}

	Transform::~Transform()
	{
	}

	Matrix Transform::GetTransformMatrix()
	{
		return GetMatrixS(scale) * GetMatrixA(axes) * GetMatrixT(pos);
	}

	Matrix Transform::GetInverseTransformMatrix()
	{
		return  GetInverseMatrixT(pos) * GetInverseMatrixA(axes) * GetInverseMatrixS(scale);
	}

	Vector Transform::GetFront()
	{
		return Vector(0, 0, 1) * GetMatrixA(axes);
	}

	Vector Transform::GetUp()
	{
		return Vector(0, 1, 0) * GetMatrixA(axes);
	}

	Vector Transform::GetRight()
	{
		return Vector(1, 0, 0) * GetMatrixA(axes);
	}

	Vector Transform::GetFrontInGround()
	{
		return Vector(sinf(axes.yaw), 0, cosf(axes.yaw));
	}

	Vector Transform::GetRightInGround()
	{
		return Vector(cosf(axes.yaw), 0, -sinf(axes.yaw));
	}

	void Transform::SetFront(Vector front)
	{
		float r = std::sqrtf(front.x * front.x + front.z * front.z);
		if (r == 0)
			axes.yaw = 0;
		else
		{
			axes.yaw = std::acos(front.z / r);
			if (front.x < 0)
				axes.yaw = pi_mul2 - axes.yaw;
		}
		if (front.y == 0)
			axes.pitch = 0;
		else if (r == 0)
			axes.pitch = (front.y > 0 ? -pi_div2 : pi_div2);
		else
			axes.pitch = -atanf(front.y / r);
	}

	Projection::Projection() :fovy(pi_div2), aspect(1), z_near(0.1f), z_far(50)
	{
		parallel_projection = false;
		height = 8.0f;
	}

	Projection::~Projection()
	{
	}

	Matrix Projection::GetTransformMatrix()
	{
		if (!parallel_projection)
			return GetMatrixP(fovy, aspect, z_near, z_far);
		else
			return GetMatrixT(0, 0, -z_near) * GetMatrixS(2 / height / aspect, 2 / height, 1 / (z_far - z_near));
	}

	Point Projection::GetOrigin()
	{
		if (!parallel_projection)
			return GetOriginP(z_near, z_far);
		else
			return Point(0, 0, -z_near / (z_far - z_near));
	}

}