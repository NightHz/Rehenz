#pragma once
#include "type.h"
#include "math.h"
#include "render_soft.h"

// need rewrite the file using rasterization rules
// url: https://docs.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-rasterizer-stage-rules

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
		void Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r, uint color);
		void Triangle(Point2I p1, Point2I p2, Point2I p3, uint color);

		void Line(Point2 p1, Point2 p2, uint color);
		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(Point2 top_l, Point2 top_r, Point2 bottom_l, Point2 bottom_r, uint color);
		void Triangle(Point2 p1, Point2 p2, Point2 p3, uint color);
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

		// require top_l and top_r have same y, and bottom_l and bottom_r have same y
		void Trapezoid(const Vertex& top_left, const Vertex& bottom_left, const Vertex& top_right, const Vertex& bottom_right,
			PixelShader pixel_shader, PixelShaderData pshader_data);
		void Triangle(const Vertex* v1, const Vertex* v2, const Vertex* v3, PixelShader pixel_shader, PixelShaderData pshader_data);
	};
}