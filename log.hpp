#include <sstream>
#include <cstring>
#include <string>
#include <iostream>

#ifndef LOGGER
#define LOGGER

inline bool writeLog = false;
inline std::ostream& lstream = std::cout;
inline std::stringstream dispose;

//KIIRÁSOKHOZ SEGÉDFÜGGVÉNYEK
inline std::ostream& l() {
	if(!writeLog ){
		dispose.clear();
		return dispose;
	}
		return lstream;
}

//Egy pointerből csinál egy könnyeben olvasható azonosítót
template<typename T>
std::string ID(T* ptr) {

	std::stringstream sstream;
	sstream << ptr;
	std::string str = sstream.str();
	std::stringstream sstream2;
	sstream2 << '<' << typeid(T).name() << (char)str[strlen(str.c_str()) - 4] << (char)str[strlen(str.c_str()) - 3] << (char)str[strlen(str.c_str()) - 5] << '>';
	return sstream2.str();
}
#endif