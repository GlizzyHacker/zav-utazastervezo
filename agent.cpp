#include "agent.h"
#include "graph.h";
#include "log.h";

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
	LOG << "Inspecting edge:" << ID(edges.last())<< edges.last()->getName() << ", ";
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

AgentPathfinder::AgentPathfinder(Graph graph, int numResult) : agents(Array<Agent*>()), Pathfinder(graph, numResult) {}

Array<Route*> AgentPathfinder::getRoutes(const Node& from, const Node& to, Time startTime) {
	Array<Edge*> firstEdges = from.getEdges();
	for (size_t i = 0; i < firstEdges.getLength(); i++)
	{
		agents += new Agent(*firstEdges[i], from, to);
	}

	int depth = 1;
	while (true) {
		LOG << "Depth:" << depth << " Num agents:" << agents.getLength() << std::endl;
		if (depth > DEPTH_LIMIT) {
			throw std::range_error("Depth limit reached");
		}

		for (size_t i = 0; i < agents.getLength(); i++)
		{
			//HACK: TÖRLÉS MÚVELET HIÁNYZIK
			if (agents[i] == NULL) {
				continue;
			}
			LOG << "Agent:" << ID(agents[i]) << ", ";
			AgentState state = agents[i]->step();
			switch (state)
			{
			case moved:
				LOG << "moved to:" << agents[i]->head()->getName() << std::endl;
				splitAgent(*agents[i]);
				break;
			case arrived:
				LOG << "arrived at:" << agents[i]->head()->getName() << std::endl;
				return Array<Route*>(1, (Route**)(agents + i));
				break;
			case terminated:
				LOG << "terminated at:" << agents[i]->head()->getName() << std::endl;
				//HACK: TÖRLÉS MÚVELET HIÁNYZIK
				*(agents + i) = NULL;
				break;
			default:
				break;
			}
		}
		if (agents.getLength() == 0) {
			LOG << "No agents remaining" << std::endl;
			break;
		}

		depth++;
	}
}

void AgentPathfinder::splitAgent(const Agent& agent, int startEdge)
{
	Array<Edge*> edges = agent.head()->getEdges();
	Node* oldHead = agent.getEdges()[agent.getEdges().getLength() - 2]->getToNode();
	for (size_t i = startEdge; i < edges.getLength(); i++)
	{
		agents += new Agent(agent, edges[i]);
	}
}

AgentPathfinder::~AgentPathfinder() {
	for (size_t i = 0; i < agents.getLength(); i++)
	{
		delete agents[i];
	}
}

