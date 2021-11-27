#pragma once
#include "type.h"
#include "math.h"

namespace Rehenz
{
	class Drawer;
	class DrawerZ;



	class Drawer
	{
	private:
		uint* const buffer;
		const int w;
		const int h;

	public:
		Drawer(uint* _buffer, int _width, int _height);
		~Drawer();

		static uint Color(uint r, uint g, uint b);
		static uint Color(float r, float g, float b);
		void Pixel(Point2I p, uint color);
		void Line(Point2I p1, Point2I p2, uint color);
	};

	class DrawerZ
	{
	private:
		uint* const buffer;
		const int w;
		const int h;
		float* const zbuffer;

	public:
		DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer);
		~DrawerZ();

		static uint Color(uint r, uint g, uint b);
		static uint Color(float r, float g, float b);
		void Pixel(Point2I p, uint color, float z);
		void Line(Point2I p1, Point2I p2, uint color, float z1, float z2);
	};
}