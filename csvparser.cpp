#include <cstring>
#include <ostream>
#include <fstream>
#include <string>
#include "csvparser.h"
#include "memtrace.h"
//HOZZAAD EGY OSZLOPOT A TOMB VEGERE AZT FELTETELEZI VAN HELY
void CSVLine::createColumn(const char* start, size_t len) {
	columns[numColumns] = new char[len + 1];
	columns[numColumns][len] = 0;
	strncpy(columns[numColumns], start, len);
	numColumns++;
}

CSVLine::CSVLine() : numColumns(0), columns(new char*[numColumns]) {}

CSVLine::CSVLine(const char line[]) : columns(line == NULL ? NULL : new char* [strlen(line)]), numColumns(0) {
	if (line == NULL) {
		return;
	}

	const char* prevSeparator = line - 1;
	const char* nextSeparator = strchr(line, ',');
	while (nextSeparator != NULL) {
		size_t lenColumn = nextSeparator - prevSeparator - 1;
		createColumn(prevSeparator + 1, lenColumn);
		prevSeparator = nextSeparator;
		nextSeparator = strchr(nextSeparator + 1, ',');
	}
	if (prevSeparator[1] == 0) {
		return;
	}
	nextSeparator = strchr(line, 0);
	size_t lenColumn = nextSeparator - prevSeparator - 1;
	createColumn(prevSeparator + 1, lenColumn);
}

bool CSVLine::isEmpty() const {
	return numColumns == 0;
}

CSVLine::~CSVLine() {
	for (size_t i = 0; i < numColumns; i++)
	{
		delete[] columns[i];
	}
	delete[] columns;
}

char** CSVLine::getColumns() const {
	return columns;
}

int CSVLine::getNumColumns() const {
	return numColumns;
}

//CSVParser

//BEOLVAS EGY SORT A FAJLBOL CSTRINGKENT
char* CSVParser::readLine(std::ifstream file) {
	size_t limit = 0;
	size_t len = 0;
	char* line = new char[1];
	line[0] = 0;

	int ch = file.get();
	while (file.eof() || ch != '\n') {
		if (len >= limit-1) {
			limit = 2 * len + 1;
			char* tmp = new char[limit];
			delete[] line;
			line = tmp;
		}
		line[len] = ch;
		len++;
		ch = file.get();
	}
	line[len] = 0;
	return line;
}

CSVParser::CSVParser(const char fileName[]) : numLines(1), lines(new CSVLine[numLines]) {
	std::ifstream file = std::ifstream(fileName);
	char* lineString = readLine(file);
	CSVLine line = CSVLine(lineString);
	lines[numLines] = line;
}