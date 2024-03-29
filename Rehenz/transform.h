#pragma once
#include "type.h"
#include "math.h"

namespace Rehenz
{
	class Transform
	{
	public:
		Vector pos;
		AircraftAxes axes;
		Vector scale;

		Transform();
		~Transform();

		Matrix GetTransformMatrix();
		Matrix GetInverseTransformMatrix();

		Vector GetFront();
		Vector GetUp();
		Vector GetRight();
		Vector GetFrontInGround();
		Vector GetRightInGround();

		void SetFront(Vector front);
	};

	class Projection
	{
	public:
		float fovy, aspect, z_near, z_far;
		bool parallel_projection;
		float height;

		// default fovy = pi/2, aspect = 1, z_near = 0.1, z_far = 50
		Projection();
		~Projection();

		Matrix GetTransformMatrix();
		Matrix GetInverseTransformMatrix();

		Point GetOrigin();
	};

}