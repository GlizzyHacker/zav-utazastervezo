//JPORTA GCC miatt
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
#include "memtrace.h"
#include "csvparser.h"
#include "csvgraph.h"
#include "graph.h"
#include "agent.h"
#include "time.h"
#include "array.hpp"

#ifndef CPORTA
#ifndef TESTS

class MissingParameter : public std::exception {
	virtual const char* what() const throw()
	{
		return "Fontos paraméter hiányzik";
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
	const char* startString = NULL;
	const char* destinationString = NULL;
	const char* timeString = NULL;
	const char* numResultString = NULL;
	const char* returnString = NULL;

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
	}

	//ADATOK ATALAKITASA
	fs::path schedulePath(fs::current_path()/"schedules");
	if (scheduleString != NULL) {
		schedulePath = fs::path(scheduleString);
	}

	int numResult = 3;
	if (numResultString != NULL) {
		int numResult = atoi(numResultString);
	}

	Time time;
	if (timeString != NULL) {
		time = parseTime(timeString)[0];
	}

	Array<Array<char>> files;
	//MENETREND FAJLOK BEOLVASASA
	for (fs::directory_entry entry : fs::directory_iterator( schedulePath)) {
		char* arr = (char*)entry.path().c_str();
		files += Array<char>(strlen(arr)+1,arr);

	}
	if (files.getLength() == 0) {
		throw MissingParameter();
	}
	CSVParser csvIn(files[0]+0);

	//FAJLOK KOMBINALASA EGY NAGY CSV FAJLBA
	for (size_t i = 1; i < files.getLength(); i++) {
		CSVParser parser(files[i]+0);
		csvIn += parser;
	}

	//GRAF LETREHOZASA
	CSVGraph csvGraph(csvIn);
	Graph* graph = &csvGraph;

	//GRAF ADATOKAT CSAK A GRAF LETREHOZASA UTAN LEHET ATALAKITANI
	if (startString == NULL) {
		std::string str;
		std::cout << "Kezdeo allomas:";
		std::cin >> str;
		if (str.empty()) {
			throw MissingParameter();
		}
		startString = str.c_str();
	}
	if (destinationString == NULL) {
		std::string str;
		std::cout << "Vegallomas:";
		std::cin >> str;
		if (str.empty()) {
			throw MissingParameter();
		}
		destinationString = str.c_str();
	}
	Node* start = graph->getNode(startString);
	Node* destination = graph->getNode(destinationString);

	//UTVONAL KERESES
	AgentPathfinder agentPathfinder(*graph, numResult);
	Pathfinder* pathfinder = &agentPathfinder;
	Array<Route*> routes = pathfinder->getRoutes(*start, *destination, time);

	//EREDMENY KIIRASA
	if (!isQuiet) {

	}

	//FAJLBA IRAS
	if (returnString != NULL) {
		CSVParser csvOut(returnString);
		for (size_t i = 0; i < routes.getLength(); i++) {
			csvOut.write(writeRoute(*routes[i]));
		}
	}

}
#endif
#endif