#include "time.h"

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
	Edge* edges;
	int numEdges;
	char* name;
public:
	Node(Edge* edges, int numEdges, char* name);
	char* getName() const;
	Edge* getEdges() const;
	int getNumEdges() const;
};

class Edge {
	Node* from;
	Node* to;
	int weight;
	Time startTime;
	char* name;
public:
	Edge(Node* from, Node* to, int weight, Time startTime, char* name);
	char* getName() const;
	int getWeight(Time currentTime) const;
	Node* getToNode() const;

};

class Graph {
private:
	Node* nodes;
	int numNodes;
public:
	Graph(Node* nodes, int numNodes);
	Node* getNodes() const;
	int getNumNodes() const;
	Node* getNode(char* name) const;
};

class Route {
	Edge* edges;
	int numEdges;
public:
	Route(Edge* edges, int numEdges);
	Node* getEdges() const;
	int getNumEdges() const;
	int getTotalWeight() const;
	Route* next;
};

#endif
