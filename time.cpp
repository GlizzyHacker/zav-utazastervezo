#include <ctime>
#include <iomanip>
#include <cmath>
#include "time.h"

Time::Time() : day(0), hour(0), minute(0) {}

Time::Time(int hour,int minute) : day(0), hour(hour), minute(minute) {
	validate();
}

void Time::validate() {
	int wholeHours = floor(minute / 60.0) ;
	minute -= wholeHours * 60;
	hour += wholeHours;
	int wholeDays = floor(hour / 24.0) ;
	hour -= wholeDays * 24;
	day += wholeDays;
}

void Time::operator+=(int minutes) {
	minute += minutes;
	validate();
}

void Time::operator-=(int minutes) {
	minute -= minutes;
	validate();
}
std::ostream& Time::print(std::ostream& os) const {
	return os << std::setfill('0') << std::setw(2) << hour << ':' << std::setw(2) << minute;
}

int Time::operator-(const Time& other) const {
	int minutes = (day * 24 + hour) * 60 + minute;
	int otherMinutes = (other.day * 24 + other.hour) * 60 + other.minute;
	//HA A JOBB OLDALI OPERATOR KÉSÖBB VAN MINT A BAL OLDALI AKKOR A KOVETKEZO NAPRA SZAMOLJA KI
	if (otherMinutes > minutes) {
		minutes += 1440*(other.day + 1);
	}
	return minutes - otherMinutes;
}

std::ostream& operator<<(std::ostream& os, const Time& time) {
	return time.print(os);
}