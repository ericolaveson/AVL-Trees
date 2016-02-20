/*
 * ListIter.h
 *
 *  Created on: Aug 26, 2015
 *      Author: dagobah
 */

#ifndef LISTITER_H_
#define LISTITER_H_

#include "List.h"
#include <iostream>
using namespace std;

template <class type>
class ListIter {

private:
	Link<type>* ptr;

public:

	ListIter(List<type> *list);

	ListIter(List<type>& list);
		// Default constructor

	ListIter(List<type>& list, int index);
		// Construct iterator to certain index

	type get();
		// POST: item of pointer

	Link<type>* next();
		// POST: ptr goes to next link on list

	void prev();
		// POST: ptr goes to previous link on list

};


template <class type>
ListIter<type> :: ListIter(List<type> *list)
{
	ptr = list->get(0);
}


template <class type>
ListIter<type> :: ListIter(List<type>& list) {

	ptr = list.get(0);
}

template <class type>
ListIter<type> :: ListIter(List<type>& list, int index) {

	ptr = list->get(index);
}

template <class type>
type ListIter<type> :: get() {
	return ptr->item;
}

template <class type>
Link<type>* ListIter<type> :: next() {
	ptr = ptr->next;

	return ptr;
}

template <class type>
void ListIter<type> :: prev() {
	ptr = ptr->prev;
}

#endif /* LISTITER_H_ */
