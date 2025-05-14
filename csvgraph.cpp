#include <sstream>
#include <iostream>
#include "csvgraph.h"

CSVEdge::CSVEdge(int weight, Time startTime, const char* name) : Edge(NULL, NULL, weight, startTime, name) {}

void CSVEdge::operator+=(CSVNode& node) {
	if (from == NULL) {
		from = &node;
	}
	else {
		to = &node;
	}
}

CSVNode::CSVNode(const char* name) : Node(Array<Edge*>(), name) {}

void CSVNode::operator+=(CSVEdge& edge) {
	edges += &edge;
	edge += *this;
}

CSVGraph::CSVGraph(CSVParser& csv) {
	int line = 1;
	Array<char> lastHeader;
	Time lastTime;
	CSVNode* lastNode = NULL;
	Array<char> lastColumn1;
	Array<char> lastColumn2;
	CSVLine newLine = csv.read();
	while (!newLine.isEmpty()) {
		try
		{
			
			Array<char> column1 = newLine.getColumns()[0];
			Array<char> column2 = newLine.getColumns()[1];
			int col1 = atoi(column1+0);
			if (lastHeader.getLength() == 0) {
				lastHeader = column1;
				lastTime = Time();
			}
			else if (strlen(column1 + 0) == 1 && column1[0] == '0') {
				CSVNode* node = new CSVNode(column2 + 0);
				operator+=(*node);
				if (lastNode != NULL) {
					lastTime += atoi(lastColumn1 + 0);
					CSVEdge* edge = new CSVEdge(col1 - atoi(lastColumn1 + 0), lastTime, lastHeader+0);
					*lastNode += *edge;
					*edge += *node;
				}
				lastNode = node;

			}
			else if (col1 == 0 && lastHeader.getLength() != 0){
				throw(FormatInvalid(csv.getFileName(), line));
			}
			lastColumn1 = column1;
			lastColumn2 = column2;
		}
		catch (const std::out_of_range&)
		{
			throw(FormatInvalid(csv.getFileName(), line));
		}
		line++;
		newLine = csv.read();
	}
}

void CSVGraph::operator+=(CSVNode& node) {
	nodes += &node;
}

CSVGraph::~CSVGraph() {
	for (size_t i = 0; i < nodes.getLength(); i++)
	{
		for (size_t j = 0; j < nodes[j]->getEdges().getLength(); j++)
		{
			delete[] nodes[i]->getEdges()[j];
		}
	delete[] nodes[i];
	}
}

CSVLine writeRoute(Route& route) {
	CSVLine newLine;
	Array<Edge*> edges = route.getEdges();
	newLine += route.getTotalWeight(edges[0]->getStartTime());
	for (size_t i = 0; i < edges.getLength(); i++)
	{
		std::stringstream timeStream;
		timeStream << edges[i]->getStartTime();
		newLine += timeStream.str().c_str();
		newLine += edges[i]->getName();
		newLine += edges[i]->getWeight();
		newLine += edges[i]->getToNode()->getName();
	}
	return newLine;
}