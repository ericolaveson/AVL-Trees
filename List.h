/*******************************************************************************
 * PROGRAMMER : ERIC OLAVESON
 * DATE       : 9/10/2015
 ******************************************************************************/
#ifndef LIST_H_
#define LIST_H_

#include <iostream>
using namespace std;

/*******************************************************************************
 * STRUCT Link
 * -----------------------------------------------------------------------------
 * This template struct will contain a type of data according to the data
 * 		type that is determined by the user. This struct is meant to be
 * 		a link in a doubly linked list
 ******************************************************************************/
template <class type>
struct Link
{
	type item;        // CALC  - item contained in each link
	Link<type> *next; // POINT - to next link
	Link<type> *prev; // POINT - to previous link
};

/*******************************************************************************
 * CLASS List
 * -----------------------------------------------------------------------------
 * This class encapsulates methods that manage a doubly linked list of
 * 		Link struct nodes. This is a template class so it's links may
 * 		contain data of any type determined by the user.
 ******************************************************************************/
template<class type>
class List
{
public:

	List();
		// Default Constructor

	List(const List<type>& otherList);
		// Copy Constructor

	~List();
		// Destructor

	bool isEmpty();
		// checks if list is empty

	bool inRange(int x);
		// checks if x is a valid index

	void add(const type& newElement);
		// adds a link to the tail

	void add(int index,const type& newElement);
		// adds a link to middle of list

	void push(const type& newElement);
		// adds a link to the head

	type pop();
		// removes a link from the head

	void remove(int index);
		// removes a node at index

	void print();
		// prints the entire list

	type getNode(int index);
		// returns a copy of a node at index

	Link<type>* get(int index);
		// returns a pointer to a node on the list

	int size();
		// gets the size of the list

	bool contains(type item);
		// checks if an item is already in the list

	type peek();

private:

	int length;       // length of the list
	Link<type> *head; // head of the list
	Link<type> *tail; // tail of the list

	void addIter(int i, int n, Link<type> *element, Link<type> *node);
		// helper method for add middle

	void removeIter(int i, int n, Link<type> *node);
		// helper method for remove

	type getNodeIter(int i, int n, Link<type> *node);
		// helper method for getNode

	Link<type>* getIter(int i, int n, Link<type> *node);
		// helper method for get

	bool containsIter(int i, type item, bool contains, Link<type> *node);
		// helper method for contains

	void printIter(Link<type> *node);
		// helper method for print

	int index(type item);

	int indexIter(Link<type> *node, int index);
};


/*******************************************************************************
 * DEFAULT Constructor
 * -----------------------------------------------------------------------------
 * Initializes list head and tail to null.
 ******************************************************************************/
template<class type>
List <type> :: List()
{
	length = 0;
	head = NULL;
	tail = NULL;
}


/*******************************************************************************
 * DEFAULT Destructor
 * -----------------------------------------------------------------------------
 * Destroys the list!
 ******************************************************************************/
template<class type>
List<type> :: ~List()
{
	while(!isEmpty())
	{
		pop();
	}
}


/*******************************************************************************
 * METHOD IsEmpty
 * -----------------------------------------------------------------------------
 * This method will check if the list is empty.
 ******************************************************************************/
template<class type>
bool List<type> :: isEmpty()
{
	return head == NULL;
}


/*******************************************************************************
 * METHOD inRange
 * -----------------------------------------------------------------------------
 * This method will check to see if the input integer is a valid index.
 ******************************************************************************/
template<class type>
bool List<type> :: inRange(int x)
{
	return (x >= 0 && x < length);
}


/*******************************************************************************
 * METHOD add
 * -----------------------------------------------------------------------------
 * This method will add a link to the tail of the list.
 ******************************************************************************/
template<class type>
void List<type> :: add(const type& newElement)
{
	Link<type> *node;

	// INITIALIZE
	node = new Link<type>;

	// IF - fails if there is no available memory
	if(node != NULL)
	{
		// INITIALIZE - new link
		node->item = newElement;
		node->next = NULL;

		// IF - adds a new node to an empty list
		if (tail == NULL)
		{
			tail = node;
			head = node;
			node->prev = NULL;
		}
		else
		{
			tail->next = node;
			node->prev = tail;
			tail = node;
		}

		// REINITIALIZE
		node = NULL;

		// COUNT - add 1 to list size
		length++;
	}
	else
	{
		cout << "\n**** You are out of memory!****\n";
	}
}


