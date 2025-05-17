#include <exception>
#include <fstream>
#include "array.hpp"

#ifndef CSVPARSER
#define CSVPARSER

class FormatInvalid : public std::exception {
	const char* file;
	size_t line;
	size_t character;
public:
	FormatInvalid(const char file[] = NULL, size_t line = 0, size_t character = 0);
	virtual const char* what() const throw();
};

class CSVLine {
	Array<char> trim(const Array<char>&);

	Array<Array<char>> columns;

	void createColumn(const char* start, size_t len);
public:
	CSVLine();
	
	CSVLine(const char line[], char separator = ',');
	
	Array<Array<char>> getColumns() const;
	
	bool isEmpty() const;

	CSVLine& operator=(const CSVLine&);

	void operator+=(const char*);

	void operator+=(size_t);
};

std::ostream& operator<<(std::ostream& os, const CSVLine& line);

class CSVParser {
	Array<CSVLine> lines;
	
	char* path;

	std::fstream file;
	
	//KELL HOGY TOBB FAJLT LEHESSEN KOMBINALNI
	CSVParser* next;

	char* readLine();
public:
	CSVParser(const char* filePath);

	void write(CSVLine);

	CSVLine read();

	char* getFileName() const;

	//Jobb oldali parsert nem szabad hasznalni ezutan
	void operator+=(CSVParser&);

	~CSVParser();
};

#endif // !CSVPARSER
