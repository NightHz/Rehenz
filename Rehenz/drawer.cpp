#include "drawer.h"

namespace Rehenz
{
	Drawer::Drawer(uint* _buffer, int _width, int _height)
		: buffer(_buffer), w(_width), h(_height)
	{
	}
	Drawer::~Drawer()
	{
	}
	uint Drawer::Color(uint r, uint g, uint b)
	{
		return ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff) << 0);
	}
	uint Drawer::Color(float r, float g, float b)
	{
		return Color(static_cast<uint>(r * 255), static_cast<uint>(g * 255), static_cast<uint>(b * 255));
	}
	void Drawer::Pixel(Point2I p, uint color)
	{
		//if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h)
		int i = p.y * w + p.x;
		buffer[i] = color;
	}
	void Drawer::Line(Point2I p1, Point2I p2, uint color)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color);
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				Swap(p1, p2);
			for (int x = p1.x; x <= p2.x; x++)
				Pixel(Point2I(x, p1.y), color);
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				Swap(p1, p2);
			for (int y = p1.y; y <= p2.y; y++)
				Pixel(Point2I(p1.x, y), color);
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2);
				int y_move = (p2.y >= p1.y) ? 1 : -1;
				int y_offset = dx / 2; // dx times of 1/2
				for (auto p = p1; p.x <= p2.x; p.x++)
				{
					Pixel(p, color);
					y_offset += dy;     // dx times of dy/dx
					if (y_offset >= dx) // dx times of >= 1
					{
						y_offset -= dx; // dx times of -= 1
						p.y += y_move;
					}
				}
			}
			else
			{
				if (p2.y < p1.y)
					Swap(p1, p2);
				int x_move = (p2.x >= p1.x) ? 1 : -1;
				int x_offset = dy / 2;
				for (auto p = p1; p.y <= p2.y; p.y++)
				{
					Pixel(p, color);
					x_offset += dx;
					if (x_offset >= dy)
					{
						x_offset -= dy;
						p.x += x_move;
					}
				}
			}
		}
	}
	DrawerZ::DrawerZ(uint* _buffer, int _width, int _height, float* _zbuffer)
		: buffer(_buffer), w(_width), h(_height), zbuffer(_zbuffer)
	{
	}
	DrawerZ::~DrawerZ()
	{
	}
	uint DrawerZ::Color(uint r, uint g, uint b)
	{
		return Drawer::Color(r, g, b);
	}
	uint DrawerZ::Color(float r, float g, float b)
	{
		return Drawer::Color(r, g, b);
	}
	void DrawerZ::Pixel(Point2I p, uint color, float z)
	{
		//if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h && z >= 0 && z <= 1)
		int i = p.y * w + p.x;
		if (z < zbuffer[i])
		{
			buffer[i] = color;
			zbuffer[i] = z;
		}
	}
	void DrawerZ::Line(Point2I p1, Point2I p2, uint color, float z1, float z2)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color, Min(z1, z2));
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				Swap(p1, p2), Swap(z1, z2);
			for (int x = p1.x; x <= p2.x; x++)
			{
				float t = static_cast<float>(x - p1.x) / (p2.x - p1.x);
				Pixel(Point2I(x, p1.y), color, Lerp(z1, z2, t));
			}
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				Swap(p1, p2), Swap(z1, z2);
			for (int y = p1.y; y <= p2.y; y++)
			{
				float t = static_cast<float>(y - p1.y) / (p2.y - p1.y);
				Pixel(Point2I(p1.x, y), color, Lerp(z1, z2, t));
			}
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2), Swap(z1, z2);
				for (auto p = p1; p.x <= p2.x; p.x++)
				{
					float t = static_cast<float>(p.x - p1.x) / (p2.x - p1.x);
					p.y = Lerp(p1.y, p2.y, t);
					Pixel(p, color, Lerp(z1, z2, t));
				}
			}
			else
			{
				if (p2.y < p1.y)
					Swap(p1, p2), Swap(z1, z2);
				for (auto p = p1; p.y <= p2.y; p.y++)
				{
					float t = static_cast<float>(p.y - p1.y) / (p2.y - p1.y);
					p.x = Lerp(p1.x, p2.x, t);
					Pixel(p, color, Lerp(z1, z2, t));
				}
			}
		}
	}
}