#include "path_finding.h"
#include <set>

namespace Rehenz
{
	struct PathFindingNodePtrCompare
	{
		bool operator() (const PathFindingNode* lhs, const PathFindingNode* rhs) const
		{
			if (lhs->f < rhs->f)
				return true;
			else if (lhs->f > rhs->f)
				return false;
			else
				return lhs < rhs;
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
			if (open_set.find(end) != open_set.end())
			{
				// find path
				std::vector<PathFindingNode*> path;
				while (end != nullptr)
				{
					path.push_back(end);
					end = end->prev;
				}
				return path;
			}
			PathFindingNode* min = *open_set.begin();
			size_t n = min->GetNeighborNodeCount();
			for (size_t i = 0; i < n; i++)
			{
				PathFindingNode* neighbor = min->GetNeighborNode(i);
				if (close_set.find(neighbor) != close_set.end())
					continue;
				else if (open_set.find(neighbor) != open_set.end())
				{
					float new_g = min->g + min->GetDistance(i);
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
			if (open_set.find(end) != open_set.end())
			{
				// find path
				std::vector<PathFindingNode*> path;
				while (end != nullptr)
				{
					path.push_back(end);
					end = end->prev;
				}
				return path;
			}
			PathFindingNode* min = *open_set.begin();
			size_t n = min->GetNeighborNodeCount();
			for (size_t i = 0; i < n; i++)
			{
				PathFindingNode* neighbor = min->GetNeighborNode(i);
				if (close_set.find(neighbor) != close_set.end())
					continue;
				else if (open_set.find(neighbor) != open_set.end())
				{
					float new_g = min->g + min->GetDistance(i);
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

	bool PathFinding::PathFindingNodePtrCompare::operator() (const PathFindingNode* lhs, const PathFindingNode* rhs) const
	{
		if (lhs->f < rhs->f)
			return true;
		else if (lhs->f > rhs->f)
			return false;
		else
			return lhs < rhs;
	}

	void PathFinding::ComputeF(PathFindingNode* node)
	{
		node->f = c_g * node->g + c_h * node->h;
	}

	PathFinding::PathFinding(PathFindingNode* _start, PathFindingNode* _end, const std::string _mode)
		: open_set(), close_set(), mode(_mode), state("Wait")
	{
		start = _start;
		end = _end;

		if (mode == "AStar" || mode == "A" || mode == "F")
			c_g = 1, c_h = 2;
		else if (mode == "Dijkstra" || mode == "D" || mode == "G")
			c_g = 1, c_h = 0;
		else if (mode == "H")
			c_g = 0, c_h = 1;
		else
			c_g = 0, c_h = 0;
	}

	PathFinding::~PathFinding()
	{
	}

	void PathFinding::Start()
	{
		if (state == "Wait")
		{
			start->g = 0;
			start->h = start->GuessDistance(end);
			ComputeF(start);
			start->prev = nullptr;
			open_set.insert(start);
			start->is_open = true;
			start->is_close = false;
			state = "Run";
		}
	}

	void PathFinding::Next(int step)
	{
		if (state == "Run")
		{
			while (!open_set.empty() && step-- > 0)
			{
				if (open_set.find(end) != open_set.end())
				{
					state = "Finish";
					return;
				}
				PathFindingNode* min = *open_set.begin();
				size_t n = min->GetNeighborNodeCount();
				for (size_t i = 0; i < n; i++)
				{
					PathFindingNode* neighbor = min->GetNeighborNode(i);
					if (close_set.find(neighbor) != close_set.end())
						continue;
					else if (open_set.find(neighbor) != open_set.end())
					{
						float new_g = min->g + min->GetDistance(i);
						if (neighbor->g > new_g)
						{
							open_set.erase(neighbor);
							neighbor->g = new_g;
							ComputeF(neighbor);
							neighbor->prev = min;
							open_set.insert(neighbor);
						}
					}
					else
					{
						neighbor->g = min->g + min->GetDistance(i);
						neighbor->h = neighbor->GuessDistance(end);
						ComputeF(neighbor);
						neighbor->prev = min;
						open_set.insert(neighbor);
						neighbor->is_open = true;
						neighbor->is_close = false;
					}
				}
				open_set.erase(min);
				close_set.insert(min);
				min->is_open = false;
				min->is_close = true;
			}
			if (open_set.empty())
				state = "Finish";
		}
	}

	bool PathFinding::IsFinished()
	{
		return state == "Finish";
	}

	bool PathFinding::ExistPath()
	{
		return !open_set.empty();
	}

	std::vector<PathFindingNode*> PathFinding::GetPath()
	{
		std::vector<PathFindingNode*> path;
		PathFindingNode* p = end;
		while (p != nullptr)
		{
			path.push_back(p);
			p = p->prev;
		}
		return path;
	}

	PathFindingNode* PathFinding::GetMinOpenNode()
	{
		if (!open_set.empty())
			return *open_set.begin();
		else
			return nullptr;
	}

	int PathFinding::SearchCount()
	{
		return open_set.size() + close_set.size();
	}

}