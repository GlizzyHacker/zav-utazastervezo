#if __cpp_lib_filesystem >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif
#include <cstring>
#include <exception>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "memtrace.h"
#include "csvparser.h"
#include "csvgraph.h"
#include "graph.h"
#include "agent.h"
#include "time.h"
#include "array.hpp"
#include "log.hpp"
#ifndef CPORTA
#ifndef TESTS

class MissingParameter : public std::exception {
	char* whatStr;
public:
	MissingParameter(const char* obj) {
		std::stringstream sstream;
		if (obj == NULL) {
			obj = "";
		}
		sstream << "Kotelezo parameter \"" << obj << "\" hianyzik";
		whatStr = new char[sstream.str().length() + 1];
		whatStr[sstream.str().length()] = 0;
		strcpy(whatStr, sstream.str().c_str());
	}

	const char* what() const throw() {
		return whatStr;
	}

	~MissingParameter() {
		delete[] whatStr;
	}
};

bool isArgument(const char* input, const char arg, const char* argument) {
	if (input[0] == '-') {
		if (input[1] == '-') {
			return strcmp(input + 2, argument) == 0;
		}
		return input[1] == arg;
	}
	return false;
}

int main(int argc, char* argv[]) {

	//PARANCSSORI ARGUMENTUMOK FELDOLGOZASA
	const char* scheduleString = NULL;
	std::string startString;
	std::string destinationString;
	std::string timeString;
	const char* numResultString = NULL;
	const char* returnString = NULL;

	bool isQuiet = false;
	bool isVerbose = false;
	bool isForced = false;

	for (int i = 0; i < argc; i++)
	{
		if (isArgument(argv[i], 'F', "schedule")) {
			scheduleString = argv[++i];
			continue;
		}
		else if (isArgument(argv[i], 'S', "start")) {
			startString = argv[++i];
			continue;
		}
		else if (isArgument(argv[i], 'D', "destination")) {
			destinationString = argv[++i];
			continue;
		}
		else if (isArgument(argv[i], 'T', "time")) {
			timeString = argv[++i];
			continue;
		}
		else if (isArgument(argv[i], 'N', "numresult")) {
			numResultString = argv[++i];
			continue;
		}
		else if (isArgument(argv[i], 'R', "return")) {
			returnString = argv[++i];
			continue;
		}
		else if (isArgument(argv[i], 'q', "quiet")) {
			isQuiet = true;
			continue;
		}
		else if (isArgument(argv[i], 'v', "verbose")) {
			isVerbose = true;
			continue;
		}
		else if (isArgument(argv[i], 'f', "force")) {
			isForced = true;
			continue;
		}
	}

	//NEM MEGADOTT ADATOK BEKERESE
	if (timeString.empty() && !isForced) {
		std::cout << "Indulas ideje:";
		std::getline(std::cin, timeString);
	}

	//ADATOK ATALAKITASA
	fs::path schedulePath(fs::current_path() / "schedules");
	if (scheduleString != NULL) {
		schedulePath = fs::path(scheduleString);
	}

	size_t numResult = 3;
	if (numResultString != NULL) {
		numResult = (size_t)atoi(numResultString);
	}

	Time time;
	if (!timeString.empty()) {
		try {
			time = parseTime(timeString.c_str())[0];
		}
		catch (std::exception e) {
			std::cout << "Ido nem jo formatumban van megadva" << std::endl;
		}
	}

	if (isVerbose) {
		writeLog = true;
	}

	//MENETREND FAJLOK BEOLVASASA
	Array<Array<char>> files;
	for (fs::directory_entry entry : fs::directory_iterator(schedulePath)) {
		std::string str = entry.path().string();
		files += Array<char>(str.length() + 1, str.c_str());
	}
	if (files.getLength() == 0) {
		std::cout << MissingParameter("schedule").what() << std::endl;
		return 1;
	}

	std::cout << "Menetrend betoltese " << files.getLength() << " fajlbol" << std::endl;

	//FAJLOK KOMBINALASA EGY NAGY CSV FAJLBA
	CSVParser csvIn(files[0] + 0);
	for (size_t i = 1; i < files.getLength(); i++) {
		try {
			CSVParser* parser = new CSVParser(files[i] + 0);
			csvIn += *parser;
		}
		catch (const FormatInvalid& e) {
			std::cout << "Hiba a " << i << ". fajl olvasasa kozben:" << files[i] + 0 << ":" << e.what() << std::endl;
		}
	}

	//GRAF LETREHOZASA
	Graph* graph;
	try
	{
		CSVGraph csvGraph(csvIn);
		graph = &csvGraph;

		//GRAF ADATOKAT CSAK A GRAF LETREHOZASA UTAN LEHET ATALAKITANI
		if (startString.empty()) {
			if (!isForced) {
				std::cout << "Kezdo allomas:";
				std::getline(std::cin, startString);
			}
			if (startString.empty()) {
				std::cout << MissingParameter("start").what() << std::endl;
				return 1;
			}
		}
		if (destinationString.empty()) {
			if (!isForced) {
				std::cout << "Vegallomas:";
				std::getline(std::cin, destinationString);
			}
			if (destinationString.empty()) {
				std::cout << MissingParameter("destination").what() << std::endl;
				return 1;
			}
		}

		Node* start;
		Node* destination;
		try {
			start = graph->getNode(startString.c_str());
			destination = graph->getNode(destinationString.c_str());
		}
		catch (const NotFound& e) {
			std::cout << "Hiba a kezdo/veg-allomas megallapitasa kozben:" << e.what() << std::endl;
			return 1;
		}
		std::cout << start->getName() << " --> " << destination->getName() << std::endl;

		//UTVONAL KERESES
		std::cout << "Utvonalak keresese" << std::endl;
		AgentPathfinder agentPathfinder(*graph, numResult);
		Pathfinder* pathfinder = &agentPathfinder;
		SortedList<Route*> routes = pathfinder->getRoutes(*start, *destination, time);

		//EREDMENY KIIRASA
		if (!isQuiet) {
			size_t i = 1;
			for (SortedList<Route*>::Iterator routeIter = routes.begin(), routeEnd = routes.end(); routeIter != routeEnd; routeIter++) {
				//Kertnel tobbet nem mutat
				if (i > numResult) {
					break;
				}
				Array<Edge*> edges = (*routeIter)->getEdges();
				Edge* nextEdge = NULL;
				Edge* firstSameEdge = NULL;
				int sameEdgeCount = 1;
				Time arrivalTime = (*routeIter)->getEdges()[0]->getFirstStartTimeAfter((*routeIter)->getStartTime());
				Time departureTime;

				std::cout << i << ". utvonal " << (*routeIter)->getTotalWeight() - (arrivalTime - time) << " perc" << std::endl;
				std::cout << start->getName() << " [" << arrivalTime << "]" << std::endl;
				for (size_t j = 0; j < edges.getLength(); j++)
				{
					if (firstSameEdge == NULL) {
						departureTime = arrivalTime;
						firstSameEdge = edges[j];
					}
					arrivalTime += edges[j]->getWeight(arrivalTime);
					nextEdge = edges.getLength() == j + 1 ? NULL : edges[j + 1];
					int wait = nextEdge ? nextEdge->getFirstStartTimeAfter(arrivalTime) - arrivalTime : 0;
					if (nextEdge && strcmp(nextEdge->getName(), edges[j]->getName()) == 0 && wait == 0) {
						sameEdgeCount++;
					}
					else {
						std::cout << "| " << edges[j]->getName() << std::endl << "| indulas:" << firstSameEdge->getFirstStartTimeAfter(departureTime) << " " << edges[j]->getWeight(firstSameEdge->getFirstStartTimeAfter(arrivalTime)) << " perc " << sameEdgeCount << " megallo" << std::endl << edges[j]->getToNode()->getName() << " [" << arrivalTime << "]";
						if (nextEdge) {
							std::cout << " varjon " << wait << " percet";
						}
						std::cout << std::endl;
						sameEdgeCount = 1;
						firstSameEdge = NULL;
					}
				}
				i++;
			}
		}

		//FAJLBA IRAS
		if (returnString != NULL) {
			CSVParser csvOut(returnString);
			for (SortedList<Route*>::Iterator routeIter = routes.begin(), routeEnd = routes.end(); routeIter != routeEnd; routeIter++) {
				csvOut.write(writeRoute(**routeIter));
			}
		}

		//FELSZABADITAS
		for (SortedList<Route*>::Iterator routeIter = routes.begin(), routeEnd = routes.end(); routeIter != routeEnd; routeIter++) {
			delete* routeIter;
		}
	}
	catch (const FormatInvalid& e)
	{
		std::cout << "Hiba a graf keszitese kozben:" << e.what() << std::endl;
		return 1;
	}
}
#endif
#endif