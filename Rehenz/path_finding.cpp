#include "path_finding.h"
#include <set>

namespace Rehenz
{
	struct PathFindingNodePtrCompare
	{
		bool operator() (const PathFindingNode* lhs, const PathFindingNode* rhs) const
		{
			return lhs->f < rhs->f;
		}
	};

	std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end)
	{
		std::set<PathFindingNode*, PathFindingNodePtrCompare> open_set;
		std::set<PathFindingNode*> close_set;
		start->g = 0;
		start->f = start->g;
		start->prev = nullptr;
		open_set.insert(start);
		while (!open_set.empty())
		{
			PathFindingNode* min = *open_set.begin();
			size_t n = min->GetNeighborNodeCount();
			for (size_t i = 0; i < n; i++)
			{
				PathFindingNode* neighbor = min->GetNeighborNode(i);
				if (neighbor == end)
				{
					neighbor->g = min->g + min->GetDistance(i);
					neighbor->f = neighbor->g;
					neighbor->prev = min;
					// find path
					std::vector<PathFindingNode*> path;
					while (end != nullptr)
					{
						path.push_back(end);
						end = end->prev;
					}
					return path;
				}
				else if (close_set.find(neighbor) != close_set.end())
					continue;
				else if (open_set.find(neighbor) != open_set.end())
				{
					uint new_g = min->g + min->GetDistance(i);
					if (neighbor->g > new_g)
					{
						open_set.erase(neighbor);
						neighbor->g = new_g;
						neighbor->f = neighbor->g;
						neighbor->prev = min;
						open_set.insert(neighbor);
					}
				}
				else
				{
					neighbor->g = min->g + min->GetDistance(i);
					neighbor->f = neighbor->g;
					neighbor->prev = min;
					open_set.insert(neighbor);
				}
			}
			open_set.erase(min);
			close_set.insert(min);
		}
		// not find path
		return std::vector<PathFindingNode*>();
	}

	std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end)
	{
		std::set<PathFindingNode*, PathFindingNodePtrCompare> open_set;
		std::set<PathFindingNode*> close_set;
		start->g = 0;
		start->h = start->GuessDistance(end);
		start->f = start->g + start->h;
		start->prev = nullptr;
		open_set.insert(start);
		while (!open_set.empty())
		{
			PathFindingNode* min = *open_set.begin();
			size_t n = min->GetNeighborNodeCount();
			for (size_t i = 0; i < n; i++)
			{
				PathFindingNode* neighbor = min->GetNeighborNode(i);
				if (neighbor == end)
				{
					neighbor->g = min->g + min->GetDistance(i);
					neighbor->h = 0;
					neighbor->f = neighbor->g + neighbor->h;
					neighbor->prev = min;
					// find path
					std::vector<PathFindingNode*> path;
					while (end != nullptr)
					{
						path.push_back(end);
						end = end->prev;
					}
					return path;
				}
				else if (close_set.find(neighbor) != close_set.end())
					continue;
				else if (open_set.find(neighbor) != open_set.end())
				{
					uint new_g = min->g + min->GetDistance(i);
					if (neighbor->g > new_g)
					{
						open_set.erase(neighbor);
						neighbor->g = new_g;
						neighbor->f = neighbor->g + neighbor->h;
						neighbor->prev = min;
						open_set.insert(neighbor);
					}
				}
				else
				{
					neighbor->g = min->g + min->GetDistance(i);
					neighbor->h = neighbor->GuessDistance(end);
					neighbor->f = neighbor->g + neighbor->h;
					neighbor->prev = min;
					open_set.insert(neighbor);
				}
			}
			open_set.erase(min);
			close_set.insert(min);
		}
		// not find path
		return std::vector<PathFindingNode*>();
	}
}