/*******************************************************************************
 * PROGRAMMER : ERIC OLAVESON
 * DATE       : 8/29/2017
 ******************************************************************************/

#ifndef NODE_H_
#define NODE_H_


#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;


/*******************************************************************************
 * CLASS - Node
 * -----------------------------------------------------------------------------
 * This class encapsulates methods for a binary tree Node. The class contains
 * a state variable which is useful for modifying the AVL tree balancing
 * functions.
 ******************************************************************************/
template<class type>
class Node
{
public:

	int  id;
	int  height;
	char path;
	char state;
	type item;

	Node<type> *parent;
	Node<type> *left;
	Node<type> *right;

	Node(int id, type item);
	bool operator <  (Node<type> *other);
	bool operator >  (Node<type> *other);
	bool operator == (Node<type> *other);
	bool leftHeavy();
	bool rightHeavy();
	bool doubleHeavy();
	bool steppedLeft();
	bool steppedRight();
	bool balanced();
	bool hasChildren();
	void detachParent();

	bool isRightChild();
	bool isLeftChild();

private:

};

template<class type>
Node<type> :: Node(int id, type item)
{
	this->id     = id;
	this->item   = item;
	this->path   = '*';
	this->state  = '=';
	this->height = 0;
	this->parent = NULL;
	this->left   = NULL;
	this->right  = NULL;
}

template<class type>
bool Node<type> :: operator < (Node<type> *other)
{
	return this->id < other->id;
}

template<class type>
bool Node<type> :: operator > (Node<type> *other)
{
	return this->id > other->id;
}

template<class type>
bool Node<type> :: operator == (Node<type> *other)
{
	return this->id == other->id;
}

template<class type>
bool Node<type> :: leftHeavy()
{
	return this->state == '<';
}

template<class type>
bool Node<type> :: rightHeavy()
{
	return this->state == '>';
}

template<class type>
bool Node<type> :: doubleHeavy()
{
	return this->state == this->path;
}

template<class type>
bool Node<type> :: balanced()
{
	return this->state == '=';
}

template<class type>
bool Node<type> :: steppedLeft()
{
	return this->path == '<';
}

template<class type>
bool Node<type> :: steppedRight()
{
	return this->path == '>';
}

template<class type>
bool Node<type> :: hasChildren()
{
	return this->left || this->right;
}

template<class type>
void Node<type> :: detachParent()
{
	if (parent)
	{
		if (parent->right && (parent->right->id == id))
			parent->right = NULL;
		else if (parent->left && (parent->left->id == id))
			parent->left = NULL;
		parent = NULL;
	}
}

template<class type>
bool Node<type> :: isLeftChild()
{
	return (parent && (parent->left && (parent->left->id == id)));
}

template<class type>
bool Node<type> :: isRightChild()
{
	return (parent && (parent->right && (parent->right->id == id)));
}


#endif /* NODE_H_ */
