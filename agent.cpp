#include "agent.h"
#include "graph.h"
#include "log.hpp"

#ifndef TRANSFER_WEIGHT
#define TRANSFER_MULTIPLIER 1.5
#endif

#ifndef MAX_NUM_ROUTES
#define MAX_NUM_ROUTES 20
#endif


#ifndef DEPTH_LIMIT
#define DEPTH_LIMIT 1024
#endif

Agent::Agent(Edge& edge, const Node& start, const Node& target, Time startTime) : Route(Array<Edge*>(), startTime), start(&start), target(&target) {
	edges += &edge;
}

Agent::Agent(const Agent& other, Edge* edge) : Route(other.edges, other.getStartTime()), start(other.start), target(other.target) {
	if (edge != NULL) {
		//Lecsereli az utolso elt az adottra
		edges = Array<Edge*>(edges.getLength() - 1, edges + 0);
		edges += edge;
	}
}

AgentState Agent::step() {
	l() << "Inspecting edge:" << ID(edges.last()) << edges.last()->getName() << ", ";
	for (size_t i = 0; i < edges.getLength() - 1; i++)
	{
		//K�R
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
		//Zsakutca
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

AgentPathfinder::AgentPathfinder(Graph graph, size_t numResult) : Pathfinder(graph, numResult), agents(SortedList<Agent*, alwaysFirst<Agent*>>()) {}

SortedList<Route*> AgentPathfinder::getRoutes(const Node& from, const Node& to, Time startTime) {
	//HIB�T DOB HA FROM �S TO NINCS A GR�FBAN
	graph.getNode(from.getName());
	graph.getNode(to.getName());

	SortedList<Route*> routes;
	Array<Edge*> firstEdges = from.getEdges();
	for (size_t i = 0; i < firstEdges.getLength(); i++)
	{
		Agent* agentPtr = new Agent(*firstEdges[i], from, to, startTime);
		agents += agentPtr;
		l() << "Creating new agent" << ID(agentPtr) << " on:" << firstEdges[i]->getName() << " to:" << firstEdges[i]->getToNode()->getName() << std::endl;
	}

	size_t depth = 1;
	while (agents.getLength() != 0) {
		//Felgyulemlett torlendo agentek eltavolitasa
		for (size_t i = 0; i < agentsToDelete.getLength(); i++)
		{
			agents.remove(agentsToDelete[i]);
			delete agentsToDelete[i];
			agentsToDelete = Array<Agent*>();
		}
		l() << "Depth:" << depth << " agents (active/total):" << agents.getLength() << "/" << agents.getLength() << std::endl;
		if (depth > DEPTH_LIMIT) {
			throw std::range_error("Depth limit reached");
		}

		for (SortedList<Agent*, alwaysFirst<Agent*>>::Iterator iter = agents.begin(), end = agents.end(); iter != end; iter++)
		{
			//HA A MEGADOTT MAXIMUMNAL TOBB UT VAN AKKOR VEGEZTUNK, HOGY NE FUSSON TUL SOKAIG A PROGRAM
			if (routes.getLength() >= MAX_NUM_ROUTES) {
				return routes;
			}
			//HA MEGVAN ANNYI UT AMMENNYI KELL AKKOR AZ ANNAL HOSSZABAKAT LEHET TOROLNI
			if (routes.getLength() >= numRoutes) {
				bool skipAgent = false;
				for (SortedList<Route*>::Iterator routeIter = routes.begin(), routeEnd = routes.end(); routeIter != routeEnd; routeIter++)
				{
					if ((*routeIter)->getTotalWeight() < (*iter)->getTotalWeight()) {
						deleteAgent(*iter);
						//kihagyuk ezt az agentet
						skipAgent = true;
						break; 
					}
				}
				if (skipAgent) {
					continue;
				}
			}

			//VALAMIERT NEM LEHET SWITCHBEN
			Route* routePtr = NULL;
			Time arrivalTime = startTime;
			arrivalTime += (*iter)->getTotalWeight();
			l() << "Agent:" << ID((*iter)) << ", ";
			AgentState state = (*iter)->step();
			switch (state)
			{
			case moved:
				l() << "moved to:" << (*iter)->head()->getName() << std::endl;
				splitAgent(*(*iter), arrivalTime);
				break;
			case arrived:
				l() << "arrived at:" << (*iter)->head()->getName() << std::endl;
				routePtr = new Agent(**iter);
				routes += routePtr;
				deleteAgent(*iter);
				break;
			case terminated:
				l() << "terminated at:" << (*iter)->head()->getName() << std::endl;
				deleteAgent(*iter);
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

void AgentPathfinder::deleteAgent(Agent* agent) {
	agentsToDelete += agent;
}

void AgentPathfinder::splitAgent(const Agent& agent, Time startTime, size_t startEdge)
{
	Node* oldHead = agent.getEdges()[agent.getEdges().getLength() - 2]->getToNode();
	Array<Node*> distinctNodes;
	Array<Edge*> edges = oldHead->getEdges();
	for (size_t i = startEdge; i < edges.getLength(); i++)
	{
		bool contains = false;
		for (size_t j = 0; j < distinctNodes.getLength(); j++)
		{
			if (edges[i]->getToNode() == distinctNodes[j]) {
				contains = true;
				break;
			}
		}
		if (contains) {
			continue;
		}
		else {
			distinctNodes += edges[i]->getToNode();
		}
		Array<Edge*> edgesToSameNode;
		for (size_t j = startEdge; j < edges.getLength(); j++)
		{
			if (edges[j]->getToNode() == edges[i]->getToNode()) {
				edgesToSameNode += edges[j];
			}
		}

		Edge* bestEdge = edges[i];
		for (size_t j = 0; j < edgesToSameNode.getLength(); j++)
		{
			if ((edgesToSameNode[j]->getWeight(startTime) * (strcmp(edgesToSameNode[j]->getName(), agent.getEdges().last()->getName()) == 0 ? 1 : TRANSFER_MULTIPLIER)) < (bestEdge->getWeight(startTime) * (strcmp(bestEdge->getName(), agent.getEdges().last()->getName()) == 0 ? 1 : TRANSFER_MULTIPLIER))) {
				bestEdge = edgesToSameNode[j];
			}
		}
		Agent* agentPtr = new Agent(agent, bestEdge);
		agents += agentPtr;
		l() << "Creating new agent:" << ID(agentPtr) << " on:" << bestEdge->getName() << " to:" << bestEdge->getToNode()->getName() << std::endl;
	}
}

AgentPathfinder::~AgentPathfinder() {
	for (SortedList<Agent*, alwaysFirst<Agent*>>::Iterator iter = agents.begin(), end = agents.end(); iter != end; iter++)
	{
		delete* iter;
	}
}


