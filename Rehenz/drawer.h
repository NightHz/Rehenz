#pragma once
#include "type.h"
#include "math.h"
#include "mesh.h"
#include "color.h"

namespace Rehenz
{
	class DrawerBase;
	class Drawer;
	class DrawerF;
	class DrawerV;



	// drawer base class, provide Fill, Pixel
	class DrawerBase
	{
	protected:
		uint* const buffer;
		const int w;
		const int h;

	public:
		DrawerBase(uint* _buffer, int _width, int _height);
		~DrawerBase();

		// only for old code
		inline static uint ColorRGB(int r, int g, int b)
		{
			return Rehenz::ColorRGB(r, g, b);
		}
		// only for old code
		inline static uint ColorRGB(Color c)
		{
			return Rehenz::ColorRGB(c);
		}
		// only for old code
		static uint white;

		// fill with a color
		void Fill(uint color);

		// draw a pixel
		void Pixel(Point2I p, uint color);
	};

	// draw point which aligned pixel center, draw region: [0,w-1]x[0,h-1]
	class Drawer : public DrawerBase
	{
	private:
		// dy must >= 0
		// (xi, dxi, dyi) define line
		// dyi must >= 0
		// line1 must be to the left of line2
		void Trapezoid(int y, int dy, int x1, int dx1, int dy1, int x2, int dx2, int dy2, uint color);

		// dy must >= 0
		// (xi, xi_offset, dxi, dyi) define line
		// dyi must >= 0
		// line1 must be to the left of line2
		// output x1, x2 pos when stop
		void Trapezoid(int y, int dy, int& x1, int& x1_offset, int dx1, int dy1,
			int& x2, int& x2_offset, int dx2, int dy2, uint color);

	public:
		Drawer(uint* _buffer, int _width, int _height);
		~Drawer();

		// draw line including all end points
		void Line(Point2I p1, Point2I p2, uint color);

		// draw triangle including all edges
		void Triangle(Point2I p1, Point2I p2, Point2I p3, uint color);

		// draw rectangle including all edges
		void Rectangle(Point2I p1, Point2I p2, uint color);
	};

	// draw point which based float, draw region: [0,w]x[0,h]
	class DrawerF : public DrawerBase
	{
	private:
		// 3.3f -> 3.5f
		// 4.5f -> 4.5f
		// 5.7f -> 6.5f
		inline float NextHalf(float x)
		{
			float x2 = static_cast<int>(x + 0.5f) + 0.5f;
			if (x2 == x + 1.0f)
				x2 = x;
			return x2;
		}

		// y must be aligned to .5
		// (xi, ai = dx/dy) define line
		// line1 must be to the left of line2
		// output x1, x2, y pos when stop
		void Trapezoid(float& y, float y_bottom, float& x1, float a1, float& x2, float a2, uint color);

	public:
		DrawerF(uint* _buffer, int _width, int _height);
		~DrawerF();

		using DrawerBase::Pixel;

		// draw a pixel
		void Pixel(Point2 p, uint color);

		// draw line
		//   sample points are aligned horizontally or vertically with grid center
		//   sample endpoint when endpoint length > half pixel
		void Line(Point2 p1, Point2 p2, uint color);

		// draw triangle
		//   sample point when point is inside
		//   sample point when point is on edge and the right of point is inside
		//   sample point when point is on edge and the right of point is on edge and the bottom of right of point is inside
		// a equivalent expression :
		//   define neighbourhood of p  :  p + d * [cos(t) sin(t)], d,t is small positive
		//   sample p when neighbourhood of p all inside
		// an implement :
		//   scan from top to bottom, scan top and not scan bottom
		//   then sample left and not sample right
		// a possible problem :
		//   due to float step, maybe cross .5 line after multiple steps
		//   if happened, maybe leave holes or rasterized more than once
		//   modify to lerp rather then step to avoid the problem
		void Triangle(Point2 p1, Point2 p2, Point2 p3, uint color);

		// draw rectangle
		//   draw left edge and top edge and left top corner
		void Rectangle(Point2 p1, Point2 p2, uint color);
	};

	// draw Vertex which based float, draw region: [0,w]x[0,h]
	// use z-buffer
	class DrawerV : public DrawerBase
	{
	private:
		float* const zbuffer;

		PixelShader ps;
		const PixelShaderData* ps_data;

		// 3.3f -> 3.5f
		// 4.5f -> 4.5f
		// 5.7f -> 6.5f
		inline float NextHalf(float x)
		{
			float x2 = static_cast<int>(x + 0.5f) + 0.5f;
			if (x2 == x + 1.0f)
				x2 = x;
			return x2;
		}

		// draw a pixel
		void Pixel(const Vertex& v);

		// y must be aligned to .5
		// (vi, ai = dv/dy) define line
		// line1 must be to the left of line2
		// output v1, v2, y pos when stop
		void Trapezoid(float& y, float y_bottom, Vertex& v1, const Vertex& a1, Vertex& v2, const Vertex& a2);

	public:
		DrawerV(uint* _buffer, int _width, int _height, float* _zbuffer);
		~DrawerV();

		// fill z-buffer
		void FillZ(float z);

		// draw triangle
		// rasterization rule is same with DrawerF::Triangle, see it to get more info
		void Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3,
			PixelShader pixel_shader, const PixelShaderData& _ps_data);
	};
}