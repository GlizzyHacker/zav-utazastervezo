#include <sstream>
#include <iostream>
#include <cstdlib>
#include "csvgraph.h"
#include "log.hpp"

CSVEdge::CSVEdge(int weight, Array<Time> startTimes, const char* name) : Edge(NULL, NULL, weight, startTimes, name) {}

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
	size_t line = 1;
	Array<char> lastHeader;
	Array<Time> lastTimes;
	CSVNode* lastNode = NULL;
	Array<char> lastColumn1;
	Array<char> lastColumn2;
	CSVLine newLine = csv.read();
	while (!newLine.isEmpty()) {
		l() << "Processing line:" << line ;
		try
		{
			Array<char> column1 = newLine.getColumns()[0];
			l() << " 1.Column:\"" << column1 + 0 << "\"";
			Array<char> column2 = newLine.getColumns()[1];
			l() << " 2.Column:\"" << column2 + 0 << "\"" << std::endl;
			
			char* endptr;
			int numCol1 = (int)strtol(column1 + 0, &endptr, 10);
			//strtol endptr-be a szam utolso jegye utani karaktert teszi, ha ez nem a lezaro nulla akkor nem az egesz egy szam
			bool isCol1Number = endptr == column1 + (column1.getLength() - 1);

			if ((lastHeader.getLength() == 0) || !isCol1Number) {
				l() << "++New header" << std::endl;
				lastHeader = column1;
				lastNode = NULL;
				try {
					lastTimes = parseTime(column2 + 0);
				}
				catch (const std::exception&)
				{
					l() << std::endl;
					throw(FormatInvalid(csv.getFileName(), line));
				}
			}
			else if (isCol1Number) {
				CSVNode* node = NULL;
				try
				{
					node = (CSVNode*)getNode(column2 + 0, true);
					l() << "==Existing node:" << node->getName() << std::endl;
				}
				catch (const NotFound&)
				{
					node = new CSVNode(column2 + 0);
					operator+=(*node);
					l() << "--New node:" << node->getName() << std::endl;
				}
				if (lastNode != NULL) {
					Array<Time> newTimes;
					for (size_t i = 0; i < lastTimes.getLength(); i++)
					{
						Time time = lastTimes[i];
						time += (size_t) atoi(lastColumn1 + 0);
						newTimes += time;
					}
					CSVEdge* edge = new CSVEdge(numCol1 - atoi(lastColumn1 + 0), newTimes, lastHeader + 0);
					*lastNode += *edge;
					*edge += *node;
					l() << "--New edge:" << ID(edge) << edge->getName() << std::endl;
				}
				lastNode = node;

			}
			lastColumn1 = column1;
			lastColumn2 = column2;
		}
		catch (const std::out_of_range&)
		{
			throw(FormatInvalid(csv.getFileName(), line));
		}

		line++;
		//5 ures sort toleral
		for (size_t i = 0; i < 5; i++)
		{
			newLine = csv.read();
			if (!newLine.isEmpty()) {
				break;
			}
		}
	}
}

Array<size_t> cronValues(Array<char> string, size_t range) {
	Array<size_t> vals;

	if (string.getLength() == 2 && string[0] == '*') {
		for (size_t i = 0; i < range; i++)
		{
			vals += i;
		}
	}
	else {
		//REUSE THE SEPARATION ALGO FROM CSVLINE
		Array<Array<char>> parts = CSVLine(string + 0, ',').getColumns();
		for (size_t i = 0; i < parts.getLength(); i++)
		{
			int number = atoi(parts[i] + 0);
			if ((number > 0) || parts[i][0] == '0') {
				vals += (size_t)number;
			}
			else {
				throw FormatInvalid();
			}
		}
	}

	//TODO: RANGE SUPPORT
	return vals;
}

Array<Time> parseTime(const char* timeString) {
	//REUSE THE SEPARATION ALGO FROM CSVLINE
	Array<Array<char>> parts = CSVLine(timeString, ' ').getColumns();

	Array<size_t> hours = cronValues(parts[0], 24);
	Array<size_t> minutes = cronValues(parts[1], 60);

	Array<Time> result;
	for (size_t i = 0; i < hours.getLength(); i++)
	{
		for (size_t j = 0; j < minutes.getLength(); j++)
		{
			result += Time(hours[i], minutes[j]);
		}
	}
	return result;
}

void CSVGraph::operator+=(CSVNode& node) {
	nodes += &node;
}

CSVGraph::~CSVGraph() {
	for (size_t i = 0; i < nodes.getLength(); i++)
	{
		for (size_t j = 0; j < nodes[i]->getEdges().getLength(); j++)
		{
			delete[] nodes[i]->getEdges()[j];
		}
		delete[] nodes[i];
	}
}

CSVLine writeRoute(Route& route) {
	CSVLine newLine;
	Array<Edge*> edges = route.getEdges();
	newLine += route.getTotalWeight();
	Time time = route.getStartTime();
	for (size_t i = 0; i < edges.getLength(); i++)
	{
		std::stringstream timeStream;
		timeStream << edges[i]->getFirstStartTimeAfter(time);
		time += edges[i]->getWeight(time);
		newLine += timeStream.str().c_str();
		newLine += edges[i]->getName();
		newLine += edges[i]->getWeight();
		newLine += edges[i]->getToNode()->getName();
	}
	return newLine;
}