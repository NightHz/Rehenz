#include "color.h"

namespace Rehenz
{
	Color operator*(Color c1, Color c2)
	{
		return Color(c1.x * c2.x, c1.y * c2.y, c1.z * c2.z, c1.w * c2.w);
	}


	Color Color::white = Color(1, 1, 1);
	Color Color::black = Color(0, 0, 0);
	Color Color::red = Color(1, 0, 0);
	Color Color::green = Color(0, 1, 0);
	Color Color::blue = Color(0, 0, 1);
	Color Color::yellow = Color(1, 1, 0);
	Color Color::magenta = Color(1, 0, 1);
	Color Color::cyan = Color(0, 1, 1);

	Color Color::red_l = Color(242, 129, 128) / 255;
	Color Color::yellow_l = Color(223, 218, 129) / 255;
	Color Color::blue_l = Color(141, 219, 254) / 255;
	Color Color::green_l = Color(149, 224, 129) / 255;
	Color Color::purple_l = Color(141, 133, 253) / 255;
	Color Color::pink_l = Color(231, 162, 244) / 255;
	Color Color::orange_l = Color(255, 178, 125) / 255;

	Color Color::yellow_tm = Color(252, 200, 0) / 255;
	Color Color::yellow_green_tm = Color(123, 141, 66) / 255;
	Color Color::blue_tm = Color(137, 195, 235) / 255;
	Color Color::purple_tm = Color(166, 154, 189) / 255;
	Color Color::orange_tm = Color(240, 131, 0) / 255;
	Color Color::green_dark_tm = Color(0, 85, 46) / 255;
	Color Color::blue_dark_tm = Color(25, 47, 96) / 255;
	Color Color::gray_tm = Color(125, 125, 125) / 255;
	Color Color::pink_tm = Color(238, 187, 203) / 255;
	Color Color::red_dark_tm = Color(108, 44, 47) / 255;
	Color Color::green_tm = Color(168, 201, 127) / 255;
	Color Color::black_tm = Color(43, 43, 43) / 255;

	Color Color::yellow_dark_o = Color(123, 141, 66) / 255;


	uint ColorRGB::white = Rehenz::ColorRGB(255, 255, 255);
	uint ColorRGB::black = Rehenz::ColorRGB(0, 0, 0);
	uint ColorRGB::red = Rehenz::ColorRGB(255, 0, 0);
	uint ColorRGB::green = Rehenz::ColorRGB(0, 255, 0);
	uint ColorRGB::blue = Rehenz::ColorRGB(0, 0, 255);
	uint ColorRGB::yellow = Rehenz::ColorRGB(255, 255, 0);
	uint ColorRGB::magenta = Rehenz::ColorRGB(255, 0, 255);
	uint ColorRGB::cyan = Rehenz::ColorRGB(0, 255, 255);

	uint ColorRGB::red_l = Rehenz::ColorRGB(242, 129, 128);
	uint ColorRGB::yellow_l = Rehenz::ColorRGB(223, 218, 129);
	uint ColorRGB::blue_l = Rehenz::ColorRGB(141, 219, 254);
	uint ColorRGB::green_l = Rehenz::ColorRGB(149, 224, 129);
	uint ColorRGB::purple_l = Rehenz::ColorRGB(141, 133, 253);
	uint ColorRGB::pink_l = Rehenz::ColorRGB(231, 162, 244);
	uint ColorRGB::orange_l = Rehenz::ColorRGB(255, 178, 125);

	uint ColorRGB::yellow_tm = Rehenz::ColorRGB(252, 200, 0);
	uint ColorRGB::yellow_green_tm = Rehenz::ColorRGB(123, 141, 66);
	uint ColorRGB::green_tm = Rehenz::ColorRGB(168, 201, 127);
	uint ColorRGB::blue_tm = Rehenz::ColorRGB(137, 195, 235);
	uint ColorRGB::purple_tm = Rehenz::ColorRGB(166, 154, 189);
	uint ColorRGB::pink_tm = Rehenz::ColorRGB(238, 187, 203);
	uint ColorRGB::orange_tm = Rehenz::ColorRGB(240, 131, 0);
	uint ColorRGB::gray_tm = Rehenz::ColorRGB(125, 125, 125);
	uint ColorRGB::black_tm = Rehenz::ColorRGB(43, 43, 43);
	uint ColorRGB::red_dark_tm = Rehenz::ColorRGB(108, 44, 47);
	uint ColorRGB::blue_dark_tm = Rehenz::ColorRGB(25, 47, 96);
	uint ColorRGB::green_dark_tm = Rehenz::ColorRGB(0, 85, 46);

	uint ColorRGB::yellow_dark_o = Rehenz::ColorRGB(123, 141, 66);
}