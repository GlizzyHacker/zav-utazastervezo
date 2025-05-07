#include <stdexcept>
#include "memtrace.h"


#ifndef ARRAY
#define ARRAY

template<typename T>
class Array {
	T* array;
	size_t length;
	size_t capacity;

	void fill(T* arr, size_t len) {
		for (size_t i = 0; i < len; i++)
		{
			array[i] = arr[i];
		}
	}
public:
	Array() : array(nullptr), length(0), capacity(0) {}

	Array(size_t length, T* array = nullptr) : array(array == nullptr ? new T[length] : array), length(length), capacity(length) {}

	Array(const Array& other, size_t cap = 0) : length(other.length), capacity((cap > other.length) ? cap : other.length) {
		array = new T[capacity];
		fill(other.array, length);
	}

	size_t getLength() const {
		return length;
	}

	Array& operator=(const Array& other) {
		length = other.length;
		capacity = other.length;
		array = new T[length];
		fill(other.array, length);
		return *this;
	}

	T operator[](size_t i) const {
		if (i >= length) {
			throw std::out_of_range("array overindexed");
		}
		return array[i];
	}

	T* operator+(size_t i) const {
		if (i >= length) {
			throw std::out_of_range("array overindexed");
		}
		return array + i;
	}

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