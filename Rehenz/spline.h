#pragma once
#include "type.h"
#include "math.h"

namespace Rehenz
{
    Point2 CalcBSplineZone(int n, float knots[], Point2 points[], float x);
    Point2 CalcBSpline(int n, int m, float knots[], Point2 points[], float x);
    Point2 CalcBSplineZoneDeBoor(int n, float knots[], Point2 points[], float x);
    Point2 CalcBSplineDeBoor(int n, int m, float knots[], Point2 points[], float x);
}
