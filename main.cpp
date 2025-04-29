#include <cstring>
#include <exception>
#include <stdlib.h>
#include "csvparser.h"
#include "csvgraph.h"
#include "graph.h"
#include "agent.h"
#include "time.h"

#ifndef CPORTA

class MissingParameter : public std::exception {
	virtual const char* what() const throw()
	{
		return "Fontos paraméter hiányzik";
	}
};

bool isArgument(char* input, char arg, const char* argument) {
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
	char* scheduleString = NULL;
	char* startString = NULL;
	char* destinationString = NULL;
	char* timeString = NULL;
	char* numResultString = NULL;
	char* returnString = NULL;

	bool isQuiet = false;
	bool isVerbose = false;
	bool isForced = false;

	for (size_t i = 0; i < argc; i++)
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
		else {
			return 400;
		}
	}

	//HIANYZO ADATOK BEKERESE

	//ADATOK ATALAKITASA
	int numResult = atoi(numResultString);
	Time time = Time(timeString);

	//MENETREND FAJLOK BEOLVASASA
	char** files;
	int fileCount = 0;
	CSVParser csvIn = CSVParser(files[0]);

	//FAJLOK KOMBINALASA EGY NAGY CSV FAJLBA
	for (size_t i = 1; i < fileCount; i++) {
		csvIn = csvIn + CSVParser(files[i]);
	}

	//GRAF LETREHOZASA
	Graph* graph = &CSVGraph(csvIn);

	//GRAF ADATOKAT CSAK A GRAF LETREHOZASA UTAN LEHET ATALAKITANI
	Node* start = graph->getNode(startString);
	Node* destination = graph->getNode(destinationString);

	//UTVONAL KERESES
	Pathfinder* pathfinder = &AgentPathfinder(*graph, numResult);
	Route* routes = pathfinder->getRoutes(start, destination, time);

	//FAJLBA IRAS
	if (returnString != NULL) {
		CSVParser csvOut = CSVParser(returnString);
		Route* current = routes;
		while (routes->next != NULL) {
			csvOut.write(writeRoute(*current));
			current = routes->next;
		}
	}

	//EREDMENY KIIRASA
	if (!isQuiet) {

	}
}
#endif