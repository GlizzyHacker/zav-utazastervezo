#include "csvparser.h"
#include "graph.h"

#ifndef CSVGRAPH
#define CSVGRAPH

class CSVNode;

class CSVEdge : public Edge {
public:
	CSVEdge(int weight, Time startTime, const char* name);

	void operator+=(CSVNode& node);
};

class CSVNode : public Node {
public:
	CSVNode(const char* name);

	void operator+=(CSVEdge& edge);
};

class CSVGraph : public Graph {

public:
	CSVGraph(CSVParser& csv);

	void operator+=(CSVNode& node);

	~CSVGraph();
};

CSVLine writeRoute(Route& route);

//CRON
Array<Time> parseTime(const char* timeString);
#endif
