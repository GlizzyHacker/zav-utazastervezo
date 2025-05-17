#include "agent.h"
#include "graph.h"
#include "log.hpp"

#ifndef DEPTH_LIMIT
#define DEPTH_LIMIT 1024
#endif

Agent::Agent(Edge& edge, const Node& start, const Node& target) : Route(Array<Edge*>()), start(&start), target(&target) {
	edges += &edge;
}

Agent::Agent(const Agent& other, Edge* edge) : Route(other.edges), start(other.start), target(other.target) {
	if (edge != NULL) {
		//Lecseréli az utolsó élt az adottra
		edges = Array<Edge*>(edges.getLength() - 1, edges + 0);
		edges += edge;
	}
}

AgentState Agent::step() {
	l() << "Inspecting edge:" << ID(edges.last())<< edges.last()->getName() << ", ";
	for (size_t i = 0; i < edges.getLength() - 1; i++)
	{
		//KÖR
		if (edges[i]->getToNode() == head() || start == head()) {
			return terminated;
		}
	}
	if (head() == target) {
		return arrived;
	}
	else {
		try
		{
			edges += (head()->getEdges()[0]);
		}
		//Zsákutca
		catch (const std::out_of_range&)
		{
			return terminated;
		}
		return moved;
	}
}

const Node* Agent::head() const {
	return ((edges.getLength() == 0) ? start : edges.last()->getToNode());
}

AgentPathfinder::AgentPathfinder(Graph graph, size_t numResult) : agents(Array<Agent*>()), Pathfinder(graph, numResult) {}

Array<Route*> AgentPathfinder::getRoutes(const Node& from, const Node& to, Time startTime) {
	//HIBÁT DOB HA FROM ÉS TO NINCS A GRÁFBAN
	graph.getNode(from.getName());
	graph.getNode(to.getName());
	
	Array<Route*> routes(0);
	Array<Edge*> firstEdges = from.getEdges();
	for (size_t i = 0; i < firstEdges.getLength(); i++)
	{
		//l() << "Creating new agent on:" << firstEdges[i]->getName() << " to:" << firstEdges[i]->getToNode()->getName() << std::endl;
		agents += new Agent(*firstEdges[i], from, to);
		agentsActive += true;
	}

	size_t depth = 1;
	while (activeAgents() != 0) {
		l() << "Depth:" << depth << " Num agents:" << agents.getLength() << std::endl;
		if (depth > DEPTH_LIMIT) {
			throw std::range_error("Depth limit reached");
		}

		//HA MEGVAN ANNYI UT AMMENNYI KELL AKKOR AZ ANNAL HOSSZABAKAT LEHET TOROLNI
		if (routes.getLength() >= numRoutes){
		}

		int numAgents = agents.getLength();
		for (size_t i = 0; i < numAgents; i++)
		{
			//HACK: TÖRLÉS MÚVELET HIÁNYZIK
			if (agentsActive[i] == false) {
				continue;
			}
			l() << "Agent:" << ID(agents[i]) << ", ";
			AgentState state = agents[i]->step();
			switch (state)
			{
			case moved:
				l() << "moved to:" << agents[i]->head()->getName() << std::endl;
				splitAgent(*agents[i]);
				break;
			case arrived:
				l() << "arrived at:" << agents[i]->head()->getName() << std::endl;
				routes += agents[i];
				deleteAgent(i);
				if (routes.getLength() == numRoutes) {
					return routes;
				}
				break;
			case terminated:
				l() << "terminated at:" << agents[i]->head()->getName() << std::endl;
				deleteAgent(i);
				break;
			default:
				break;
			}
		}
		if (agents.getLength() == 0) {
			l() << "No agents remaining" << std::endl;
			break;
		}

		depth++;
	}
	return routes;
}

//HACK: TÖRLÉS MÚVELET HIÁNYZIK
void AgentPathfinder::deleteAgent(size_t i) {
	*(agentsActive + i) = false;
}

void AgentPathfinder::splitAgent(const Agent& agent, size_t startEdge)
{
	Array<Edge*> edges = agent.head()->getEdges();
	Node* oldHead = agent.getEdges()[agent.getEdges().getLength() - 2]->getToNode();
	for (size_t i = startEdge; i < edges.getLength(); i++)
	{
		//l() << "Creating new agent on:" << edges[i]->getName() << " to:" << edges[i]->getToNode()->getName() << std::endl;
		agents += new Agent(agent, edges[i]);
		agentsActive += true;
	}
}

size_t AgentPathfinder::activeAgents() {
	size_t count = 0;
	for (size_t i = 0; i < agents.getLength(); i++)
	{
		if (agentsActive[i] == true) {
			count++;
		}
	}
	return count;
}

AgentPathfinder::~AgentPathfinder() {
	for (size_t i = 0; i < agents.getLength(); i++)
	{
		if (agents[i] != NULL) {
			delete agents[i];
		}
	}
}

