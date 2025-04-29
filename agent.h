#include "pathfinder.h"

#ifndef AGENT
#define AGENT

enum AgentState
{
	move = 0,
	arrived = 1,
	terminate = 2
};

class Agent {
public:
	Agent(Node);
	AgentState step();
};

class AgentPathfinder : public Pathfinder {
	Agent* agents;
	int numAgents;
public:
	AgentPathfinder(Graph graph, int numRoutes = 3);

	Route* getRoutes(Node* from, Node* to, Time starTime);
};
#endif
