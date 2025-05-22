#include <stdexcept>
#include "memtrace.h"


#ifndef ARRAY
#define ARRAY

/*!
 * Egy dinamikusan tarolt tomb
 * @tparam T A tipus amit tarol
 */
template<typename T>
class Array {
	/*!
	 * A dinamikusan foglalt tomb
	 */
	T* array;

	/*!
	 * A tombben tarol elemek szama
	 */
	size_t length;

	/*!
	 * A jelenleg foglalt tomb merete
	 */
	size_t capacity;

	/*!
	 * Betolti array-t a megadott tomb elemeivel
	 * @param arr a tomb amibol betolti az elemeket
	 * @param len a betoltendo elemek szama
	 */
	void fill(const T* arr, size_t len) {
		for (size_t i = 0; i < len; i++)
		{
			array[i] = arr[i];
		}
	}
public:
	Array() : array(nullptr), length(0), capacity(0) {}

	/*!
	 * Letrehoz egy Array-t egy tombbol amit lemasol
	 * @param length a tomb hossza
	 * @param arr a tombre mutato pointer
	 */
	Array(size_t length, const T* arr = nullptr) : array(new T[length]), length(length), capacity(length) {
		if (arr != nullptr) {
			fill(arr, length);
		}
	}

	/*!
	 * Letrehoz egy Array-t egy masikbol, lemasolja az elemeit
	 * @param other a masik Array
	 * @param cap a dinamikusan foglalando tomb merete, ha nincs megadva a masik Array hossza
	 */
	Array(const Array& other, size_t cap = 0) : length(other.length), capacity((cap > other.length) ? cap : other.length) {
		array = new T[capacity];
		fill(other.array, length);
	}

	/*!
	 * @return A tomb elemeinek szama
	 */
	size_t getLength() const {
		return length;
	}

	/*!
	 * @return A tomb utolso eleme
	 * @exception std::out_of_range ha ures a tomb
	 */
	T last() const {
		return operator[](length-1);
	}

	/*!
	 * Lemasolja a jobbertekkent adott Array-t
	 * @param other a masolando Array
	 * @return ez az Array
	 */
	Array& operator=(const Array& other) {
		if (&other == this) {
			return *this;
		}
		length = other.length;
		capacity = other.length;
		if (array != NULL) {
			delete[] array;
		}
		array = new T[length];
		fill(other.array, length);
		return *this;
	}

	/*!
	 * @param i index
	 * @return a tomb i-edik eleme
	 * @exception std::out_of_range ha i nagyobb vagy egyenlo mint a tomb elemeinek szama
	 */
	T operator[](size_t i) const {
		if (i >= length) {
			throw std::out_of_range("array overindexed");
		}
		return array[i];
	}

	/*!
	 * Hasznos ha egy Array<char>-bol c-stringet szeretnel csinalni
	 * @param i index
	 * @return a tomb i-edik elemere mutato pointer
	 * @exception std::out_of_range ha i nagyobb vagy egyenlo mint a tomb elemeinek szama
	 */
	T* operator+(size_t i) const {
		if (i >= length) {
			throw std::out_of_range("array overindexed");
		}
		return array + i;
	}

	/*!
	 * Hozzafuz a tomb vegere egy elemet
	 * ujra foglal ha nem ferne el
	 * @param element a hozzadando elem
	 */
	void operator+=(T element) {
		if (length >= capacity) {
			capacity = 2 * length + 1;
			T* tmp = array;
			array = new T[capacity];
			if (tmp != NULL) {
				fill(tmp, length);
			}
			delete[] tmp;
		}
		array[length] = element;
		length++;
	}

	~Array() {
		delete[] array;
	}
};
#endif // !ARRAY