#include <iostream>

#ifndef TIME
#define TIME

/*!
 * Egy nap ideje perc pontossaggal
 */
class Time {
	/*!
	 * Az orak tulfolyasat taroljak nem egy adott napot ad meg
	 */
	size_t day;

	/*!
	 * A nap oraja
	 */
	size_t hour;

	/*!
	 * Az ora perce
	 */
	size_t minute;

	/*!
	 * Ellenorzi az ora es perc tulfolyasat es korrigalja
	 */
	void validate();
public:
	/*!
	 * Letrehoz egy idot ejfelkor (00:00)
	 */
	Time();

	/*!
	 * Letrehoz egy idot
	 * @param hour az ido oraja
	 * @param minute az ora perce
	 */
	Time(size_t hour, size_t minute);

	/*!
	 * Hozzaad az idohoz valahany percet
	 * @param minutes
	 */
	void operator+=(size_t minutes);

	/*!
	 * Kivon egy idobol valahany percet
	 * @param minutes
	 */
	void operator-=(size_t minutes);

	/*!
	 * Megadja hany percel kesobb lesz a bal oldali ido a jobb oldali hoz kepest
	 * Ha a jobb oldali operator kesobb van mint a bal oldali akkor a kovetkezo nap bal oldali idejehez szamol
	 * @param other a jobb oldali ido
	 * @return A kulonbseg percben
	 */
	size_t operator-(const Time& other) const;

	/*!
	 * Kiirja az idot HH:mm formatumban
	 * @param os a stream amibe irja
	 * @return a stream amibe irta
	 */
	std::ostream& print(std::ostream& os) const;
};

/*!
 * Kiirja az idot HH:mm formatumban
 * @param os a stream amibe irja
 * @return a stream amibe irta
 */
std::ostream& operator<<(std::ostream& os, const Time& time);
#endif