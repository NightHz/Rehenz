#pragma once
#include "math.h"

namespace Rehenz
{
	bool ClipLine2DCohenSutherland(Point2& p1, Point2& p2, float _xmax, float _ymax);
	bool ClipLine2DCohenSutherland(Point2& p1, Point2& p2, float _xmin, float _xmax, float _ymin, float _ymax);

	bool ClipLine3DCohenSutherland(Point3& p1, Point3& p2, float _xmax, float _ymax, float _zmax);
	bool ClipLine3DCohenSutherland(Point3& p1, Point3& p2, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax);
}