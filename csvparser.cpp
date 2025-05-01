#include <cstring>
#include <ostream>
#include <fstream>
#include <string>
#include "csvparser.h"
#include "memtrace.h"
//HOZZAAD EGY OSZLOPOT A TOMB VEGERE AZT FELTETELEZI VAN HELY
void CSVLine::createColumn(const char* start, size_t len) {
	char* newPtr = new char[len + 1];
	newPtr[len] = 0;
	strncpy(newPtr, start, len);
	columns += newPtr;
}

CSVLine::CSVLine() {}

CSVLine::CSVLine(const char line[]) {
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
	return columns.getLength() == 0;
}

CSVLine::~CSVLine() {
	for (size_t i = 0; i < columns.getLength(); i++)
	{
		delete[] columns[i];
	}
}

Array<char*> CSVLine::getColumns() const {
	return columns;
}

//CSVParser

//BEOLVAS EGY SORT A FAJLBOL CSTRINGKENT
char* CSVParser::readLine() {
	size_t limit = 0;
	size_t len = 0;
	char* line = new char[1];
	line[0] = 0;

	int ch = file.get();
	while (!file.eof() && ch != '\n' && ch != EOF) {
		if (len >= limit) {
			limit = 2 * len + 1;
			char* tmp = new char[limit];
			strcpy(tmp, line);
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

CSVParser::CSVParser(const char* filePath) {
	file = std::ifstream(filePath);
}

CSVLine CSVParser::read() {
	char* lineString = readLine();
	CSVLine line = CSVLine(lineString);
	lines += line;
	return line;
}

CSVParser::~CSVParser() {
	file.close();
}