/**************************************************************************
 * METHOD add
 * ------------------------------------------------------------------------
 * This method will add a link to the specified index of the list
 *************************************************************************/
template <class type>
void List<type> :: add(int index,const type& newElement)
{
	Link<type> *element; // ITERATOR - traverses the list

	// INITIALIZE
	element = new Link<type>;

	// IF - fails if there is no available memory
	if(element != NULL)
	{
		// INITIALIZE - new link
		element->item = newElement;

		// IF - the index is within the bounds of the array, add it
		if(index <= length && index >= 0)
		{
			if(index == 0)
			{
				element->prev = NULL;
				element->next = head;

				// add to head
				if(isEmpty())
				{
					tail = element;
				}
				else
				{
					head->prev = element;
				}

				head = element;
			}
			else if(index == length)
			{
				// add to tail
				tail->next = element;
				element->prev = tail;
				tail = element;
			}
			else
			{
				// add to middle
				addIter(0,index,element, head);
			}

			length++;
		}
		else
		{
			delete element;
			cout << endl << "**** Index out of bounds ****" << endl;
		}
	}
	else
	{
		cout << endl << "**** You are out of memory ****" << endl;
	}
}


/**************************************************************************
 * METHOD addIter
 * ------------------------------------------------------------------------
 * This is an iterative helper method for add middle.
 *************************************************************************/
template<class type>
void List<type> :: addIter(int i, int n, Link<type> *element, Link<type> *node)
{
	if(i != n)
	{
		addIter(i + 1, n, element, node->next);
	}
	else
	{
		// add element to middle of the list
		//
		// this will push the node at the current index to the next index
		node->prev->next = element;
		element->prev = node->prev;
		node->prev = element;
		element->next = node;
	}
}


/*******************************************************************************
 * METHOD push
 * -----------------------------------------------------------------------------
 * This method will add a link to the head of the list.
 ******************************************************************************/
template <class type>
void List<type> :: push(const type& newElement)
{
	Link<type> *node; // ITERATOR - traverses the list

	// INITIALIZE
	node = new Link<type>;

	// IF - fails if there is no available memory
	if(node != NULL)
	{
		// INITIALIZE - new link
		node->item = newElement;
		node->prev = NULL;

		// IF - adds a new node to head of list
		if (head == NULL)
		{
			head = node;
			tail = node;
			node->next = NULL;
		}
		else
		{
			head->prev = node;
			node->next = head;
			head = node;
		}

		// REINITIALIZE
		node = NULL;

		// COUNT - add 1 to list size
		length++;
	}
	else
	{
		cout << "\n**** You are out of memory!****\n";
	}
}


/*******************************************************************************
 * METHOD pop
 * -----------------------------------------------------------------------------
 * This method will remove a link from the head of the list.
 ******************************************************************************/
template<class type>
type List<type> :: pop()
{
	// IF - fails if list is empty
	if (!isEmpty())
	{
		Link<type> *popped = head;

		// IF - check to see if list is of length 1
		if (tail->prev == NULL)
		{
			tail = NULL;
			head = NULL;
		}
		else
		{
			head = head->next;
			head->prev->next = NULL;
			head->prev = NULL;
		}

		// COUNT - subtract 1 to list size
		length--;
		return popped->item;
	}
	else
	{
		cout << "\n**** You cannot remove from an empty list ****\n";
		return NULL;
	}
}


/*******************************************************************************
 * METHOD remove
 * -----------------------------------------------------------------------------
 * This method will remove a link from the list according to which
 * index the user has entered.
 ******************************************************************************/
template<class type>
void List<type> :: remove(int index)
{
	if(inRange(index))
	{
		removeIter(0, index, head);
	}
	else
	{
		cout << "\n****The node you want to delete is out of "
						"bounds.****\n";
	}
}


/*******************************************************************************
 * METHOD removeIter
 * -----------------------------------------------------------------------------
 * This is a helper method for remove.
 ******************************************************************************/
