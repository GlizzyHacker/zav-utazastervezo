#include <exception>
#include <fstream>
#include "array.hpp"

#ifndef CSVPARSER
#define CSVPARSER

/*!
 * Hibas formatum kivetel
 */
class FormatInvalid : public std::exception {
	/*!
	 * A kivetelt leiro c-string
	 */
	char* whatStr;
public:
	/*!
	 * @param file a fajl amiben a hiba van
	 * @param line a sor ahol a hiba van
	 * @param character a karakter ami a hibat okozta
	 */
	FormatInvalid(const char file[] = NULL, size_t line = 0, size_t character = 0);

	virtual const char* what() const throw();

	virtual ~FormatInvalid();
};

/*!
 * Egy .csv fajlban levo sor
 */
class CSVLine {
	/*!
	 * A sor egyes oszlopai
	 */
	Array<Array<char>> columns;

	/*!
	 * Eltavolitja a szokozoket es idezojeleket a szoveg elejerol es vegerol
	 * @param chars a szoveget tartalmazo char Array
	 * @return chars szokozok es idezojelek nelkuli masolata
	 */
	Array<char> trim(const Array<char>& chars);

	/*!
	 * Hozzad egy oszlopot a sorhoz
	 * lezaro 0-at nem feltetelez
	 * @param start az oszlop taralmanak kezdete
	 * @param len az oszlop tartalmanak hossza
	 */
	void createColumn(const char* start, size_t len);

	/*!
	 * Megkeresi a megadott c-stringben a kovetkezo sep karakter helyet
	 * @param str c-string amiben keres
	 * @param sep az elvalaszto amit keres
	 * @return az elvalasztora mutato pointer vagy NULL ha nem talalt
	 */
	const char* findNextSeparator(const char str[], char sep);
public:
	/*!
	 * Letrehoz egy ures sort
	 */
	CSVLine();

	/*!
	 * Egy c-stringbol letrehoz egy sort
	 * @param line c-string
	 * @param separator az oszlop elvalasztasara szolgalo karakter
	 */
	CSVLine(const char line[], char separator = ',');

	CSVLine(const CSVLine&);

	/*!
	 * @return sor oszlopai
	 */
	Array<Array<char>> getColumns() const;

	/*!
	 * @return a sor ures-e
	 */
	bool isEmpty() const;

	CSVLine& operator=(const CSVLine&);

	/*!
	 * hozzad egy c-stringet a sorhoz
	 * @param sor c-string
	 */
	void operator+=(const char* str);

	/*!
	 * hozzad egy szamot a sorhoz
	 * @param num szam
	 */
	void operator+=(size_t num);
};

/*!
 * kiirja a sort a megadott streambe vesszokkel elvalasztva
 * @param os a stream amibe irva lesz
 * @param line a sor
 * @return az ostream
 */
std::ostream& operator<<(std::ostream& os, const CSVLine& line);

/*!
 * Egy vagy tobb .csv fajl beolvasasaert felelos osztaly
 * a beolvasas nem letrehozaskor tortenik hanem folyamatosan ahogy az osztalybol kerik
 */
class CSVParser {
	/*!
	 * A feldolgozott fajl eleresi utja
	 */
	char* path;

	/*!
	 * A file stream
	 */
	std::fstream file;

	/*!
	 * A kovetkezo parserre mutato pointer
	 * ha a jelenglegi kifogy ez kovetkezik
	 */
	CSVParser* next;

	/*!
	 * Igaz ha a fajlt megnyitotta mar valaha az objektum
	 */
	bool beenOpened;

	/*!
	 * Beolvas egy sort c-stringkent a fajlbol
	 * uj sor karakterig vagy fajl vegeig olvas
	 * @return c-string
	 */
	char* readLine();

	/*!
	 * Megnyitja a fajlt
	 * ha nem letezik letrehozza es ujra probalja nyitni
	 * @exception FormatInvalid ha ujraprobalkozas utan sem birja megnyitni a fajlt
	 */
	void openFile();
public:

	/*!
	 * Letrehoz egy parsert egy fajl eleresi utvonalbol
	 * @param filePath fajl eleresi utvonal c-stringként
	 */
	CSVParser(const char* filePath);

	/*!
	 * Ir a .csv fajlba
	 * @param line sor amit ir a fajlba
	 */
	void write(CSVLine line);

	/*!
	 * olvas a .csv fajlbol
	 * @return a sor amit olvas a fajlbol
	 */
	CSVLine read();

	/*!
	 * Megadja a jelenleg nyitott fajl eleresi utjat
	 * @return
	 */
	char* getFileName() const;

	/*!
	 * Kombinal ket parsert
	 * a jobb oldali parsernek dinamikusan taroltnak kell lennie es ez utan a bal oldali parser fogja kezelni
	 * ezutan a jobb oldali parsert nem szabad hasznalni
	 * @param parser
	 */
	void operator+=(CSVParser& parser);

	~CSVParser();
};

#endif // !CSVPARSER
