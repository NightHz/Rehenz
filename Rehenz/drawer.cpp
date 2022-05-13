#include "drawer.h"
#include "util.h"
#include <cassert>

namespace Rehenz
{
	DrawerBase::DrawerBase(uint* _buffer, int _width, int _height)
		: buffer(_buffer), w(_width), h(_height)
	{
	}

	DrawerBase::~DrawerBase()
	{
	}

	uint DrawerBase::red_l = ColorRGB(242, 129, 128);
	uint DrawerBase::yellow_l = ColorRGB(223, 218, 129);
	uint DrawerBase::blue_l = ColorRGB(141, 219, 254);
	uint DrawerBase::green_l = ColorRGB(149, 224, 129);
	uint DrawerBase::purple_l = ColorRGB(141, 133, 253);
	uint DrawerBase::pink_l = ColorRGB(231, 162, 244);
	uint DrawerBase::orange = ColorRGB(255, 178, 125);

	void DrawerBase::Fill(uint color)
	{
		int s = w * h;
		for (int i = 0; i < s; i++)
			buffer[i] = color;
	}

	void DrawerBase::Pixel(Point2I p, uint color)
	{
		assert(p.x >= 0 && p.x < w&& p.y >= 0 && p.y < h);
		int i = p.y * w + p.x;
		buffer[i] = color;
	}

	void Drawer::Trapezoid(int y, int dy, int x1, int dx1, int dy1, int x2, int dx2, int dy2, uint color)
	{
		int x1_offset = dy1 / 2;
		int x2_offset = dy2 / 2;
		Trapezoid(y, dy, x1, x1_offset, dx1, dy1, x2, x2_offset, dx2, dy2, color);
	}

	void Drawer::Trapezoid(int y, int dy, int& x1, int& x1_offset, int dx1, int dy1,
		int& x2, int& x2_offset, int dx2, int dy2, uint color)
	{
		int y2 = y + dy;
		for (; y <= y2; y++)
		{
			while (x1_offset >= dy1)
			{
				x1_offset -= dy1;
				x1++;
			}
			while (x1_offset < 0)
			{
				x1_offset += dy1;
				x1--;
			}
			while (x2_offset >= dy2)
			{
				x2_offset -= dy2;
				x2++;
			}
			while (x2_offset < 0)
			{
				x2_offset += dy2;
				x2--;
			}
			for (int x = x1; x <= x2; x++)
				Pixel(Point2I(x, y), color);
			x1_offset += dx1;
			x2_offset += dx2;
		}
	}

	Drawer::Drawer(uint* _buffer, int _width, int _height)
		: DrawerBase(_buffer, _width, _height)
	{
	}

	Drawer::~Drawer()
	{
	}

