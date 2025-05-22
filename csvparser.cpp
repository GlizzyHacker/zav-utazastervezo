#include <cstring>
#include <ostream>
#include <sstream>
#include <fstream>
#include "csvparser.h"
#include "memtrace.h"
#include "log.hpp"

FormatInvalid::FormatInvalid(const char file[], size_t line, size_t character) {

	std::stringstream sstream;
	if (file == NULL) {
		file = "";
	}
	sstream << "Helytelen formatumu fajl:" << file << " sor:" << line << "character: " << character;
	whatStr = new char[sstream.str().length()+1];
	whatStr[sstream.str().length()] = 0;
	strcpy(whatStr, sstream.str().c_str());
}

const char* FormatInvalid::what() const throw(){
	return whatStr;
}

FormatInvalid::~FormatInvalid() {
	delete[] whatStr;
}

Array<char> CSVLine::trim(const Array<char>& chars) {
	const char* begin = chars + 0;
	const char* end = chars + (chars.getLength() - 1);
	while (begin != end && (isspace(*begin) || *begin == '"')) {
		begin++;
	}
	while (end != begin + 0 && (isspace(*end) || *end == '"')) {
		end--;
	}

	return Array<char>((size_t)(end - begin + 1), begin);
}

void CSVLine::createColumn(const char* start, size_t len) {
	Array<char> newArr = trim(Array<char>(len, start));
	newArr += 0;
	columns += newArr;
}

//Allapotgep
enum SeparatorState {
	base,
	inQuote,
	end
};

const char* CSVLine::findNextSeparator(const char str[], char sep) {
	SeparatorState state = base;
	const char* ptr = str;
	while ((*ptr != 0) && (state != end)) {
		if (*ptr == sep) {
			if (state == base) {
				state = end;
			}
		}
		else if (*ptr == '\"') {
			if (state == base) {
				state = inQuote;
			}
			else if (state == inQuote) {
				state = base;
			}
		}
		ptr++;
	}
	return state == end ?--ptr : NULL;
}

CSVLine::CSVLine() {}

CSVLine::CSVLine(const CSVLine& other) : columns(other.columns) {}

CSVLine::CSVLine(const char line[], char separator) {
	if (line == NULL) {
		return;
	}

	const char* prevSeparator = line - 1;
	const char* nextSeparator = findNextSeparator(line, separator);
	while (nextSeparator != NULL) {
		size_t lenColumn = nextSeparator - prevSeparator - 1;
		createColumn(prevSeparator + 1, lenColumn);
		prevSeparator = nextSeparator;
		nextSeparator = findNextSeparator(nextSeparator + 1, separator);
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
	columns += Array<char>(strlen(str) + 1, str);
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
	Array<char> lineArray = Array<char>();

	if (file.eof() && next != NULL) {
		file.close();
		return next->readLine();
	}

	if (!beenOpened) {
		openFile();
	}

	int ch = file.get();
	while (!file.eof() && ch != '\n' && ch != EOF) {
		lineArray += (char)ch;
		ch = file.get();
	}
	if ((ch == EOF || file.eof() )&&path != NULL) {
		l() << "End of file:" << path << std::endl;
	}
	lineArray += 0;
	//Uj sztringet kell letrehozni mert a jelenlegit az array kezeli
	char* line = new char[lineArray.getLength()];
	strcpy(line, lineArray + 0);
	return line;
}

//Nem a ctor-ban nyitom meg mert 512 a megnyitott fajlok korlatja
void CSVParser::openFile() {
	file = std::fstream(path, std::ios::in | std::ios::out);

	if (!file) {
		//ha nem létezik a fajl letrehozza
		file.open(path, std::ios::out);
		file.close();
		//ujraprobalkozas
		file = std::fstream(path, std::ios::in | std::ios::out);
		if (!file) {
			throw FormatInvalid(path);
		}
	}

	beenOpened = true;
}

CSVParser::CSVParser(const char* filePath) : next(NULL), beenOpened(false) {
	path = new char[strlen(filePath) + 1];
	strcpy(path, filePath);
}

CSVLine CSVParser::read() {
	const char* lineString = readLine();
	CSVLine line(lineString);
	delete[] lineString;
	return line;
}

void CSVParser::write(CSVLine line) {
	if (!beenOpened) {
		openFile();
	}
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
	if (strcmp(other.path, path) == 0) {
		return;
	}
	if (next == NULL) {
		next = &other;
	}
	else {
		(*next) += other;
	}
}

CSVParser::~CSVParser() {
	delete[] path;
	if (next != NULL) {
		delete next;
	}
	file.close();
}