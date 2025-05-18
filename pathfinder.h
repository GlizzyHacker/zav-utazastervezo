#include "graph.h"
#include "sorted_list.hpp"

#ifndef PATHFINDER
#define PATHFINDER
class Pathfinder {
protected:
	Graph graph;
	size_t numRoutes;
public:
	Pathfinder(Graph graph, size_t numRoutes = 3);
	//HIVO KEZELI A MEMORIAT
	virtual SortedList<Route*> getRoutes(const Node& from,const Node& to, Time starTime) = 0;

	virtual ~Pathfinder();
};
#endif