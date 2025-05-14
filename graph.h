#include "time.h"
#include "array.hpp"

#ifndef GRAPH
#define GRAPH

class NotFound : public std::exception {};

class Edge;

class Node {
protected:
	Array<Edge*> edges;
	
	char* name;
public:
	Node(Array<Edge*> edges, const char* name);
	
	Node();
	
	const char* getName() const;
	
	Array<Edge*> getEdges() const;

	Node& operator=(const Node&);

	virtual ~Node();
};

class Edge {
protected:
	Node* from;
	
	Node* to;
	
	int weight;

	Time startTime;
	
	char* name;
public:
	Edge(Node* from, Node* to, int weight, Time startTime, const char* name);

	Edge();
	
	const char* getName() const;

	int getWeight(Time currentTime) const;
	int getWeight() const;
	
	Time getStartTime() const;

	Node* getToNode() const;

	Edge& operator=(const Edge&);

	virtual ~Edge();
};

class Graph {
protected:
	Array<Node*> nodes;
public:
	Graph();

	Graph(Array<Node*> nodes);

	Array<Node*> getNodes() const;

	Node* getNode(const char* name) const;
};

class Route {
protected:
	Array<Edge*> edges;
public:
	Route(Array<Edge*> edges);

	Array<Edge*> getEdges() const;
	
	int getTotalWeight(Time startTime) const;
};

#endif
