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
	columns += Array<char>(len + 1, newPtr);
}

CSVLine::CSVLine() {}

//TODO: SUPPORT QUOTES
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

Array<Array<char>> CSVLine::getColumns() const {
	return columns;
}

CSVLine& CSVLine::operator=(const CSVLine& other) {
	columns = Array<Array<char>>(other.getColumns());
	return *this;
}

//CSVParser
// 
//BEOLVAS EGY SORT A FAJLBOL CSTRINGKENT
char* CSVParser::readLine() {
	size_t limit = 0;
	size_t len = 0;
	Array<char> lineArray = Array<char>();
	
	if (file.eof() && next != NULL) {
		return next->readLine();
	}

	int ch = file.get();
	while (!file.eof() && ch != '\n' && ch != EOF) {
		lineArray += ch;
		ch = file.get();
	}
	lineArray += 0;
	//Uj sztringet kell letrehozni mert a jelenlegit az array kezeli
	char* line = new char[lineArray.getLength()];
	strcpy(line, lineArray + 0);
	return line;
}

CSVParser::CSVParser(const char* filePath) : next(NULL) {

	file = std::fstream(filePath, std::ios::in | std::ios::out);

	if (!file) {
		//ha nem létezik a fájl létrehozza
		file.open(filePath, std::ios::out);
		file.close();
		//ujraprobalkozas
		file = std::fstream(filePath, std::ios::in | std::ios::out);
		if (!file) {
			throw FormatInvalid();
		}
	}
}

CSVLine CSVParser::read() {
	char* lineString = readLine();
	CSVLine line(lineString);
	delete[] lineString;
	lines += line;
	return line;
}

void CSVParser::write(CSVLine line) {
	for (size_t i = 0; i < line.getColumns().getLength(); i++)
	{
		if (i != 0) {
			file << ",";
		}
		file << (line.getColumns()[i] + 0);
	}
	file << std::endl;
}

//Nem a leghatekonyabb megoldas de mukodik
void CSVParser::operator+=(CSVParser& other) {
	if (next == NULL) {
		next = &other;
	}
	else {
		*next += other;
	}
}

CSVParser::~CSVParser() {
		file.close();
}