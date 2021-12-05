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
	void Drawer::Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r, uint color)
	{
		if (top_l.y != bottom_l.y)
		{
			for (int y = top_l.y; y <= bottom_l.y; y++)
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

	void DrawerZ::Trapezoid(Point2I top_l, Point2I top_r, Point2I bottom_l, Point2I bottom_r,
		const Vertex& v_tl, const Vertex& v_tr, const Vertex& v_bl, const Vertex& v_br,
		PixelShader pixel_shader, PixelShaderData pshader_data)
	{
		if (top_l.y != bottom_l.y)
		{
			for (int y = top_l.y; y <= bottom_l.y; y++)
			{
				float t = static_cast<float>(y - top_l.y) / (bottom_l.y - top_l.y);
				int xl = Lerp(top_l.x, bottom_l.x, t);
				int xr = Lerp(top_r.x, bottom_r.x, t);
				Vertex vl = VertexLerp(v_tl, v_bl, t);
				Vertex vr = VertexLerp(v_tr, v_br, t);
				if (xl == xr)
				{
					VertexNormalize(vl);
					VertexNormalize(vr);
					Pixel(Point2I(xl, y), Color(pixel_shader(pshader_data, vl)), vl.p.w);
					Pixel(Point2I(xl, y), Color(pixel_shader(pshader_data, vr)), vr.p.w);
				}
				else
				{
					for (int x = xl; x <= xr; x++)
					{
						float t2 = static_cast<float>(x - xl) / (xr - xl);
						Vertex v = VertexLerp(vl, vr, t2);
						VertexNormalize(v);
						Pixel(Point2I(x, y), Color(pixel_shader(pshader_data, v)), v.p.w);
					}
				}
			}
		}
	}

	void DrawerZ::Triangle(Point2I p1, Point2I p2, Point2I p3, const Vertex* v1, const Vertex* v2, const Vertex* v3,
		PixelShader pixel_shader, PixelShaderData pshader_data)
	{
		if (p1.y > p2.y)
			Swap(p1, p2), Swap(v1, v2);
		if (p1.y > p3.y)
			Swap(p1, p3), Swap(v1, v3);
		if (p2.y > p3.y)
			Swap(p2, p3), Swap(v2, v3);

		if (p3.y != p1.y)
		{
			float t = static_cast<float>(p2.y - p1.y) / (p3.y - p1.y);
			int x = Lerp(p1.x, p3.x, t);
			auto v = VertexLerp(*v1, *v3, t);
			if (p2.x <= x)
			{
				Trapezoid(p1, p1, p2, Point2I(x, p2.y), *v1, *v1, *v2, v, pixel_shader, pshader_data);
				Trapezoid(p2, Point2I(x, p2.y), p3, p3, *v2, v, *v3, *v3, pixel_shader, pshader_data);
			}
			else
			{
				Trapezoid(p1, p1, Point2I(x, p2.y), p2, *v1, *v1, v, *v2, pixel_shader, pshader_data);
				Trapezoid(Point2I(x, p2.y), p2, p3, p3, v, *v2, *v3, *v3, pixel_shader, pshader_data);
			}
		}
	}
}