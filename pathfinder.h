#include "graph.h"
#include "array.hpp"

#ifndef PATHFINDER
#define PATHFINDER
class Pathfinder {
protected:
	Graph graph;
	size_t numRoutes;
public:
	Pathfinder(Graph graph, size_t numRoutes = 3);

	virtual Array<Route*> getRoutes(const Node& from,const Node& to, Time starTime) = 0;

	virtual ~Pathfinder();
};
#endif