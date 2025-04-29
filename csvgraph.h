#include "csvparser.h"
#include "graph.h"

#ifndef CSVGRAPH
#define CSVGRAPH

class CSVNode;

class CSVEdge : public Edge {
public:
	CSVEdge operator+=(CSVNode node);
};

class CSVNode : public Node {
public:
	CSVNode operator+=(CSVEdge edge);
};

class CSVGraph : public Graph {
public:
	CSVGraph(CSVParser csv);
	CSVGraph operator+=(CSVNode node);
};

CSVLine writeRoute(Route route);
#endif
