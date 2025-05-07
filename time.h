#include <ostream>
#include <ctime>

#ifndef TIME
#define TIME

//CIKLIKUS IDOT AD MEG, Tehát nem egy adott napra, hanem minden napra adja meg az időpontot
class Time {
	//Arra kell hogy ha az órák túlfolynának
	int day;
	int hour;
	int minute;

	void validate();
public:
	Time();
	
	Time(int hour, int minute);

	void operator+=(int minutes);
	
	void operator-=(int minutes);

	int operator-(const Time& other) const;

	std::ostream& print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const Time& time);
#endif