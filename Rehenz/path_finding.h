#pragma once
#include <vector>

namespace Rehenz
{
	class PathFindingNode
	{
		friend struct PathFindingNodePtrCompare;
		friend std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
		friend std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);
	private:
		int g, h, f;
		PathFindingNode* prev;
	public:
		virtual size_t GetNeighborNodeCount() = 0;
		virtual PathFindingNode* GetNeighborNode(size_t index) = 0;
		virtual unsigned int GetDistance(size_t index) = 0;
		virtual unsigned int GuessDistance(PathFindingNode* end) = 0;
	};

	class TilemapPF : PathFindingNode
	{
	private:
		int x, y;
	public:
	};

	std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
	std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);
}