#include <stdexcept>
#include "memtrace.h"

#ifndef SORTEDLIST
#define SORTEDLIST

template<typename T>
class alwaysFirst {
public:
	bool operator()(T t1, T t2) { return false; }
};

template<typename T>
class descending {
public:
	bool operator()(T t1, T t2) { return t1 > t2; }
};

template<typename T>
class ascending {
public:
	bool operator()(T t1, T t2) { return t1 < t2; }
};

template<typename T, typename compare = ascending<T>>
class SortedList {

	class ListMember {
	public:
		T element;
		
		ListMember* prev;
		
		ListMember* next;

		ListMember(T element, ListMember* prevP, ListMember* nextP) : element(element), prev(prevP), next(nextP) {
			if (prev != NULL) {
				prev->next = this;
			}
			if (next != NULL) {
				next->prev = this;
			}
		}
	};
	
	compare pred;

	ListMember* list;
	
	size_t length;
public:

	class Iterator {
		ListMember* member;
	public:
		Iterator(ListMember* member) : member(member) {}

		Iterator& operator++(int) {
			member = member->next;
			return *this;
		}

		Iterator& operator--(int) {
			member = member->prev;
			return *this;
		}

		T operator*() const {
			if (member == NULL) {
				throw std::out_of_range("end of list");
			}
			return member->element;
		}

		bool operator ==(const Iterator& iter) const {
			return member == iter.member;
		}

		bool operator !=(const Iterator& iter) const {
			return !operator==(iter);
		}
	};

	SortedList() : list(NULL), length(0) {}

	size_t getLength() const {
		return length;
	}

	void remove(T element) {
		ListMember* ptr = list;
		while (ptr != NULL) {
			if (ptr->element == element) {
				if (ptr->next) {
					ptr->next->prev = ptr->prev;
				}
				else {
					ptr->prev = NULL;
				}
				if (ptr->prev) {
					ptr->prev->next = ptr->next;
				}
				else {
					list = ptr->next;
				}
				delete ptr;
				length--;
				return;
			}
			ptr = ptr->next;
		}
	}

	void removeAt(size_t at) {
		ListMember* ptr = list;
		for (size_t i = 0; i < at; i++) {
			ptr = ptr->next;
		}
		if (ptr->prev) {
			ptr->prev->next = ptr->next;
		}
		else {
			list = ptr->next;
		}
		delete ptr;
		length--;
	}

	Iterator begin() {
		return Iterator(list);
	}

	Iterator end() {
		return Iterator(NULL);
	}

	void operator+=(T element) {
		ListMember* ptr = list;
		if (ptr != NULL) {
			while ((ptr->next != NULL)) {
				if (!pred(ptr->next->element, element)) {
					break;
				}
				ptr = ptr->next;
			}
		}
		ListMember* newPtr = new ListMember(element, ptr, ptr ? ptr->next : NULL);
		if (newPtr->prev == NULL) {
			list = newPtr;
		}
		length++;
	}

	~SortedList() {
		ListMember* ptr = list;
		while (ptr != NULL) {
			ListMember* oldPtr = ptr;
			ptr = ptr->next;
			delete oldPtr;
		}
	}
};
#endif // !SORTEDLIST