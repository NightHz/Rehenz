#pragma once
#include <vector>
#include "type.h"
#include <set>
#include <string>

namespace Rehenz
{
	class PathFindingNode
	{
		friend struct PathFindingNodePtrCompare;
		friend std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
		friend std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);
		friend class PathFinding;
	private:
		float g, h, f;
		PathFindingNode* prev;
		bool is_open, is_close;

	public:
		PathFindingNode() : g(0), h(0), f(0), prev(nullptr), is_open(false), is_close(false) {}
		~PathFindingNode() {}

		float GetG() const { return g; }
		float GetH() const { return h; }
		float GetF() const { return f; }
		PathFindingNode* GetPrevNode() const { return prev; }

		bool IsOpen() const { return is_open; }
		bool IsClose() const { return is_close; }

		virtual size_t GetNeighborNodeCount() = 0;
		virtual PathFindingNode* GetNeighborNode(size_t index) = 0;
		virtual float GetDistance(size_t index) = 0;
		virtual float GuessDistance(PathFindingNode* end) = 0;
	};

	std::vector<PathFindingNode*> PathFindingDijkstra(PathFindingNode* start, PathFindingNode* end);
	std::vector<PathFindingNode*> PathFindingAStar(PathFindingNode* start, PathFindingNode* end);

	// path finding algorithm which runs as a coroutine
	// don't modify path finding node or run path finding algorithm again
	class PathFinding
	{
	private:
		struct PathFindingNodePtrCompare
		{
			bool operator() (const PathFindingNode* lhs, const PathFindingNode* rhs) const;
		};

		std::set<PathFindingNode*, PathFindingNodePtrCompare> open_set;
		std::set<PathFindingNode*> close_set;
		PathFindingNode* start, * end;

		const std::string mode;
		std::string state;
		float c_g, c_h;

		void ComputeF(PathFindingNode* node);

	public:
		PathFinding(PathFindingNode* _start, PathFindingNode* _end, const std::string _mode = "AStar");
		PathFinding(const PathFinding&) = delete;
		PathFinding& operator=(const PathFinding&) = delete;
		~PathFinding();

		void Start();
		void Next(int step);
		bool IsFinished();
		bool ExistPath();
		std::vector<PathFindingNode*> GetPath();

		PathFindingNode* GetMinOpenNode();
		int SearchCount();
	};
}