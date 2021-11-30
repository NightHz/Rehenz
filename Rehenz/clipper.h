#pragma once
#include "math.h"

namespace Rehenz
{
	namespace ClipState
	{
		const int inside = 0; // 000000
		const int left = 1;   // 000001
		const int right = 2;  // 000010
		const int down = 4;   // 000100
		const int top = 8;    // 001000
		const int front = 16; // 010000
		const int back = 32;  // 100000
	};

	int ComputeClipState(Point2 p);
	bool CohenSutherlandClip2D(Point2& p1, Point2& p2);
}