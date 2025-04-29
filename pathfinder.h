#include "graph.h"
#ifndef PATHFINDER
#define PATHFINDER
class Pathfinder {
public:
	Pathfinder(Graph graph, int numRoutes = 3);
	virtual Route* getRoutes(Node* from, Node* to, Time starTime) = 0;
};
#endif