template<class type>
void List<type> :: removeIter(int i, int n, Link<type> *node)
{
	if(i < n)
	{
		getIter(i+1,n,node->next);
	}
	else
	{
		if(node->prev != NULL)
		{
			if(node->next != NULL)
			{
				// Remove link not on head or tail
				node->prev->next = node->next;
				node->next->prev = node->prev;
				node->prev = NULL;
				node->next = NULL;
			}
			else
			{
				// Remove link on the tail
				tail = node->prev;
				tail->next = NULL;
				node->prev = NULL;
			}
		}
		else
		{
			if(node->next != NULL)
			{
				// Remove link on head
				head = node->next;
				head->prev = NULL;
				node->next = NULL;
			}
			else
			{
				// Remove last node
				head = NULL;
				tail = NULL;
			}
		}

		delete node;
		node = NULL;

		// COUNT - subtract 1 to list size
		length--;
	}
}


/*******************************************************************************
 * METHOD getNode
 * -----------------------------------------------------------------------------
 * This method will return a node at the specified index in the list.
 ******************************************************************************/
template<class type>
type List<type> :: getNode(int index)
{
	if(inRange(index))
	{
		return getNodeIter(0, index, head);
	}
	else
	{
		cout << "\n**** The node you wish to see is out of bounds ****\n";
		type x;
		return x;
	}
}


/*******************************************************************************
 * METHOD getNodeIter
 * -----------------------------------------------------------------------------
 * Helper method for get.
 ******************************************************************************/
template<class type>
type List<type> :: getNodeIter(int i, int n, Link<type> *node)
{
	if(i < n)
	{
		return getNodeIter(i+1,n,node->next);
	}
	else
	{
		return node->item;
	}
}


/*******************************************************************************
 * METHOD get
 * -----------------------------------------------------------------------------
 * This method will return a pointer to a node at the specified index in
 * the list.
 ******************************************************************************/
template<class type>
Link<type>* List<type> :: get(int index)
{
	if(inRange(index))
	{
		return getIter(0, index, head);
	}
	else if(index == 0)
	{
		return NULL;
	}
	else
	{
		cout << "\n**** The node you wish to see is out of bounds ****\n";
		return NULL;
	}
}


/*******************************************************************************
 * METHOD getNodeIter
 * -----------------------------------------------------------------------------
 * Helper method for get.
 ******************************************************************************/
template<class type>
Link<type>* List<type> :: getIter(int i, int n, Link<type> *node)
{
	if(i < n)
	{
		return getIter(i+1,n,node->next);
	}
	else
	{
		return node;
	}
}


/*******************************************************************************
 * METHOD contains
 * -----------------------------------------------------------------------------
 * This method checks to see if an item is already contained within the
 * list.
 ******************************************************************************/
template<class type>
bool List<type> :: contains(type item)
{
	return containsIter(0, item, false, head);
}


/*******************************************************************************
 * METHOD containsIter
 * -----------------------------------------------------------------------------
 * Helper method for contains.
 ******************************************************************************/
template<class type>
bool List<type> :: containsIter(int i, type item, bool contains, Link<type> *node)
{
	if(i < length)
	{
		if(node->item == item)
		{
			return true;
		}

		return containsIter(i+1, item, false, node->next);
	}
	else
	{
		return false;
	}
}


template<class type>
type List<type> :: peek()
{
	return isEmpty() ? NULL : head->item;
}


/*******************************************************************************
 * METHOD size
 * -----------------------------------------------------------------------------
 * This method will return the length of the list.
 ******************************************************************************/
template<class type>
int List<type> :: size()
{
	return length;
}


/*******************************************************************************
 * METHOD print
 * -----------------------------------------------------------------------------
 * This method will print all the links of the list.
 ******************************************************************************/
template<class type>
void List<type> :: print()
{
	if(!isEmpty())
	{
		printIter(head);
	}
	else
	{
		cout << endl << "List is Empty" << endl;
	}
}


/*******************************************************************************
 * METHOD printIter
 * -----------------------------------------------------------------------------
 * This is a helper method for print.
 ******************************************************************************/
template<class type>
void List<type> :: printIter(Link<type> *node)
{
	if(node != NULL)
	{
		cout << " " << node->item;
		printIter(node->next);
	}
}


#endif /* LIST_H_ */
