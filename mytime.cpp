#include <ctime>
#include <iomanip>
#include <cmath>
#include "mytime.h"

Time::Time() : day(0), hour(0), minute(0) {}

Time::Time(size_t hour, size_t minute) : day(0), hour(hour), minute(minute) {
    validate();
}

void Time::validate() {
    size_t wholeHours = (size_t) floor((double)minute / 60.0) ;
    minute -= wholeHours * 60;
    hour += wholeHours;
    size_t wholeDays = (size_t) floor((double)hour / 24.0) ;
    hour -= wholeDays * 24;
    day += wholeDays;
}

void Time::operator+=(size_t minutes) {
    minute += minutes;
    validate();
}

void Time::operator-=(size_t minutes) {
    minute -= minutes;
    validate();
}
std::ostream& Time::print(std::ostream& os) const {
    return os << std::setfill('0') << std::setw(2) << hour << ':' << std::setw(2) << minute;
}

size_t Time::operator-(const Time& other) const {
    size_t minutes = (day * 24 + hour) * 60 + minute;
    size_t otherMinutes = (other.day * 24 + other.hour) * 60 + other.minute;
    //HA A JOBB OLDALI OPERATOR KÉSÖBB VAN MINT A BAL OLDALI AKKOR A KOVETKEZO NAPRA SZAMOLJA KI
    if (otherMinutes > minutes) {
        minutes += 1440*(other.day + 1);
    }
    return minutes - otherMinutes;
}

std::ostream& operator<<(std::ostream& os, const Time& time) {
    return time.print(os);
}