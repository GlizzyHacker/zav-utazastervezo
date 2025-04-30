#include <exception>
#include <fstream>

#ifndef CSVPARSER
#define CSVPARSER

class FormatInvalid : public std::exception {
	virtual const char* what() const throw()
	{
		return "A f�jl rosszul van form�zva";
	}
};

class CSVLine {
	char** columns;
	int numColumns;

	void createColumn(const char* start, size_t len);
public:
	CSVLine();
	CSVLine(const char line[]);
	char** getColumns() const;
	int getNumColumns() const;
	bool isEmpty() const;

	~CSVLine();
};

class CSVParser {
	CSVLine* lines;
	int numLines;

	char* readLine(std::ifstream);
public:
	CSVParser(const char* filePath);

	void write(CSVLine);

	CSVLine read();

	CSVParser operator+(CSVParser);

	~CSVParser();
};

#endif // !CSVPARSER
