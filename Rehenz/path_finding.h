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
		float g, h, f;
		PathFindingNode* prev;

	public:
		PathFindingNode() : g(0), h(0), f(0), prev(nullptr) {}
		~PathFindingNode() {}

		virtual size_t GetNeighborNodeCount() = 0;
		virtual PathFindingNode* GetNeighborNode(size_t index) = 0;
		virtual float GetDistance(size_t index) = 0;
		virtual float GuessDistance(PathFindingNode* end) = 0;
	};

	std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
	std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);
}