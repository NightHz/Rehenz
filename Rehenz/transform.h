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

		// default fovy = pi/2, aspect = 1, z_near = 1, z_far = 500
		Projection();
		~Projection();

		Matrix GetTransformMatrix();

		Point GetOrigin();
	};

}