#include <cstring>
#include "graph.h"
#include "mytime.h"

Node::Node(Array<Edge*> edges,const char* newName) : edges(edges) {
	name = new char[strlen(newName) + 1];
	strcpy(name, newName);
}

Node::Node() : name(NULL) {}

const char* Node::getName() const {
	return name;
}

Array<Edge*> Node::getEdges() const {
	return edges;
}

Node& Node::operator=(const Node& other) {
	if (&other == this) {
		return *this;
	}
	if (name != NULL) {
		delete[] name;
	}
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	edges = other.edges;
	return *this;
}

Node::~Node() {
	delete[] name;
}

Edge::Edge(Node* from, Node* to, int weight, Array<Time> startTimes, const char* newName) : from(from), to(to), weight(weight), startTimes(startTimes){
	name = new char[strlen(newName) + 1];
	strcpy(name, newName);
}

Edge::Edge() : from(NULL), to(NULL), weight(0), name(NULL) {}

const char* Edge::getName() const {
	return name;
}

Time Edge::getFirstStartTimeAfter(Time after) const {
	Time closest = startTimes[0];
	for (size_t i = 1; i < startTimes.getLength(); i++)
	{
		if (startTimes[i] - after < closest - after) {
			closest = startTimes[i];
		}
	}
	return closest;
}

size_t Edge::getWeight() const {
	return weight;
}

size_t Edge::getWeight(Time currentTime) const {
	int bestWeight  = 0;
	for (size_t i = 0; i < startTimes.getLength(); i++)
	{
		if (bestWeight == 0 || bestWeight > (startTimes[i] - currentTime)) {
			bestWeight = weight + (startTimes[i] - currentTime);
		}
	}
	return bestWeight;
}

Node* Edge::getToNode() const {
	return to;
}

Edge& Edge::operator=(const Edge& other) {
	if (&other == this) {
		return *this;
	}
	if (name != NULL) {
		delete[] name;
	}
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	from = other.from;
	to = other.to;
	weight = other.weight;
	startTimes = other.startTimes;
	return *this;
}

Edge::~Edge() {
	delete[] name;
}

Graph::Graph() : nodes(Array<Node*>()) {}

Graph::Graph(Array<Node*> nodes) : nodes(nodes) {}

Array<Node*> Graph::getNodes() const {
	return nodes;
}

Node* Graph::getNode(const char* name, bool exactMatch) const {
	for (size_t i = 0; i < nodes.getLength(); i++)
	{
		if (exactMatch && strcmp(name, nodes[i]->getName()) == 0) {
			return (nodes[i]);
		}
		else if (!exactMatch && std::string(nodes[i]->getName()).find(name) != std::string::npos) {
			return (nodes[i]);
		}
	}
	throw NotFound();
}

Route::Route(Array<Edge*> edges, Time startTime) : edges(edges), startTime(startTime) {}

Array<Edge*> Route::getEdges() const {
	return edges;
}

Time Route::getStartTime() const {
	return startTime;
}

size_t Route::getTotalWeight() const {
	Time time = Time(edges[0]->getFirstStartTimeAfter(startTime));
	for (size_t i = 0; i < edges.getLength(); i++)
	{
		 time += edges[i]->getWeight(time);
	}
	return time - startTime;
}