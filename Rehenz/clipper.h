#pragma once
#include "math.h"

namespace Rehenz
{
	// inside return 0
	int ComputeClipState(Point2 p);
	// inside return 0
	int ComputeClipState(Point3 p);
	// inside return 0
	int ComputeClipState(Point p);

	bool ClipLine2DCohenSutherland(Point2& p1, Point2& p2, float _xmax, float _ymax);
	bool ClipLine2DCohenSutherland(Point2& p1, Point2& p2, float _xmin, float _xmax, float _ymin, float _ymax);

	bool ClipLine3DCohenSutherland(Point3& p1, Point3& p2, float _xmax, float _ymax, float _zmax);
	bool ClipLine3DCohenSutherland(Point3& p1, Point3& p2, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax);

	bool ClipLineCohenSutherland(Point& p1, Point& p2);

	// return the number of small triangles, up to 5
	// this is a wrong implement, and not correctly handle the trianlge which surrounds corner
	int ClipTriangle(const Point& p1, const Point& p2, const Point& p3, Point ps_out[15]);
}