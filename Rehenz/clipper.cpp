#include "clipper.h"

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

	float xmin = 0, xmax = 1;
	float ymin = 0, ymax = 1;
	float zmin = 0, zmax = 1;

	int ComputeClipState(Point2 p)
	{
		int state = ClipState::inside;
		if (p.x < xmin)      state |= ClipState::left;
		else if (p.x > xmax) state |= ClipState::right;
		if (p.y < ymin)      state |= ClipState::down;
		else if (p.y > ymax) state |= ClipState::top;
		return state;
	}

	int ComputeClipState(Point3 p)
	{
		int state = ClipState::inside;
		if (p.x < xmin)      state |= ClipState::left;
		else if (p.x > xmax) state |= ClipState::right;
		if (p.y < ymin)      state |= ClipState::down;
		else if (p.y > ymax) state |= ClipState::top;
		if (p.z < zmin)      state |= ClipState::back;
		else if (p.z > zmax) state |= ClipState::front;
		return state;
	}

	bool ClipLine2DCohenSutherland(Point2& p1, Point2& p2, float _xmax, float _ymax)
	{
		return ClipLine2DCohenSutherland(p1, p2, 0, _xmax, 0, _ymax);
	}

	bool ClipLine2DCohenSutherland(Point2& p1, Point2& p2, float _xmin, float _xmax, float _ymin, float _ymax)
	{
		// paste from https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

		xmin = _xmin, xmax = _xmax;
		ymin = _ymin, ymax = _ymax;
		int p1_state = ComputeClipState(p1);
		int p2_state = ComputeClipState(p2);

		while (true)
		{
			if (p1_state == ClipState::inside && p2_state == ClipState::inside)
			{
				// bitwise OR is 0: both points inside window; trivially accept and exit loop
				return true;
			}
			else if (p1_state & p2_state)
			{
				// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
				// or BOTTOM), so both must be outside window; exit loop (accept is false)
				return false;
			}
			else
			{
				// failed both tests, so calculate the line segment to clip
				// from an outside point to an intersection with clip edge
				float x = 0, y = 0;

				// At least one endpoint is outside the clip rectangle; pick it.
				int outcodeOut = p2_state > p1_state ? p2_state : p1_state;

				// Now find the intersection point;
				// use formulas:
				//   slope = (y1 - y0) / (x1 - x0)
				//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
				//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
				// No need to worry about divide-by-zero because, in each case, the
				// outcode bit being tested guarantees the denominator is non-zero
				if (outcodeOut & ClipState::top)          // point is above the clip window
				{
					x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
					y = ymax;
				}
				else if (outcodeOut & ClipState::down)    // point is below the clip window
				{
					x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
					y = ymin;
				}
				else if (outcodeOut & ClipState::right)   // point is to the right of clip window
				{
					y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
					x = xmax;
				}
				else if (outcodeOut & ClipState::left)    // point is to the left of clip window
				{
					y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
					x = xmin;
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == p1_state)
				{
					p1 = Point2(x, y);
					p1_state = ComputeClipState(p1);
				}
				else
				{
					p2 = Point2(x, y);
					p2_state = ComputeClipState(p2);
				}
			}
		}
	}

	bool ClipLine3DCohenSutherland(Point3& p1, Point3& p2, float _xmax, float _ymax, float _zmax)
	{
		return ClipLine3DCohenSutherland(p1, p2, 0, _xmax, 0, _ymax, 0, _zmax);
	}

	bool ClipLine3DCohenSutherland(Point3& p1, Point3& p2, float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax)
	{
		xmin = _xmin, xmax = _xmax;
		ymin = _ymin, ymax = _ymax;
		zmin = _zmin, zmax = _zmax;
		int p1_state = ComputeClipState(p1);
		int p2_state = ComputeClipState(p2);

		while (true)
		{
			if (p1_state == ClipState::inside && p2_state == ClipState::inside)
			{
				return true;
			}
			else if (p1_state & p2_state)
			{
				return false;
			}
			else
			{
				float x = 0, y = 0, z = 0;
				int state = p2_state > p1_state ? p2_state : p1_state;
				if (state & ClipState::front)
				{
					x = p1.x + (p2.x - p1.x) * (zmax - p1.z) / (p2.z - p1.z);
					y = p1.y + (p2.y - p1.y) * (zmax - p1.z) / (p2.z - p1.z);
					z = zmax;
				}
				else if (state & ClipState::back)
				{
					x = p1.x + (p2.x - p1.x) * (zmin - p1.z) / (p2.z - p1.z);
					y = p1.y + (p2.y - p1.y) * (zmin - p1.z) / (p2.z - p1.z);
					z = zmin;
				}
				else if (state & ClipState::top)
				{
					x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
					y = ymax;
					z = p1.z + (p2.z - p1.z) * (ymax - p1.y) / (p2.y - p1.y);
				}
				else if (state & ClipState::down)
				{
					x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
					y = ymin;
					z = p1.z + (p2.z - p1.z) * (ymin - p1.y) / (p2.y - p1.y);
				}
				else if (state & ClipState::right)
				{
					x = xmax;
					y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
					z = p1.z + (p2.z - p1.z) * (xmax - p1.x) / (p2.x - p1.x);
				}
				else if (state & ClipState::left)
				{
					x = xmin;
					y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
					z = p1.z + (p2.z - p1.z) * (xmin - p1.x) / (p2.x - p1.x);
				}

				if (state == p1_state)
				{
					p1 = Point3(x, y, z);
					p1_state = ComputeClipState(p1);
				}
				else
				{
					p2 = Point3(x, y, z);
					p2_state = ComputeClipState(p2);
				}
			}
		}
	}
}