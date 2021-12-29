#pragma once
#include <vector>
#include "type.h"

namespace Rehenz
{
	class PathFindingNode
	{
		friend struct PathFindingNodePtrCompare;
		friend std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
		friend std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);
	private:
		uint g, h, f;
		PathFindingNode* prev;
	public:
		virtual size_t GetNeighborNodeCount() = 0;
		virtual PathFindingNode* GetNeighborNode(size_t index) = 0;
		virtual uint GetDistance(size_t index) = 0;
		virtual uint GuessDistance(PathFindingNode* end) = 0;
	};

	std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
	std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);
}