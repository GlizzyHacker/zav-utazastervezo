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
	const Node* start;
	const Node* target;
public:
	Agent(Edge& edge, const Node& start, const Node& target);
	
	Agent(const Agent&, Edge* firstEdge = NULL);
	
	AgentState step();

	const Node* head() const;
};

class AgentPathfinder : public Pathfinder {
	Graph graph;
	
	int numResult;

	Array<Agent*> agents;

	void splitAgent(const Agent&, int startEdge = 1);
public:
	AgentPathfinder(Graph graph, int numRoutes = 3);

	Array<Route*> getRoutes(const Node& from, const Node& to, Time starTime);

	~AgentPathfinder();
};
#endif
