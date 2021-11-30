#include "clipper.h"

namespace Rehenz
{
	int ComputeClipState(Point2 p)
	{
		int state = ClipState::inside;
		if (p.x < 0)      state |= ClipState::left;
		else if (p.x > 1) state |= ClipState::right;
		if (p.y < 0)      state |= ClipState::down;
		else if (p.y > 1) state |= ClipState::top;
		return state;
	}

	bool CohenSutherlandClip2D(Point2& p1, Point2& p2)
	{
		const float xmin = 0, xmax = 1;
		const float ymin = 0, ymax = 1;

		int p1_state = ComputeClipState(p1);
		int p2_state = ComputeClipState(p2);
		bool accept = false;

		while (true) {
			if (!(p1_state | p2_state)) {
				// bitwise OR is 0: both points inside window; trivially accept and exit loop
				accept = true;
				break;
			}
			else if (p1_state & p2_state) {
				// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
				// or BOTTOM), so both must be outside window; exit loop (accept is false)
				break;
			}
			else {
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
				if (outcodeOut & ClipState::top) {           // point is above the clip window
					x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
					y = ymax;
				}
				else if (outcodeOut & ClipState::down) { // point is below the clip window
					x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
					y = ymin;
				}
				else if (outcodeOut & ClipState::right) {  // point is to the right of clip window
					y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
					x = xmax;
				}
				else if (outcodeOut & ClipState::left) {   // point is to the left of clip window
					y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
					x = xmin;
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == p1_state) {
					p1 = Point2(x, y);
					p1_state = ComputeClipState(p1);
				}
				else {
					p2 = Point2(x, y);
					p2_state = ComputeClipState(p2);
				}
			}
		}

		return accept;
	}
}