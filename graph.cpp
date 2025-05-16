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

Edge::Edge(Node* from, Node* to, int weight, Time startTime, const char* newName) : from(from), to(to), weight(weight), startTime(startTime){
	name = new char[strlen(newName) + 1];
	strcpy(name, newName);
}

Edge::Edge() : from(NULL), to(NULL), weight(0), name(NULL) {}

const char* Edge::getName() const {
	return name;
}

Time Edge::getStartTime() const {
	return startTime;
}

size_t Edge::getWeight() const {
	return weight;
}

size_t Edge::getWeight(Time currentTime) const {
	return weight + (startTime-currentTime);
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
	startTime = other.startTime;
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

Node* Graph::getNode(const char* name) const {
	for (size_t i = 0; i < nodes.getLength(); i++)
	{
		if (strcmp(name, nodes[i]->getName()) == 0) {
			return (nodes[i]);
		}
	}
	throw NotFound();
}

Route::Route(Array<Edge*> edges) : edges(edges) {}

Array<Edge*> Route::getEdges() const {
	return edges;
}

size_t Route::getTotalWeight(Time startTime) const {
	Time time = Time(startTime);
	for (size_t i = 0; i < edges.getLength(); i++)
	{
		 time += edges[i]->getWeight(time);
	}
	return time - startTime;
}