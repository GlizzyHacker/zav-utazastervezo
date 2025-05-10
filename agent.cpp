#include "agent.h"

Agent::Agent(Edge& edge, Node& start, Node& target) : Route(Array<Edge*>()), start(&start), target(&target) {
	edges += &edge;
}

AgentState Agent::step() {
	Edge* lastEdge = edges.last();

	for (size_t i = 0; i < edges.getLength() - 1; i++)
	{
		//KÖR
		if (edges[i]->getToNode() == lastEdge->getToNode() || start == lastEdge->getToNode()) {
			return terminated;
		}
	}
	if (lastEdge->getToNode() == target) {
		return arrived;
	}
	else {
		try
		{
			edges += (lastEdge->getToNode()->getEdges()[0]);
		}
		//Zsákutca
		catch (const std::out_of_range&)
		{
			return terminated;
		}
		return moved;
	}
}

