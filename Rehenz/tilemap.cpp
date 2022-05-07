#include "tilemap.h"
#include "drawer.h"
#include <math.h>

namespace Rehenz
{

	TileShader Tilemap::DefaultTileShader = [](uint tile)->uint
	{
		switch (tile)
		{
		case 1u:
			return Drawer::ColorRGB(255, 255, 255); // white
		case 2u:
			return Drawer::ColorRGB(252, 200, 0); // yellow
		case 3u:
			return Drawer::ColorRGB(123, 141, 66); // green
		case 4u:
			return Drawer::ColorRGB(137, 195, 235); // blue
		case 5u:
			return Drawer::ColorRGB(166, 154, 189); // purple
		case 6u:
			return Drawer::ColorRGB(240, 131, 0u); // orange
		case 7u:
			return Drawer::ColorRGB(0, 85, 46); // dark green
		case 8u:
			return Drawer::ColorRGB(25, 47, 96); // dark blue
		case 9u:
			return Drawer::ColorRGB(125, 125, 125); // gray
		case 10u:
			return Drawer::ColorRGB(238, 187, 203); // pink
		default:
			return Drawer::ColorRGB(108, 44, 47); // dark red
		}
	};

	Tilemap::Tilemap(uint _width, uint _height) : width(_width), height(_height),
		tile_size(10, 10), render_center(_width * 0.5f, _height * 0.5f)
	{
		tiles = new uint[static_cast<size_t>(width) * height]{ 0 };
		bg_color = Drawer::ColorRGB(168, 201, 127); // green
		edge_color = Drawer::ColorRGB(43, 43, 43); // black
		tile_shader = DefaultTileShader;
	}
	
	Tilemap::Tilemap(const Tilemap& tilemap) : width(tilemap.width), height(tilemap.height),
		tile_size(tilemap.tile_size), render_center(tilemap.render_center)
	{
		tiles = new uint[static_cast<size_t>(width) * height]{ 0 };
		bg_color = tilemap.bg_color;
		edge_color = tilemap.edge_color;
		tile_shader = tilemap.tile_shader;
	}

	Tilemap::~Tilemap()
	{
		delete[] tiles;
	}

	uint& Tilemap::operator()(uint x, uint y)
	{
		return tiles[x + y * width];
	}

	uint* Tilemap::Render(uint* image, uint screen_width, uint screen_height)
	{
		int sw = static_cast<int>(screen_width);
		int sh = static_cast<int>(screen_height);
		float sw_half = sw * 0.5f;
		float sh_half = sh * 0.5f;
		Drawer drawer(image, sw, sh);
		drawer.Fill(bg_color);

		Vector2 tiles_for_unit_pixel(1 / tile_size.x, 1 / tile_size.y);
		// compute render region in screen
		int rrx1 = Max(0, static_cast<int>(sw_half - render_center.x * tile_size.x));
		int rry1 = Max(0, static_cast<int>(sh_half - render_center.y * tile_size.y));
		int rrx2 = Min(sw, static_cast<int>(sw_half + (width - render_center.x) * tile_size.x));
		int rry2 = Min(sh, static_cast<int>(sh_half + (height - render_center.y) * tile_size.y));
		// compute covered tiles in tilemap
		int x1 = Max(0, static_cast<int>(render_center.x + (rrx1 - sw_half) * tiles_for_unit_pixel.x));
		int y1 = Max(0, static_cast<int>(render_center.y + (rry1 - sh_half) * tiles_for_unit_pixel.y));
		int x2 = Min(static_cast<int>(width - 1), static_cast<int>(render_center.x + (rrx2 - sw_half) * tiles_for_unit_pixel.x));
		int y2 = Min(static_cast<int>(height - 1), static_cast<int>(render_center.y + (rry2 - sh_half) * tiles_for_unit_pixel.y));
		// loop tiles
		float sx1_tile = sw_half + (x1 - render_center.x) * tile_size.x;
		float sy1_tile = sh_half + (y1 - render_center.y) * tile_size.y;
		float sy_tile = sy1_tile;
		for (int y = y1; y <= y2; y++, sy_tile += tile_size.y)
		{
			float sx_tile = sx1_tile;
			for (int x = x1; x <= x2; x++, sx_tile += tile_size.x)
			{
				// compute screen region of the tile
				int sx1 = static_cast<int>(sx_tile);
				int sy1 = static_cast<int>(sy_tile);
				int sx2 = static_cast<int>(sx_tile + tile_size.x);
				int sy2 = static_cast<int>(sy_tile + tile_size.y);
				// draw tile
				uint tile = tiles[x + y * width];
				//if (tile != 1) continue;
				uint color = tile_shader(tile);
				drawer.Rectangle(Point2I(Max(0, sx1), Max(0, sy1)), Point2I(Min(sw, sx2) - 1, Min(sh, sy2) - 1), color);
				// draw edges
				if (sy1 >= 0 && sy1 < sh)
					drawer.Line(Point2I(Max(0, sx1), sy1), Point2I(Min(sw - 1, sx2), sy1), edge_color);
				if (sy2 >= 0 && sy2 < sh)
					drawer.Line(Point2I(Max(0, sx1), sy2), Point2I(Min(sw - 1, sx2), sy2), edge_color);
				if (sx1 >= 0 && sx1 < sw)
					drawer.Line(Point2I(sx1, Max(0, sy1)), Point2I(sx1, Min(sh - 1, sy2)), edge_color);
				if (sx2 >= 0 && sx2 < sw)
					drawer.Line(Point2I(sx2, Max(0, sy1)), Point2I(sx2, Min(sh - 1, sy2)), edge_color);
			}
		}

		return image;
	}

