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
	uint Drawer::Color(Vector c)
	{
		return Color(static_cast<uint>(c(0) * 255), static_cast<uint>(c(1) * 255), static_cast<uint>(c(2) * 255));
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
		: Drawer(_buffer, _width, _height), zbuffer(_zbuffer)
	{
	}
	DrawerZ::~DrawerZ()
	{
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
	void DrawerZ::Trapezoid(Point2I top_l, Point2I bottom_l, Point2I top_r, Point2I bottom_r, uint color, float z_tl, float z_bl, float z_tr, float z_br)
	{
		for (int y = bottom_l.y; y <= top_l.y; y++)
		{
			float t = static_cast<float>(y - bottom_l.y) / (top_l.y - bottom_l.y);
			int xl = Lerp(bottom_l.x, top_l.x, t);
			int xr = Lerp(bottom_r.x, top_r.x, t);
			float zl = Lerp(z_bl, z_tl, t);
			float zr = Lerp(z_br, z_tr, t);
			for (int x = xl; x <= xr; x++)
				Pixel(Point2I(x, y), color, Lerp(zl, zr, static_cast<float>(x - xl) / (xr - xl)));
		}
	}
	void DrawerZ::Triangle(Point2I p1, Point2I p2, Point2I p3, uint color, float z1, float z2, float z3)
	{
		if (p1.y > p2.y)
			Swap(p1, p2), Swap(z1, z2);
		if (p1.y > p3.y)
			Swap(p1, p3), Swap(z1, z3);
		if (p2.y > p3.y)
			Swap(p2, p3), Swap(z2, z3);

		float t = static_cast<float>(p2.y - p1.y) / (p3.y - p1.y);
		int x = Lerp(p1.x, p3.x, t);
		float z = Lerp(z1, z3, t);
		if (p2.x <= x)
		{
			Trapezoid(p3, p2, p3, Point2I(x, p2.y), color, z3, z2, z3, z);
			Trapezoid(p2, p1, Point2I(x, p2.y), p1, color, z2, z1, z, z1);
		}
		else
		{
			Trapezoid(p3,  Point2I(x, p2.y),p3, p2, color, z3, z, z3, z2);
			Trapezoid(Point2I(x, p2.y), p1, p2, p1, color, z, z1, z2, z1);
		}
	}
	void DrawerZ::Trapezoid(Point2I top_l, Point2I bottom_l, Point2I top_r, Point2I bottom_r, const Vertex& v_tl, const Vertex& v_bl, const Vertex& v_tr, const Vertex& v_br)
	{
		for (int y = bottom_l.y; y <= top_l.y; y++)
		{
			float t = static_cast<float>(y - bottom_l.y) / (top_l.y - bottom_l.y);
			int xl = Lerp(bottom_l.x, top_l.x, t);
			int xr = Lerp(bottom_r.x, top_r.x, t);
			Vertex vl = VertexLerp(v_bl, v_tl, t);
			Vertex vr = VertexLerp(v_br, v_tr, t);
			for (int x = xl; x <= xr; x++)
			{
				float t2 = static_cast<float>(x - xl) / (xr - xl);
				Vertex v = VertexLerp(vl, vr, t2);
				Pixel(Point2I(x, y), Color(v.c), v.p.z);
			}
		}
	}

	void DrawerZ::Triangle(Point2I p1, Point2I p2, Point2I p3, const Vertex* v1, const Vertex* v2, const Vertex* v3)
	{
		if (p1.y > p2.y)
			Swap(p1, p2), Swap(v1, v2);
		if (p1.y > p3.y)
			Swap(p1, p3), Swap(v1, v3);
		if (p2.y > p3.y)
			Swap(p2, p3), Swap(v2, v3);

		float t = static_cast<float>(p2.y - p1.y) / (p3.y - p1.y);
		int x = Lerp(p1.x, p3.x, t);
		auto v = VertexLerp(*v1, *v3, t);
		if (p2.x <= x)
		{
			Trapezoid(p3, p2, p3, Point2I(x, p2.y), *v3, *v2, *v3, v);
			Trapezoid(p2, p1, Point2I(x, p2.y), p1, *v2, *v1, v, *v1);
		}
		else
		{
			Trapezoid(p3, Point2I(x, p2.y), p3, p2, *v3, v, *v3, *v2);
			Trapezoid(Point2I(x, p2.y), p1, p2, p1, v, *v1, *v2, *v1);
		}
	}
}