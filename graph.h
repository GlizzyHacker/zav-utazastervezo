#include "time.h"
#include "array.hpp"

#ifndef GRAPH
#define GRAPH

class NotFound : public std::exception {
	virtual const char* what() const throw()
	{
		return "Nem létezik ilyen állomás";
	}
};

class Edge;

class Node {
	Array<Edge> edges;
	const char* name;
public:
	Node(Array<Edge> edges, const char* name);
	Node();
	const char* getName() const;
	Array<Edge> getEdges() const;
};

class Edge {
protected:
	Node* from;
	Node* to;
	int weight;
	Time startTime;
	const char* name;
public:
	Edge(Node* from, Node* to, int weight, Time startTime, const char* name);
	Edge();
	const char* getName() const;
	int getWeight(Time currentTime) const;
	Node* getToNode() const;

};

class Graph {
	Array<Node> nodes;
public:
	Graph();

	Graph(Array<Node> nodes);

	Array<Node> getNodes() const;

	Node* getNode(const char* name) const;
};

class Route {
	Array<Edge> edges;
public:
	Route(Array<Edge> edges);
	Array<Edge> getEdges() const;
	int getTotalWeight(Time startTime) const;
	Route* next;
};

#endif
