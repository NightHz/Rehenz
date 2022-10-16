#pragma once
#include "type.h"
#include "math.h"

namespace Rehenz
{
	// 4-component color, r=x, b=y, g=z, a=w, defualt a=1
	struct Color : public Point
	{
	public:
		// default a = 1
		Color() : Point() {}
		Color(Vector v) : Point(v) {}
		explicit Color(float value) : Point(value) {}
		explicit Color(float _x, float _y, float _z, float _w = 1) : Point(_x, _y, _z, _w) {}

	public:
		// 8 pure color
		static Color white, black, red, green, blue, yellow, magenta, cyan;
		// 7 light color
		static Color red_l, yellow_l, blue_l, green_l, purple_l, pink_l, orange_l;
		// 12 color for tilemap
		static Color yellow_tm, yellow_green_tm, green_tm, blue_tm, purple_tm;
		static Color pink_tm, orange_tm, gray_tm, black_tm;
		static Color red_dark_tm, blue_dark_tm, green_dark_tm;
		// 2 other color
		static Color yellow_dark_o, yellow_green_o;
	};

	// clmap to [0,1]
	inline Color ColorSaturate(Color c)
	{
		return Color(Clamp(c.x, 0.0f, 1.0f), Clamp(c.y, 0.0f, 1.0f), Clamp(c.z, 0.0f, 1.0f), Clamp(c.w, 0.0f, 1.0f));
	}

	Color operator*(Color c1, Color c2);

	// ouput format : high-(0,R,G,B)-low = 32bppBGR
	inline static uint ColorRGB(int r, int g, int b)
	{
		return ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff) << 0);
	}
	// ouput format : high-(0,R,G,B)-low = 32bppBGR
	inline static uint ColorRGB(Color c)
	{
		return ColorRGB(static_cast<int>(c.x * 0xff), static_cast<int>(c.y * 0xff), static_cast<int>(c.z * 0xff));
	}

	struct ColorRGB
	{
	public:
		ColorRGB() = delete;
		// 8 pure color
		static uint white, black, red, green, blue, yellow, magenta, cyan;
		// 7 light color
		static uint red_l, yellow_l, blue_l, green_l, purple_l, pink_l, orange_l;
		// 12 color for tilemap
		static uint yellow_tm, yellow_green_tm, green_tm, blue_tm, purple_tm;
		static uint pink_tm, orange_tm, gray_tm, black_tm;
		static uint red_dark_tm, blue_dark_tm, green_dark_tm;
		// 2 other color
		static uint yellow_dark_o, yellow_green_o;
	};
}