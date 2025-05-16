#include <cstring>
#include <ostream>
#include <sstream>
#include <fstream>
#include "csvparser.h"
#include "memtrace.h"

FormatInvalid::FormatInvalid(const char file[], size_t line, size_t character) : file(file), line(line), character(character) {}

Array<char> CSVLine::trim(const Array<char>& charArray) {
	char* begin = charArray + 0;
	char* end = charArray + (charArray.getLength() - 1);
	while (begin != end && isspace(*begin)) {
		begin++;
	}
	while (end != charArray + 0 && isspace(*end)) {
		end--;
	}

	return Array<char>((double)(end - begin + 1), begin);

}

//HOZZAAD EGY OSZLOPOT A TOMB VEGERE AZT FELTETELEZI VAN HELY
void CSVLine::createColumn(const char* start, size_t len) {
	Array<char> newArr = trim(Array<char>(len, start));
	newArr += 0;
	columns += newArr;
}

CSVLine::CSVLine() {}

//TODO: SUPPORT QUOTES
CSVLine::CSVLine(const char line[], char separator) {
	if (line == NULL) {
		return;
	}

	const char* prevSeparator = line - 1;
	const char* nextSeparator = strchr(line, separator);
	while (nextSeparator != NULL) {
		size_t lenColumn = nextSeparator - prevSeparator - 1;
		createColumn(prevSeparator + 1, lenColumn);
		prevSeparator = nextSeparator;
		nextSeparator = strchr(nextSeparator + 1, separator);
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
	if (&other == this) {
		return *this;
	}
	columns = Array<Array<char>>(other.getColumns());
	return *this;
}

void CSVLine::operator+=(const char* str) {
	columns += Array<char>(strlen(str)+1, str);
}

void CSVLine::operator+=(size_t i) {
	std::stringstream numStream;
	numStream << i;

	operator+=(numStream.str().c_str());
}

std::ostream& operator<<(std::ostream& os, const CSVLine& line) {
	for (size_t i = 0; i < line.getColumns().getLength(); i++)
	{
		if (i != 0) {
			os << ",";
		}
		os << (line.getColumns()[i] + 0);
	}
	return os;
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
		lineArray += (char) ch;
		ch = file.get();
	}
	lineArray += 0;
	//Uj sztringet kell letrehozni mert a jelenlegit az array kezeli
	char* line = new char[lineArray.getLength()];
	strcpy(line, lineArray + 0);
	return line;
}

CSVParser::CSVParser(const char* filePath) : next(NULL) {
	path = new char[strlen(filePath) + 1];
	strcpy(path, filePath);

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
	file << line << std::endl;
}

char* CSVParser::getFileName() const {
	if (file.eof() && next != NULL) {
		return next->getFileName();
	}
	return path;
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
	delete[] path;
	file.close();
}