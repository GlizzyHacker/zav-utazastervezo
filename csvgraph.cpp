#include <sstream>
#include "csvgraph.h"

void CSVEdge::operator+=(CSVNode& node) {
	if (from == NULL) {
		from = &node;
	}
	else {
		to = &node;
	}
}

void CSVNode::operator+=(CSVEdge& edge) {
	edges += &edge;
	edge += *this;
}

CSVGraph::CSVGraph(CSVParser csv) {
	int line = 1;
	CSVLine firstLine = csv.read();
	try
	{
		firstLine.getColumns()[0];
		firstLine.getColumns()[1];
	}
	catch (const std::out_of_range&)
	{
		throw(FormatInvalid(NULL,line));
	}

	line++;
	CSVLine newLine = csv.read();
	while (!newLine.isEmpty()) {
		try
		{
			newLine.getColumns()[0];
			newLine.getColumns()[1];
		}
		catch (const std::out_of_range&)
		{
			throw(FormatInvalid(NULL, line));
		}
		line++;
		CSVLine newLine = csv.read();
	}
}

void CSVGraph::operator+=(CSVNode& node) {
	nodes += &node;
}

CSVLine writeRoute(Route route) {
	CSVLine newLine;
	Array<Edge> edges = route.getEdges();
	newLine += route.getTotalWeight(edges[0].getStartTime());
	for (size_t i = 0; i < edges.getLength(); i++)
	{
		std::stringstream timeStream;
		timeStream << edges[i].getStartTime();
		newLine += timeStream.str().c_str();
		newLine += edges[i].getName();
		newLine += edges[i].getWeight();
		newLine += edges[i].getToNode()->getName();
	}
	return newLine;
}