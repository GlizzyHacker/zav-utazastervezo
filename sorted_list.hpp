#include <stdexcept>
#include "memtrace.h"

#ifndef SORTEDLIST
#define SORTEDLIST

//Functorok a listahoz
template<typename T>
class alwaysFirst {
public:
	bool operator()(T, T) { return false; }
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

/*!
 * Egy dinamikusan tarolt rendezett lancolt lista
 * @tparam T az elemek tipusa == operatort implementalni kell
 * @tparam compare a rendezeshez hasznalt functor
 */
template<typename T, typename compare = ascending<T>>
class SortedList {
	/*!
	 * Egy lancolt lista elem
	 */
	class ListMember {
	public:
		/*!
		 * A lancolt lista ezen elemenek erteke
		 */
		T element;

		/*!
		 * A lista elozo eleme NULL ha ez az elso
		 */
		ListMember* prev;

		/*!
		 * A list kovetkezo elem NULL ha ez az utolso
		 */
		ListMember* next;

		/*!
		 * Letrehoz egy list elemet a ket megadott pointer koze
		 * @param element a lista elem erteke
		 * @param prevP a lista elozo elemenek pointere
		 * @param nextP a lista kovetkezo elemenek pointere
		 */
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
	/*!
	 * A lancolt lista elso elemere mutato pointer
	 */
	ListMember* list;

	/*!
	 * A lancolt lista hossza
	 */
	size_t length;
public:
	/*!
	 * A lancolt lista iteratora
	 * Implementalja az iteratortol elvart muveleteket
	 */
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

	/*!
	 * Letrehoz egy ures listat
	 */
	SortedList() : list(NULL), length(0) {}

	/*!
	 * @return A lista hossza
	 */
	size_t getLength() const {
		return length;
	}

	/*!
	 * Torol egy elemet a listabol, ha tartalmazza
	 * @param element A torolendo ertek
	 */
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

	/*!
	 * Torol egy elemet a listabol
	 * @param at a torlendo elem indexe
	 */
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

	/*!
	 * @return Lista elejen levo iterator
	 */
	Iterator begin() {
		return Iterator(list);
	}

	/*!
	 * @return Lista utolso eleme utan mutato iterator
	 */
	Iterator end() {
		return Iterator(NULL);
	}

	/*!
	 * Hozzaad a listahoz egy elemet
	 * A megadott rendezo functor alapjan jo helyre teszi
	 * @param element A beszurando ertek
	 */
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