	void Drawer::Line(Point2I p1, Point2I p2, uint color)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color);
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
				std::swap(p1, p2);
			for (int x = p1.x; x <= p2.x; x++)
				Pixel(Point2I(x, p1.y), color);
		}
		else if (p1.x == p2.x)
		{
			if (p2.y < p1.y)
				std::swap(p1, p2);
			for (int y = p1.y; y <= p2.y; y++)
				Pixel(Point2I(p1.x, y), color);
		}
		else
		{
			int dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					std::swap(p1, p2);
				int y_offset = dx / 2; // dx times of 1/2
				if (p2.y >= p1.y)
				{
					for (auto p = p1; p.x <= p2.x; p.x++)
					{
						Pixel(p, color);
						y_offset += dy;     // dx times of dy/dx
						if (y_offset >= dx) // dx times of >= 1
						{
							y_offset -= dx; // dx times of -= 1
							p.y++;
						}
					}
				}
				else
				{
					for (auto p = p1; p.x <= p2.x; p.x++)
					{
						Pixel(p, color);
						y_offset -= dy;     // dx times of dy/dx
						if (y_offset < 0)   // dx times of < 0
						{
							y_offset += dx; // dx times of += 1
							p.y--;
						}
					}
				}
			}
			else
			{
				if (p2.y < p1.y)
					std::swap(p1, p2);
				int x_offset = dy / 2;
				if (p2.x >= p1.x)
				{
					for (auto p = p1; p.y <= p2.y; p.y++)
					{
						Pixel(p, color);
						x_offset += dx;
						if (x_offset >= dy)
						{
							x_offset -= dy;
							p.x++;
						}
					}
				}
				else
				{
					for (auto p = p1; p.y <= p2.y; p.y++)
					{
						Pixel(p, color);
						x_offset -= dx;
						if (x_offset < 0)
						{
							x_offset += dy;
							p.x--;
						}
					}
				}
			}
		}
	}

	void Drawer::Triangle(Point2I p1, Point2I p2, Point2I p3, uint color)
	{
		if (p3.y < p2.y)
			std::swap(p2, p3);
		if (p2.y < p1.y)
			std::swap(p1, p2);
		if (p3.y < p2.y)
			std::swap(p2, p3);
		// now p1.y <= p2.y <= p3.y
		if (p1.y == p3.y)
		{
			int xmin = Min(p1.x, p2.x, p3.x);
			int xmax = Max(p1.x, p2.x, p3.x);
			for (int x = xmin; x <= xmax; x++)
				Pixel(Point2I(x, p1.y), color);
		}
		else if (p1.y == p2.y)
		{
			Sort(p1.x, p2.x);
			int dy = p3.y - p1.y;
			int dx13 = p3.x - p1.x;
			int dx23 = p3.x - p2.x;
			Trapezoid(p1.y, dy, p1.x, dx13, dy, p2.x, dx23, dy, color);
			Line(p1, p3, color);
			Line(p2, p3, color);
		}
		else if (p2.y == p3.y)
		{
			Sort(p2.x, p3.x);
			int dy = p3.y - p1.y;
			int dx12 = p2.x - p1.x;
			int dx13 = p3.x - p1.x;
			Trapezoid(p1.y, dy, p1.x, dx12, dy, p1.x, dx13, dy, color);
			Line(p1, p2, color);
			Line(p1, p3, color);
		}
		else
		{
			int dy12 = p2.y - p1.y;
			int dy23 = p3.y - p2.y;
			int dy13 = p3.y - p1.y;
			int dx12 = p2.x - p1.x;
			int dx23 = p3.x - p2.x;
			int dx13 = p3.x - p1.x;
			if (dx12 * dy13 <= dy12 * dx13)
			{
				// line12 is to the left of line13
				int x12_offset = dy12 / 2;
				int x13_offset = dy13 / 2;
				int x12 = p1.x;
				int x13 = p1.x;
				Trapezoid(p1.y, dy12 - 1, x12, x12_offset, dx12, dy12, x13, x13_offset, dx13, dy13, color);
				int x23_offset = dy23 / 2;
				int x23 = p2.x;
				Trapezoid(p2.y, dy23, x23, x23_offset, dx23, dy23, x13, x13_offset, dx13, dy13, color);
			}
			else
			{
				// line12 is to the right of line13
				int x12_offset = dy12 / 2;
				int x13_offset = dy13 / 2;
				int x12 = p1.x;
				int x13 = p1.x;
				Trapezoid(p1.y, dy12 - 1, x13, x13_offset, dx13, dy13, x12, x12_offset, dx12, dy12, color);
				int x23_offset = dy23 / 2;
				int x23 = p2.x;
				Trapezoid(p2.y, dy23, x13, x13_offset, dx13, dy13, x23, x23_offset, dx23, dy23, color);
			}
			Line(p1, p2, color);
			Line(p1, p3, color);
			Line(p2, p3, color);
		}
	}

	void Drawer::Rectangle(Point2I p1, Point2I p2, uint color)
	{
		Sort(p1.x, p2.x);
		Sort(p1.y, p2.y);
		for (int y = p1.y; y <= p2.y; y++)
		{
			for (int x = p1.x; x <= p2.x; x++)
				Pixel(Point2I(x, y), color);
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
					Pixel(Point2I(xl, y), ColorRGB(pixel_shader(pshader_data, vl)), vl.p.w);
					Pixel(Point2I(xl, y), ColorRGB(pixel_shader(pshader_data, vr)), vr.p.w);
				}
				else
				{
					Vertex v_step = (vr - vl) * (1 / static_cast<float>(xr - xl));
					Vertex v_current = vl;
					for (int x = xl; x <= xr; x++)
					{
						Vertex v = v_current;
						VertexNormalize(v);
						Pixel(Point2I(x, y), ColorRGB(pixel_shader(pshader_data, v)), v.p.w);
						v_current += v_step;
					}
				}
			}
		}
	}

	void DrawerZ::Triangle(Point2I p1, Point2I p2, Point2I p3, const Vertex* v1, const Vertex* v2, const Vertex* v3,
		PixelShader pixel_shader, PixelShaderData pshader_data)
	{
		if (p1.y > p2.y)
			std::swap(p1, p2), std::swap(v1, v2);
		if (p1.y > p3.y)
			std::swap(p1, p3), std::swap(v1, v3);
		if (p2.y > p3.y)
			std::swap(p2, p3), std::swap(v2, v3);

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

	void DrawerF::Trapezoid(float& y, float y_bottom, float& x1, float a1, float& x2, float a2, uint color)
	{
		for (; y < y_bottom; y++)
		{
			for (float x = NextHalf(x1); x < x2; x += 1.0f)
				Pixel(Point2I(static_cast<int>(x), static_cast<int>(y)), color);
			x1 += a1;
			x2 += a2;
		}
	}

	DrawerF::DrawerF(uint* _buffer, int _width, int _height)
		: DrawerBase(_buffer, _width, _height)
	{
	}

	DrawerF::~DrawerF()
	{
	}

	void DrawerF::Pixel(Point2 p, uint color)
	{
		if (p.x == w)
			p.x -= 0.1f;
		if (p.y == h)
			p.y -= 0.1f;
		Pixel(Point2I(p), color);
	}

	void DrawerF::Line(Point2 p1, Point2 p2, uint color)
	{
		if (p1.x == p2.x && p1.y == p2.y)
			Pixel(p1, color);
		else
		{
			float dx = abs(p1.x - p2.x), dy = abs(p1.y - p2.y);
			if (dx >= dy)
			{
				if (p2.x < p1.x)
					std::swap(p1, p2);
				float a = (p2.y - p1.y) / dx;
				float x = static_cast<int>(p1.x + 0.5f) + 0.5f;
				float y = p1.y + a * (x - p1.x);
				if (x - 0.5f > p1.x)
					Pixel(p1, color);
				for (; x < p2.x; x += 1.0f)
				{
					// avoid y out of bounds caused by float precision
					Point2I sp(static_cast<int>(x), static_cast<int>(y));
					if (sp.y == -1)
						sp.y++;
					else if (sp.y == h)
						sp.y--;
					Pixel(sp, color);
					y += a;
				}
				if (x - 0.5f < p2.x)
					Pixel(p2, color);
			}
			else
			{
				if (p2.y < p1.y)
					std::swap(p1, p2);
				float a = (p2.x - p1.x) / dy;
				float y = static_cast<int>(p1.y + 0.5f) + 0.5f;
				float x = p1.x + a * (y - p1.y);
				if (y - 0.5f > p1.y)
					Pixel(p1, color);
				for (; y < p2.y; y += 1.0f)
				{
					// avoid x out of bounds caused by float precision
					Point2I sp(static_cast<int>(x), static_cast<int>(y));
					if (sp.x == -1)
						sp.x++;
					else if (sp.x == w)
						sp.x--;
					Pixel(sp, color);
					x += a;
				}
				if (y - 0.5f < p2.y)
					Pixel(p2, color);
			}
		}
	}

	void DrawerF::Triangle(Point2 p1, Point2 p2, Point2 p3, uint color)
	{
		if (p3.y < p2.y)
			std::swap(p2, p3);
		if (p2.y < p1.y)
			std::swap(p1, p2);
		if (p3.y < p2.y)
			std::swap(p2, p3);
		// now p1.y <= p2.y <= p3.y
		if (p1.y == p3.y)
			return;
		else if (p1.y == p2.y)
		{
			Sort(p1.x, p2.x);
			float y = NextHalf(p1.y);
			float a13 = (p3.x - p1.x) / (p3.y - p1.y);
			float a23 = (p3.x - p2.x) / (p3.y - p2.y);
			float x13 = p1.x + a13 * (y - p1.y);
			float x23 = p2.x + a23 * (y - p2.y);
			Trapezoid(y, p3.y, x13, a13, x23, a23, color);
		}
		else if (p2.y == p3.y)
		{
			Sort(p2.x, p3.x);
			float y = NextHalf(p1.y);
			float a12 = (p2.x - p1.x) / (p2.y - p1.y);
			float a13 = (p3.x - p1.x) / (p3.y - p1.y);
			float x12 = p1.x + a12 * (y - p1.y);
			float x13 = p1.x + a13 * (y - p1.y);
			Trapezoid(y, p3.y, x12, a12, x13, a13, color);
		}
		else
		{
			float dy12 = p2.y - p1.y;
			float dy13 = p3.y - p1.y;
			float dx12 = p2.x - p1.x;
			float dx13 = p3.x - p1.x;
			if (dx12 * dy13 <= dy12 * dx13)
			{
				// line12 is to the left of line13
				float y = NextHalf(p1.y);
				float a12 = (p2.x - p1.x) / (p2.y - p1.y);
				float a13 = (p3.x - p1.x) / (p3.y - p1.y);
				float x12 = p1.x + a12 * (y - p1.y);
				float x13 = p1.x + a13 * (y - p1.y);
				Trapezoid(y, p2.y, x12, a12, x13, a13, color);
				float a23 = (p3.x - p2.x) / (p3.y - p2.y);
				float x23 = p2.x + a23 * (y - p2.y);
				Trapezoid(y, p3.y, x23, a23, x13, a13, color);
			}
			else
			{
				// line12 is to the right of line13
				float y = NextHalf(p1.y);
				float a12 = (p2.x - p1.x) / (p2.y - p1.y);
				float a13 = (p3.x - p1.x) / (p3.y - p1.y);
				float x12 = p1.x + a12 * (y - p1.y);
				float x13 = p1.x + a13 * (y - p1.y);
				Trapezoid(y, p2.y, x13, a13, x12, a12, color);
				float a23 = (p3.x - p2.x) / (p3.y - p2.y);
				float x23 = p2.x + a23 * (y - p2.y);
				Trapezoid(y, p3.y, x13, a13, x23, a23, color);
			}
		}
	}

}