	TilePF::TilePF()
	{
		x = 0;
		y = 0;
	}

	TilePF::~TilePF()
	{
	}

	size_t TilePF::GetNeighborNodeCount()
	{
		return neighbor.size();
	}

	PathFindingNode* TilePF::GetNeighborNode(size_t index)
	{
		return neighbor[index];
	}

	float TilePF::GetDistance(size_t index)
	{
		return distance[index];
	}

	float TilePF::GuessDistance(PathFindingNode* end)
	{
		TilePF* tile = static_cast<TilePF*>(end);
		int dx = x - tile->x;
		int dy = y - tile->y;
		return 2 * sqrtf(static_cast<float>(dx * dx + dy * dy));
	}

	TilesPF::TilesPF(Tilemap* _tilemap, std::set<uint> wall, bool diag)
		: w(_tilemap->width), h(_tilemap->height), size(_tilemap->width* _tilemap->height)
	{
		tilemap = _tilemap;
		tiles = new TilePF[size];
		for (int i = 0; i < size; i++)
		{
			tiles[i].x = i % w;
			tiles[i].y = i / w;
			if (wall.find(tilemap->tiles[i]) == wall.end())
			{
				std::vector<int> offset;
				if (tiles[i].x != 0)
					offset.push_back(-1);
				if (tiles[i].x != w - 1)
					offset.push_back(+1);
				if (tiles[i].y != 0)
					offset.push_back(-w);
				if (tiles[i].y != h - 1)
					offset.push_back(+w);
				for (int o : offset)
				{
					int i2 = i + o;
					if (i2 >= 0 && i2 < size && wall.find(tilemap->tiles[i2]) == wall.end())
					{
						tiles[i].neighbor.push_back(tiles + i2);
						tiles[i].distance.push_back(1.0f);
					}
				}
				if (diag)
				{
					offset.clear();
					if (tiles[i].x != 0 && tiles[i].y != 0)
						offset.push_back(-1 - w);
					if (tiles[i].x != w - 1 && tiles[i].y != 0)
						offset.push_back(+1 - w);
					if (tiles[i].x != 0 && tiles[i].y != h - 1)
						offset.push_back(-1 + w);
					if (tiles[i].x != w - 1 && tiles[i].y != h - 1)
						offset.push_back(+1 + w);
					for (int o : offset)
					{
						int i2 = i + o;
						if (i2 >= 0 && i2 < size && wall.find(tilemap->tiles[i2]) == wall.end())
						{
							tiles[i].neighbor.push_back(tiles + i2);
							tiles[i].distance.push_back(1.414f);
						}
					}
				}
			}
		}
	}

	TilesPF::~TilesPF()
	{
		delete[] tiles;
	}

	TilePF& TilesPF::operator()(uint x, uint y)
	{
		return tiles[x + y * tilemap->width];
	}
}