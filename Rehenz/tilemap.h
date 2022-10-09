#pragma once
#include "type.h"
#include "math.h"
#include <functional>
#include "path_finding.h"
#include <vector>

namespace Rehenz
{
	class Tilemap
	{
		friend class TilesPF;
	private:
		uint* tiles;

	public:
		static std::unordered_map<uint, uint> default_tile_colors;

		// size info
		const uint width;
		const uint height;

		// render info
		uint bg_color;
		uint edge_color;
		Vector2 tile_size;
		float edge_width;
		Vector2 render_center;
		std::unordered_map<uint, uint> tile_colors;

		Tilemap(uint _width, uint _height);
		Tilemap(const Tilemap& tilemap);
		Tilemap& operator=(const Tilemap&) = delete;
		~Tilemap();

		// access tile with x and y
		inline uint& operator()(uint x, uint y) { return tiles[x + y * width]; }

		// render to image
		uint* Render(uint* image, uint screen_width, uint screen_height);
	};

	// tile node for path finding
	class TilePF : public PathFindingNode
	{
		friend class TilesPF;
	private:
		int x, y;
		std::vector<TilePF*> neighbor;
		std::vector<float> distance;

		TilePF();
		TilePF(const TilePF&) = delete;
		TilePF& operator=(const TilePF&) = delete;
		~TilePF();

	public:
		int GetX() { return x; }
		int GetY() { return y; }

		size_t GetNeighborNodeCount() override;
		PathFindingNode* GetNeighborNode(size_t index) override;
		float GetDistance(size_t index) override;
		float GuessDistance(PathFindingNode* end) override;
	};

	// tile node array for path finding
	class TilesPF
	{
	private:
		const int w, h, size;
		Tilemap* tilemap;
		TilePF* tiles;

	public:
		TilesPF(Tilemap* _tilemap, std::set<uint> wall = std::set<uint>(), bool diag = true);
		TilesPF(const TilesPF&) = delete;
		TilesPF& operator=(const TilesPF&) = delete;
		~TilesPF();

		TilePF& operator()(uint x, uint y);

		Tilemap* GetTilemap() { return tilemap; }
	};
}