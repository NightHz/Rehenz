#pragma once
#include "type.h"
#include "math.h"
#include <functional>

namespace Rehenz
{
	// a function which define tile's color
	typedef std::function<uint(uint tile)> TileShader;

	class Tilemap
	{
	private:
		uint* tiles;

	public:
		static const uint tile_value_no_render;
		static TileShader DefaultTileShader;

		// size info
		const uint width;
		const uint height;
		Vector2 tile_size;

		// render info
		uint bg_color;
		uint edge_color;
		Vector2 render_center;
		TileShader tile_shader;

		Tilemap(uint _width, uint _height);
		Tilemap(const Tilemap& tilemap);
		Tilemap& operator=(const Tilemap&) = delete;
		~Tilemap();

		// access tile with x and y
		uint& operator()(uint x, uint y);

		// access tiles
		uint* GetTiles() { return tiles; }

		// render to image
		uint* Render(uint* image, uint screen_width, uint screen_height);
	};
}