#include "pathfinder.h"
#include "graph.h"
#include "array.hpp"
#include "sorted_list.hpp"

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
	Agent(Edge& edge, const Node& start, const Node& target, Time startTime);
	
	Agent(const Agent&, Edge* firstEdge = NULL);
	
	AgentState step();

	const Node* head() const;
};

class AgentPathfinder : public Pathfinder {
	//Mindig a lista legelejere kerulnek az uj agentek, hogy gyorsabb legyen
	SortedList<Agent*, alwaysFirst<Agent*>> agents;

	Array<Agent*> agentsToDelete;

	void splitAgent(const Agent&, Time startTime, size_t startEdge = 1); 
	//ITERALAS kozben nem lehet torolni
	void deleteAgent(Agent*);
	
public:
	AgentPathfinder(Graph graph, size_t numRoutes = 3);

	SortedList<Route*> getRoutes(const Node& from, const Node& to, Time starTime);

	~AgentPathfinder();
};
#endif
