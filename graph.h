#include "mytime.h"
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

	size_t weight;
	
	Array<Time> startTimes;
	
	char* name;

public:
	Edge(Node* from, Node* to, int weight, Array<Time> startTimes, const char* name);

	Edge();
	
	const char* getName() const;

	size_t getWeight(Time currentTime) const;

	size_t getWeight() const;
	
	Time getFirstStartTimeAfter(Time time) const;

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

	Node* getNode(const char* name, bool exactMatch = false) const;
};

class Route {
	Time startTime;
protected:
	Array<Edge*> edges;
public:
	Route(Array<Edge*> edges, Time startTime);

	Array<Edge*> getEdges() const;

	Time getStartTime() const;
	
	size_t getTotalWeight() const;
};

#endif
