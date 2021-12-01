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
	uint Drawer::Color(Rehenz::Color c)
	{
		return Color(static_cast<uint>(c.x * 255), static_cast<uint>(c.y * 255), static_cast<uint>(c.z * 255));
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
			return;
		else if (p1.y == p2.y)
		{
			if (p1.y != h)
			{
				if (p2.x < p1.x)
					Swap(p1, p2);
				for (int x = p1.x; x < p2.x; x++)
					Pixel(Point2I(x, p1.y), color);
			}
		}
		else if (p1.x == p2.x)
		{
			if (p1.x != w)
			{
				if (p2.y < p1.y)
					Swap(p1, p2);
				for (int y = p1.y; y <= p2.y; y++)
					Pixel(Point2I(p1.x, y), color);
			}
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					Swap(p1, p2);
				int y_move = (p2.y >= p1.y) ? 1 : -1;
				int y_offset = 0; // 0 times of 0
				for (auto p = p1; p.x < p2.x; p.x++)
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
				int x_offset = 0;
				for (auto p = p1; p.y < p2.y; p.y++)
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
	void Drawer::Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r, uint color)
	{
		if (top_l.y != bottom_l.y)
		{
			for (int y = top_l.y; y < bottom_l.y; y++)
			{
				float t = static_cast<float>(y - top_l.y) / (bottom_l.y - top_l.y);
				int xl = Lerp(top_l.x, bottom_l.x, t);
				int xr = Lerp(top_r.x, bottom_r.x, t);
				for (int x = xl; x <= xr; x++)
					Pixel(Point2I(x, y), color);
			}
		}
	}
	void Drawer::Triangle(Point2I p1, Point2I p2, Point2I p3, uint color)
	{
		if (p1.y > p2.y)
			Swap(p1, p2);
		if (p1.y > p3.y)
			Swap(p1, p3);
		if (p2.y > p3.y)
			Swap(p2, p3);

		if (p3.y != p1.y)
		{
			float t = static_cast<float>(p2.y - p1.y) / (p3.y - p1.y);
			int x = Lerp(p1.x, p3.x, t);
			if (p2.x <= x)
			{
				Trapezoid(p1, p1, p2, Point2I(x, p2.y), color);
				Trapezoid(p2, Point2I(x, p2.y), p3, p3, color);
			}
			else
			{
				Trapezoid(p1, p1, Point2I(x, p2.y), p2, color);
				Trapezoid(Point2I(x, p2.y), p2, p3, p3, color);
			}
		}
	}

	void Drawer::Line(Point2 p1, Point2 p2, uint color)
	{
		Point2I p1_i(p1), p2_i(p2);
		Line(p1_i, p2_i, color);
	}

	void Drawer::Trapezoid(Point2 top_l, Point2 top_r, Point2 bottom_l, Point2 bottom_r, uint color)
	{
	}

	void Drawer::Triangle(Point2 p1, Point2 p2, Point2 p3, uint color)
	{
		Point2I p1_i(p1), p2_i(p2), p3_i(p3);
		Triangle(p1_i, p2_i, p3_i, color);
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
	void Drawer::Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r, uint color)
	{
		if (top_l.y != bottom_l.y)
		{
			for (int y = top_l.y; y < bottom_l.y; y++)
			{
				float t = static_cast<float>(y - top_l.y) / (bottom_l.y - top_l.y);
				int xl = Lerp(top_l.x, bottom_l.x, t);
				int xr = Lerp(top_r.x, bottom_r.x, t);
				for (int x = xl; x <= xr; x++)
					Pixel(Point2I(x, y), color);
			}
		}
	}
	void DrawerZ::Trapezoid(const Vertex& top_left, const Vertex& bottom_left, const Vertex& top_right, const Vertex& bottom_right,
		PixelShader pixel_shader, PixelShaderData pshader_data)
	{
		if (top_left.p.y != bottom_left.p.y)
		{
			for (int y = static_cast<int>(bottom_left.p.y); y <= top_left.p.y; y += 1)
			{
				float t = static_cast<float>(y - bottom_left.p.y) / (top_left.p.y - bottom_left.p.y);
				Vertex vl = VertexLerp(bottom_left, top_left, t);
				Vertex vr = VertexLerp(bottom_right, top_right, t);
				if (vl.p.x == vr.p.x)
				{
					int x = static_cast<int>(vl.p.x);
					VertexPerspectiveEnd(vl);
					VertexPerspectiveEnd(vr);
					Pixel(Point2I(x, y), Color(pixel_shader(pshader_data, vl)), -vl.rhw);
					Pixel(Point2I(x, y), Color(pixel_shader(pshader_data, vr)), -vr.rhw);
				}
				else
				{
					for (int x = static_cast<int>(vl.p.x); x <= vr.p.x; x++)
					{
						float t2 = static_cast<float>(x - vl.p.x) / (vr.p.x - vl.p.x);
						Vertex v = VertexLerp(vl, vr, t2);
						VertexPerspectiveEnd(v);
						Pixel(Point2I(x, y), Color(pixel_shader(pshader_data, v)), -v.rhw);
					}
				}
			}
		}
	}

	void DrawerZ::Triangle(const Vertex* v1, const Vertex* v2, const Vertex* v3, PixelShader pixel_shader, PixelShaderData pshader_data)
	{
		if (v1->p.y > v2->p.y)
			Swap(v1, v2);
		if (v1->p.y > v3->p.y)
			Swap(v1, v3);
		if (v2->p.y > v3->p.y)
			Swap(v2, v3);

		if (v3->p.y != v1->p.y)
		{
			float t = static_cast<float>(v2->p.y - v1->p.y) / (v3->p.y - v1->p.y);
			auto v = VertexLerp(*v1, *v3, t);
			if (v2->p.x <= v.p.x)
			{
				Trapezoid(*v3, *v2, *v3, v, pixel_shader, pshader_data);
				Trapezoid(*v2, *v1, v, *v1, pixel_shader, pshader_data);
			}
			else
			{
				Trapezoid(*v3, v, *v3, *v2, pixel_shader, pshader_data);
				Trapezoid(v, *v1, *v2, *v1, pixel_shader, pshader_data);
			}
		}
	}
}