#include <ostream>
#include <sstream>
#include <cstring>
#include <string>

//KIIRÁSOKHOZ SEGÉDFÜGGVÉNYEK

std::ostream& LOG = std::cout;

//Egy pointerből csinál egy könnyeben olvasható azonosítót
template<typename T>
std::string ID(T* ptr) {
	std::stringstream sstream;
	sstream << ptr;
	std::string str = sstream.str();
	std::stringstream sstream2;
	sstream2 << '<' << typeid(T).name() << (char)str[strlen(str.c_str()) - 1] << (char)str[strlen(str.c_str()) - 3] << (char)str[strlen(str.c_str()) - 5] << '>';
	return sstream2.str();
}