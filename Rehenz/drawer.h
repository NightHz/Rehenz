#pragma once
#include "type.h"
#include "math.h"
#include "render_soft.h"

namespace Rehenz
{
	class Drawer;
	class DrawerZ;



	class Drawer
	{
	protected:
		uint* const buffer;
		const int w;
		const int h;

	public:
		Drawer(uint* _buffer, int _width, int _height);
		~Drawer();

		static uint Color(uint r, uint g, uint b);
		static uint Color(float r, float g, float b);
		static uint Color(Rehenz::Color c);

		// fill with a color
		void Fill(uint color);

		// draw a pixel
		void Pixel(Point2I p, uint color);

		// draw line including all end points
		void Line(Point2I p1, Point2I p2, uint color);

		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r, uint color);
		// draw triangle including all edges
		void Triangle(Point2I p1, Point2I p2, Point2I p3, uint color);
		// draw rectangle including all edges
		void Rectangle(Point2I top_left, Point2I bottom_right, uint color);
	};

	class DrawerZ : public Drawer
	{
	private:
		float* const zbuffer;

	public:
		DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer);
		~DrawerZ();

		void Pixel(Point2I p, uint color, float z);

		using Drawer::Line;

		using Drawer::Trapezoid;
		using Drawer::Triangle;
		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r,
			const Vertex& v_tl, const Vertex& v_tr, const Vertex& v_bl, const Vertex& v_br,
			PixelShader pixel_shader, PixelShaderData pshader_data);
		// draw triangle including all edges
		void Triangle(Point2I p1, Point2I p2, Point2I p3, const Vertex* v1, const Vertex* v2, const Vertex* v3,
			PixelShader pixel_shader, PixelShaderData pshader_data);
	};
}