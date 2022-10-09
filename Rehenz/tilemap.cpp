#include "tilemap.h"
#include "drawer.h"
#include <math.h>

namespace Rehenz
{
	std::unordered_map<uint, uint> Tilemap::default_tile_colors{
		{0,Drawer::red_dark_tm}, {1,Drawer::white}, {2,Drawer::yellow_tm}, {3,Drawer::green_yellow_tm}, {4,Drawer::blue_tm},
		{5,Drawer::purple_tm}, {6,Drawer::orange_tm}, {7,Drawer::green_dark_tm}, {8,Drawer::blue_dark_tm}, {9,Drawer::gray_tm},
		{10,Drawer::pink_tm} };

	Tilemap::Tilemap(uint _width, uint _height) : width(_width), height(_height),
		tile_size(10, 10), render_center(_width * 0.5f, _height * 0.5f)
	{
		tiles = new uint[static_cast<size_t>(width) * height]{ 0 };
		bg_color = Drawer::green_tm;
		edge_color = Drawer::black_tm;
		edge_width = 1;
		tile_colors = default_tile_colors;
	}
	
	Tilemap::Tilemap(const Tilemap& tilemap) : width(tilemap.width), height(tilemap.height),
		tile_size(tilemap.tile_size), render_center(tilemap.render_center)
	{
		tiles = new uint[static_cast<size_t>(width) * height]{ 0 };
		bg_color = tilemap.bg_color;
		edge_color = tilemap.edge_color;
		tile_colors = tilemap.tile_colors;
	}

	Tilemap::~Tilemap()
	{
		delete[] tiles;
	}

	uint* Tilemap::Render(uint* image, uint screen_width, uint screen_height)
	{
		if (tile_size.x <= 0 || tile_size.y <= 0)
			return image;

		float sw = static_cast<float>(screen_width);
		float sh = static_cast<float>(screen_height);
		float sw_half = sw * 0.5f;
		float sh_half = sh * 0.5f;
		DrawerF drawer(image, static_cast<int>(screen_width), static_cast<int>(screen_height));
		drawer.Fill(bg_color);

		int w = static_cast<int>(width);
		int h = static_cast<int>(height);
		Vector2 tiles_for_half_screen(sw_half / tile_size.x, sh_half / tile_size.y);
		float edge_width_half = edge_width / 2;

		// compute covered tiles in tilemap
		int x1 = Max(0, static_cast<int>(render_center.x - tiles_for_half_screen.x));
		int y1 = Max(0, static_cast<int>(render_center.y - tiles_for_half_screen.y));
		int x2 = Min(w - 1, static_cast<int>(render_center.x + tiles_for_half_screen.x));
		int y2 = Min(h - 1, static_cast<int>(render_center.y + tiles_for_half_screen.y));
		// loop tiles
		for (int y = y1; y <= y2; y++)
		{
			for (int x = x1; x <= x2; x++)
			{
				// compute screen region of the tile
				float sx1 = sw_half + (x - render_center.x) * tile_size.x;
				float sy1 = sh_half + (y - render_center.y) * tile_size.y;
				float sx2 = sx1 + tile_size.x;
				float sy2 = sy1 + tile_size.y;
				// draw tile
				uint tile = tiles[x + y * width];
				uint color = tile_colors[tile];
				drawer.Rectangle(Point2(Clamp(sx1, 0.0f, sw), Clamp(sy1, 0.0f, sh)), Point2(Clamp(sx2, 0.0f, sw), Clamp(sy2, 0.0f, sh)), color);
			}
		}

		// compute render region in screen
		float rrx1 = Max(0.0f, sw_half + (x1 - render_center.x) * tile_size.x - edge_width_half);
		float rry1 = Max(0.0f, sh_half + (y1 - render_center.y) * tile_size.y - edge_width_half);
		float rrx2 = Min(sw, sw_half + (x2 + 1 - render_center.x) * tile_size.x + edge_width_half);
		float rry2 = Min(sh, sh_half + (y2 + 1 - render_center.y) * tile_size.y + edge_width_half);
		// draw edges
		for (int x = x1; x <= x2 + 1; x++)
		{
			float sx = sw_half + (x - render_center.x) * tile_size.x;
			drawer.Rectangle(Point2(Clamp(sx - edge_width_half, 0.0f, sw), rry1), Point2(Clamp(sx + edge_width_half, 0.0f, sw), rry2), edge_color);
		}
		for (int y = y1; y <= y2 + 1; y++)
		{
			float sy = sh_half + (y - render_center.y) * tile_size.y;
			drawer.Rectangle(Point2(rrx1, Clamp(sy - edge_width_half, 0.0f, sh)), Point2(rrx2, Clamp(sy + edge_width_half, 0.0f, sh)), edge_color);
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