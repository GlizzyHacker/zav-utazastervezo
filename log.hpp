#include <sstream>
#include <cstring>
#include <string>
#include <iostream>

#ifndef LOGGER
#define LOGGER

//KIIRASOKHOZ SEGEDFUGGVENYEK

/*!
 * Ez a valtozo szabalyozza, hogy a l()-be irt uzenetek kiirodnak-e a standard kimenetre
 */
inline bool writeLog = false;

/*!
 * Ezzel a valtozoval meglehet adni egy masik streamet kiirasnak
 */
inline std::ostream& lstream = std::cout;

/*!
 * Ezt a valtozo nem hasznalando
 * A nem kiirt uzeneteket ide irja es torli
 */
inline std::stringstream dispose;

/*!
 * Hibakereseshez hasznalt uzeneteket ide lehet irni
 * @return egy stream ahova lehet irni
 */
inline std::ostream& l() {
	if(!writeLog ){
		dispose.clear();
		return dispose;
	}
		return lstream;
}

/*!
 * Egy pointerbol csinal egy konnyebben olvashato azonositot
 * @tparam T  a pointer tipusa
 * @param ptr a pointer
 * @return az azonosito
 */
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