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

		void Pixel(Point2I p, uint color);
		void Line(Point2I p1, Point2I p2, uint color);
		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(Point2I top_l, Point2I bottom_l, Point2I top_r, Point2I bottom_r, uint color);
		void Triangle(Point2I p1, Point2I p2, Point2I p3, uint color);
	};

	class DrawerZ : public Drawer
	{
	private:
		float* const zbuffer;

	public:
		DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer);
		~DrawerZ();

		using Drawer::Pixel;
		using Drawer::Line;
		using Drawer::Trapezoid;
		using Drawer::Triangle;

		void Pixel(Point2I p, uint color, float z);
		// [Warning] wrong interpolation for z
		void Line(Point2I p1, Point2I p2, uint color, float z1, float z2);
		// [Warning] wrong interpolation for z
		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(Point2I top_l, Point2I bottom_l, Point2I top_r, Point2I bottom_r, uint color, float z_tl, float z_bl, float z_tr, float z_br);
		// [Warning] wrong interpolation for z
		void Triangle(Point2I p1, Point2I p2, Point2I p3, uint color, float z1, float z2, float z3);
		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(Point2I top_l, Point2I bottom_l, Point2I top_r, Point2I bottom_r,
			const Vertex& v_tl, const Vertex& v_bl, const Vertex& v_tr, const Vertex& v_br,
			PixelShader pixel_shader, PixelShaderData pshader_data);
		void Triangle(Point2I p1, Point2I p2, Point2I p3, const Vertex* v1, const Vertex* v2, const Vertex* v3,
			PixelShader pixel_shader, PixelShaderData pshader_data);
	};
}