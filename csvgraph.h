#include "csvparser.h"
#include "graph.h"

#ifndef CSVGRAPH
#define CSVGRAPH

class CSVNode;

class CSVEdge : public Edge {
public:
	void operator+=(CSVNode& node);
};

class CSVNode : public Node {
	Array<Edge*> edges;
public:
	void operator+=(CSVEdge& edge);
};

class CSVGraph : public Graph {
	Array<Node*> nodes;
public:
	CSVGraph(CSVParser csv);
	void operator+=(CSVNode& node);
};

CSVLine writeRoute(Route route);
#endif
