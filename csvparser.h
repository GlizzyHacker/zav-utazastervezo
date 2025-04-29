#ifndef CSVPARSER
#define CSVPARSER

class FormatInvalid : public std::exception {
	virtual const char* what() const throw()
	{
		return "A fájl rosszul van formázva";
	}
};

class CSVLine {
	char** columns;
	int numColumns;
public:
	CSVLine(const char line[]);
	char** getColumns() const;
	int getNumColumns() const;
	bool isEmpty() const;
};

class CSVParser {
	CSVLine* lines;
	int numLines;
public:
	CSVParser(const char* filePath);

	void write(CSVLine);

	CSVLine read();

	CSVParser operator+(CSVParser);
};

#endif // !CSVPARSER
