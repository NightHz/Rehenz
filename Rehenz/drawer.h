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

		inline static uint ColorRGB(byte r, byte g, byte b)
		{
			return (r << 16) | (g << 8) | (b << 0);
		}
		inline static uint ColorRGB(int r, int g, int b)
		{
			return ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff) << 0);
		}
		inline static uint ColorRGB(uint r, uint g, uint b)
		{
			return ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff) << 0);
		}
		inline static uint ColorRGB(float r, float g, float b)
		{
			return ColorRGB(static_cast<byte>(r * 0xff), static_cast<byte>(g * 0xff), static_cast<byte>(b * 0xff));
		}
		inline static uint ColorRGB(Color c)
		{
			return ColorRGB(static_cast<byte>(c.x * 0xff), static_cast<byte>(c.y * 0xff), static_cast<byte>(c.z * 0xff));
		}

		inline static uint ColorARGB(byte r, byte g, byte b, byte a)
		{
			return (a << 24) | (r << 16) | (g << 8) | (b << 0);
		}
		inline static uint ColorARGB(int r, int g, int b, int a)
		{
			return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff) << 0);
		}
		inline static uint ColorARGB(uint r, uint g, uint b, uint a)
		{
			return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff) << 0);
		}
		inline static uint ColorARGB(float r, float g, float b, float a)
		{
			return ColorARGB(static_cast<byte>(r * 0xff), static_cast<byte>(g * 0xff), static_cast<byte>(b * 0xff), static_cast<byte>(a * 0xff));
		}
		inline static uint ColorARGB(Color c)
		{
			return ColorARGB(static_cast<byte>(c.x * 0xff), static_cast<byte>(c.y * 0xff), static_cast<byte>(c.z * 0xff), static_cast<byte>(c.w * 0xff));
		}

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
	protected:
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