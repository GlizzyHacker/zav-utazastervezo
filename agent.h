#include "pathfinder.h"
#include "graph.h"
#include "array.hpp";

#ifndef AGENT
#define AGENT

enum AgentState
{
	moved = 0,
	arrived = 1,
	terminated = 2
};

class Agent : public Route {
	Node* start;
	Node* target;
public:
	Agent(Edge& edge, Node& start, Node& target);
	Agent(const Agent&);
	AgentState step();
};

class AgentPathfinder : public Pathfinder {
	Agent* agents;
public:
	AgentPathfinder(Graph graph, int numRoutes = 3);

	Array<Route*> getRoutes(Node* from, Node* to, Time starTime);
};
#endif
