#include <iostream>

#ifndef TIME
#define TIME

//CIKLIKUS IDOT AD MEG, Tehát nem egy adott napra, hanem minden napra adja meg az időpontot
class Time {
	//Arra kell hogy ha az órák túlfolynának
	size_t day;
	size_t hour;
	size_t minute;

	void validate();
public:
	Time();

	Time(size_t hour, size_t minute);

	void operator+=(size_t minutes);

	void operator-=(size_t minutes);

	size_t operator-(const Time& other) const;

	std::ostream& print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Time& time);
#endif