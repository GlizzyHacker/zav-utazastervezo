#include "graph.h"
#include "time.h"

Node::Node(Array<Edge> edges,const char* name) : edges(edges), name(name) {}

Node::Node() : name(NULL) {}

const char* Node::getName() const {
	return name;
}

Array<Edge> Node::getEdges() const {
	return edges;
}

Edge::Edge(Node* from, Node* to, int weight, Time startTime, const char* name) : from(from), to(to), weight(weight), startTime(startTime), name(name) {}

Edge::Edge() : from(NULL), to(NULL), weight(0), name(NULL) {}

const char* Edge::getName() const {
	return name;
}

Time Edge::getStartTime() {
	return startTime;
}

int Edge::getWeight() const {
	return weight;
}

int Edge::getWeight(Time currentTime) const {
	return weight + (startTime-currentTime);
}

Node* Edge::getToNode() const {
	return to;
}

Graph::Graph() : nodes(Array<Node>()) {}

Graph::Graph(Array<Node> nodes) : nodes(nodes) {}

Array<Node> Graph::getNodes() const {
	return nodes;
}

Node* Graph::getNode(const char* name) const {
	for (size_t i = 0; i < nodes.getLength(); i++)
	{
		if (strcmp(name, nodes[i].getName()) == 0) {
			return nodes + i;
		}
	}
	throw NotFound();
}

Route::Route(Array<Edge> edges) : edges(edges) {}

Array<Edge> Route::getEdges() const {
	return edges;
}

int Route::getTotalWeight(Time startTime) const {
	Time time = Time(startTime);
	for (size_t i = 0; i < edges.getLength(); i++)
	{
		 time += edges[i].getWeight(time);
	}
	return time - startTime;
}