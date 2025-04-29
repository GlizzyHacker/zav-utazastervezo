#include <ostream>

#ifndef TIME
#define TIME

class Time {
public:
	Time(const char timeString[]);

	Time operator+(int minutes);
	Time operator-(int minutes);
	int operator-(Time other);

	std::ostream& print(std::ostream& os) const;
};

#endif