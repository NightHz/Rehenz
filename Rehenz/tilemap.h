#pragma once
#include "type.h"
#include "math.h"
#include <functional>
#include "path_finding.h"
#include <vector>

namespace Rehenz
{
	// a function which define tile's color
	typedef std::function<uint(uint tile)> TileShader;

	class Tilemap
	{
	private:
		uint* tiles;

	public:
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
		TilesPF(Tilemap* _tilemap);
		TilesPF(const TilesPF&) = delete;
		TilesPF& operator=(const TilesPF&) = delete;
		~TilesPF();

		TilePF& operator()(uint x, uint y);

		Tilemap* GetTilemap() { return tilemap; }
	};
}