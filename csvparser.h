#include <exception>
#include <fstream>
#include "array.hpp"

#ifndef CSVPARSER
#define CSVPARSER

class FormatInvalid : public std::exception {
	virtual const char* what() const throw()
	{
		return "A fájl rosszul van formázva";
	}
};

class CSVLine {
	Array<char*> columns;

	void createColumn(const char* start, size_t len);
public:
	CSVLine();
	CSVLine(const char line[]);
	Array<char*> getColumns() const;
	bool isEmpty() const;

	~CSVLine();
};

class CSVParser {
	Array<CSVLine> lines;
	std::ifstream file;

	char* readLine();
public:
	CSVParser(const char* filePath);

	void write(CSVLine);

	CSVLine read();

	CSVParser operator+(CSVParser);

	~CSVParser();
};

#endif // !CSVPARSER
