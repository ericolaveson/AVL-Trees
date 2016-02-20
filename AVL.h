/***********************************************************************************************************************
 * PROGRAMMER : Eric Olaveson
 * DATE       : 8/17/15
 **********************************************************************************************************************/

#ifndef AVL_H_
#define AVL_H_


#include "List.h"
#include "ListIter.h"
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;


/***********************************************************************************************************************
 * STRUCT Node
 * ---------------------------------------------------------------------------------------------------------------------
 * This template struct will contain an AVL tree node.
 **********************************************************************************************************************/
template <class type>
struct Node
{
	int id;          // serial number for node
	int printHeight; // height to print (not used in calculations)
	bool path;       // tells which path was taken to get to a node
	char state;      // balance state of node
	type item;       // data stored in the tree node

	Node<type> *parent; // POINT - to parent tree node
	Node<type> *left;   // POINT - to left tree node
	Node<type> *right;  // POINT - to right tree node
};


/***********************************************************************************************************************
 * CLASS Trifectopod
 * ---------------------------------------------------------------------------------------------------------------------
 * This class encapsulates methods for the self balancing binary tree known as the AVL tree.
 **********************************************************************************************************************/
template<class type>
class AVL
{

public:

	Node<type> *root;

	bool insert(int id, type item);
		// insert node into the tree

	void print(ostream& out);
		// prints the tree level by level


private:

	int height;
	int maxDigits;
	int nodeLength;

	Node<type> *first;
	Node<type> *second;
	Node<type> *third;
	List<List<Node<type> *> *> levels;

	Node<type>* initNode(int id, type item);
		// initializes an AVL node

	bool insertIter(Node<type> *root, Node<type> *node, Node<type> *parent);
		// iterative helper method for insert

	void updatePetiole(Node<type> *node);
		// update state of the added node and its parent node, the petiole

	void updateTree(Node<type> *head);
		// update path taken, searching for a unbalanced node

	void updateTrackingBranch(Node<type> *node);
		// updates the three tracking nodes

	void balance();
		// performs a balancing algorithm on the unbalanced subtree

	void balance00();
		// left-left re-balance

	void balance01();
		// left-right re-balance

	void balance10();
		// right-left re-balance

	void balance11();
		// right-right re-balance

	void connectSubtreeSecond();
		// connects second tracking node to right or left child of parent

	void connectSubtreeThird();
		// connects third tracking node to right or left child of parent
	void replaceDeletedWithSuccessor(Node<type> *deleted);
		// removes a node in a left heavy state


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// These methods print out an epic AVL tree
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void printTree(List<string> *treeString, ostream& out);
		// prints tree level by level

	void inOrder();
		// traversal to collect printing variables

	void inOrder(Node<type> *node, int level);
		// iterative helper method for inOrder()

	void fillNullNodes(Node<type> *node);
		// fills null spaces in the printed version of the avl tree

	List<string>* constructLevels();
		// constructs each level of the tree

	void clean();
		// cleans the print variables

	void cleanLevels();
		// cleans level print variables

	void initLevels();
		// initializes each level's linked list

	void initLevels(Node<type> *root, int level);
		// iterative helper method for initLevels

	void updateMaxDigits(int id);
		// updates the max digits of the print width

	string paddedInt(int n, int width);
		// returns a string of the int padded with zeros to fit maxDigits

	int digits(int n);
		// returns the width of the number

	string pad(char c, int length);
		// returns a padded string of type c in the given length

	string pad(char c, int i, int n, ostringstream *buffer);
		// iterative helper for pad

	string lvlString(int first, int branch, int between, List<Node<type> *> *level);
		// returns the string representation of each level of tree
};


/***********************************************************************************************************************
 * METHOD insert
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will insert a node into the AVL tree. The update algorithm will be called in this function as well to
 * make sure that the tree holds the AVL property.
 **********************************************************************************************************************/
template<class type>
bool AVL<type> :: insert(int id, type item)
{
	Node<type> *node = initNode(id, item);

	if (!root)
	{
		root = node;
		node = NULL;

		return true;
	}
	else
	{
		if (insertIter(root, node, root))
		{
			updatePetiole(node);

			if (node->parent->state != '=' && node->parent->parent)
			{
				// SET - auxiliary nodes
				second = node;
				first  = node->parent;
				updateTree(node->parent->parent);
			}

			// safe measure
			node = NULL;

			return true;
		}
	}

	return false;
}


/***********************************************************************************************************************
 * METHOD initNode
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will initialize an AVL tree node in preparation for an insertion.
 **********************************************************************************************************************/
template<class type>
Node<type>* AVL<type> :: initNode(int id, type item)
{
	Node<type> *node = new Node<type>;

	if (node)
	{
		node->item   = item;
		node->id     = id;
		node->state  = '=';
		node->parent = NULL;
		node->left   = NULL;
		node->right  = NULL;
	}
	else
		cout << "Out of Memory";

	return node;
}


/***********************************************************************************************************************
 * METHOD insertIter
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will use iterative recursion to add a node to the AVL tree.
 **********************************************************************************************************************/
template<class type>
bool AVL<type> :: insertIter(Node<type> *root, Node<type> *node, Node<type> *parent)
{
	// IF - check if tree is empty when adding a new node
	if (root)
	{
		// CALC - perform iterative binary search of tree to add node
		if (root->id < node->id)
		{
			root->path = true;
			return insertIter(root->right, node, root);
		}
		else if (root->id > node->id)
		{
			root->path = false;
			return insertIter(root->left, node, root);
		}
		else
			// node with id already exists
			return false;
	}
	else
	{
		// SET - the parent's child pointer
		if (parent->path)
			parent->right = node;
		else
			parent->left = node;

		// SET - the child's parent pointer
		node->parent = parent;

		return true;
	}
}


/***********************************************************************************************************************
 * METHOD
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will update the state of the leaf as well as the node to which it is attached. We call this second node
 * the petiole.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: updatePetiole(Node<type>* node)
{
	if (node->parent->state == '=')
		node->parent->state = (node->parent->path) ? '>' : '<';
	else
		node->parent->state = '=';
}


/***********************************************************************************************************************
 * METHOD updateTree
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will use iterative recursion to update the branches of the AVL tree.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: updateTree(Node<type> *head)
{
	if (head->path)
	{
		if (head->state == '=')
		{
			head->state = '>';
			updateTrackingBranch(head);

			if(head->parent)
				updateTree(head->parent);
		}
		else if (head->state == '>')
		{
			updateTrackingBranch(head);
			balance();
		}
		else
			head->state = '=';
	}
	else
	{
		if (head->state == '=')
		{
			head->state = '<';
			updateTrackingBranch(head);

			if (head->parent)
				updateTree(head->parent);
		}
		else if (head->state == '<')
		{
			updateTrackingBranch(head);
			balance();
		}
		else
			head->state = '=';
	}
}


/***********************************************************************************************************************
 * METHOD updateTrackingBranch
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will update the first, second, and third tracking nodes which are used in the balancing functions.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: updateTrackingBranch(Node<type> *node)
{
	third  = second;
	second = first;
	first  = node;
}


/***********************************************************************************************************************
 * METHOD balance
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will weigh the kinks of the tracking branch to determine which AVL balancing method will be
 * performed.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: balance()
{
	// choose the balancing act to perform
	if (first->path)
		second->path ? balance11() : balance10();
	else
		second->path ? balance01() : balance00();
}


/***********************************************************************************************************************
 * METHOD balance00
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will perform the balancing operation with the case of having left-left tracking branch
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: balance00()
{
	connectSubtreeSecond();

	// if the second->right child is equal to NULL, then by AVL property, each A,B,C,D child nodes of the tracking
	//    branch will be equal to NULL as well
	if (second->right)
	{
		first->left = second->right;
		first->left->parent = first;
	}
	else
		first->left = NULL;

	// re-balance
	second->right = first;
	first->parent = second;

	// propagate state
	first->state  = '=';
	second->state = '=';
}


/***********************************************************************************************************************
 * METHOD balance01
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will perform the balancing operation with the case of having left-right tracking branch.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: balance01()
{
	connectSubtreeThird();

	// connect first and second to third
	first->left    = third->right;
	second->right  = third->left;
	third->left    = second;
	third->right   = first;
	first->parent  = third;
	second->parent = third;

	if (first->left)
		first->left->parent = first;

	if (second->right)
		second->right->parent = second;

	// propagate state
	if (!first->right)
	{
		first->state  = '=';
		second->state = '=';
	}
	else if (third->state == '>')
	{
		first->state  = '=';
		second->state = '<';
	}
	else
	{
		first->state  = '>';
		second->state = '=';
	}

	third->state = '=';
}


/***********************************************************************************************************************
 * METHOD balance10
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will perform the balancing operation with the case of having right-left tracking branch.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: balance10()
{
	connectSubtreeThird();

	// connect first and second to third
	first->right   = third->left;
	second->left   = third->right;
	third->left    = first;
	third->right   = second;
	first->parent  = third;
	second->parent = third;

	if (first->right)
		first->right->parent = first;

	if (second->left)
		second->left->parent = second;

	// propagate state
	if (!first->left)
	{
		first->state  = '=';
		second->state = '=';
	}
	else if (third->state == '>')
	{
		first->state  = '<';
		second->state = '=';
	}
	else
	{
		first->state  = '=';
		second->state = '>';
	}

	third->state = '=';
}


/***********************************************************************************************************************
 * METHOD
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will print all levels of the Trifectopod.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: balance11()
{
	connectSubtreeSecond();

	// if the second->left child is equal to NULL, then by AVL property, each A,B,C,D child nodes of the tracking
	//    branch will be equal to NULL as well
	if (second->left)
	{
		first->right = second->left;
		first->right->parent = first;
	}
	else
		first->right = NULL;

	// re-balance
	second->left  = first;
	first->parent = second;

	// propagate state
	first->state  = '=';
	second->state = '=';
}


/***********************************************************************************************************************
 * METHOD connectSubtreeSecond
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will connect the second node of the tracking branch to the parent of the balancing subtree.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: connectSubtreeSecond()
{
	Node<type> *parent = first->parent;

	// IF - attach the the new root of the balancing sub-tree to the child node of the parent to the sub-tree
	if (parent)
	{
		if (parent->path)
			parent->right = second;
		else
			parent->left = second;

		second->parent = parent;
	}
	else
	{
		root = second;
		second->parent = NULL;
	}
}


/***********************************************************************************************************************
 * METHOD connectSubtreeThird
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will connect the third node of the tracking branch to the parent of the balancing subtree.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: connectSubtreeThird()
{
	Node<type> *parent = first->parent;

	// IF - attach the the new root of the balancing sub-tree to the child node of the parent to the sub-tree
	if (parent)
	{
		if (parent->path)
			parent->right = third;
		else
			parent->left = third;

		third->parent = parent;
	}
	else
	{
		root = third;
		third->parent = NULL;
	}
}


/***********************************************************************************************************************
 * METHOD print
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will print the AVL tree.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: print(ostream& out)
{
	inOrder();
	printTree(constructLevels(), out);
}


/***********************************************************************************************************************
 * METHOD inOrder
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will perform an in-order traversal of the tree which will gain print variables.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: inOrder()
{
	clean();
	initLevels();
	inOrder(root, 0);
}


/***********************************************************************************************************************
 * METHOD inOrder
 * ---------------------------------------------------------------------------------------------------------------------
 * This is a recursive helper method for inOrder.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: inOrder(Node<type> *node, int level)
{
	if(node != NULL)
	{
		updateMaxDigits(node->id);

		// recurse left
		inOrder(node->left, level + 1);

		// SET - root height and add to levels
		node->printHeight = level;
		levels.get(level)->item->add(node);

		if (!node->left)
			fillNullNodes(node);

		if (!node->right)
			fillNullNodes(node);

		// recurse right
		inOrder(node->right, level + 1);
	}
}


/***********************************************************************************************************************
 * METHOD fillNullNodes
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will fill the lists with null node placers so an even binary tree may be represented.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: fillNullNodes(Node<type> *node)
{
	for(int i = node->printHeight + 1; i < levels.size(); ++i)
	{
		int filler = pow(2, (i - 1) - node->printHeight);

		for(int k = 0; k < filler; ++k)
		{
			Node<type> *nullNode = new Node<type>;
			nullNode->id = -1;
			nullNode->printHeight = 0;
			levels.get(i)->item->add(nullNode);
		}
	}
}


/***********************************************************************************************************************
 * METHOD printTree
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will print all levels of the AVL tree.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: printTree(List<string> *treeString, ostream& out)
{
	for(int i = 0; i < treeString->size(); ++i)
	{
		out << treeString->get(i)->item << endl;
	}
}


/***********************************************************************************************************************
 * METHOD clean
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will destroy the print variables.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: clean()
{
	height = 0;
	cleanLevels();
}


/***********************************************************************************************************************
 * METHOD cleanLevels
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will destroy the levels list for print variables.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: cleanLevels()
{
	for(int i = 0; i < levels.size(); ++i)
	{
		(levels.get(i)->item)->~List();
	}

	levels.~List();
}


/***********************************************************************************************************************
 * METHOD initLevels
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will initialize linked lists for each level of the tree.
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: initLevels()
{
	levels.add(new List<Node<type> *>);
	initLevels(root, 0);
}


/***********************************************************************************************************************
 * METHOD initLevels
 * ---------------------------------------------------------------------------------------------------------------------
 * This is an iterative helper method for initLevels
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: initLevels(Node<type> *root, int level)
{
	if (root)
	{
		// IF - we have traveled to a node deeper than the current height of the traversal
		if (level > height)
		{
			height = level;
			levels.add(new List<Node<type> *>);
		}

		// recurse left
		initLevels(root->left, level + 1);

		// recurse right
		initLevels(root->right, level + 1);
	}
}


/***********************************************************************************************************************
 * METHOD updateMaxDigits
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will update the maximum width of the input tree id's
 **********************************************************************************************************************/
template<class type>
void AVL<type> :: updateMaxDigits(int id)
{
	if (digits(id) > maxDigits)
	{
		maxDigits = digits(id);
		nodeLength = maxDigits + 2;
	}
}


/***********************************************************************************************************************
 * METHOD
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will construct all levels of the AVL tree in printable form.
 **********************************************************************************************************************/
template<class type>
List<string>* AVL<type> :: constructLevels()
{
	int between;
	int prevBetween;
	int powCount;
	int first;
	int branch;
	List<string> *treeString = new List<string>;

	// INITIALIZE
	powCount    = 2;
	first       = 0;
	branch      = 0;
	prevBetween = 3 * nodeLength;
	between     = 3 * nodeLength;

	// builds the tree from the base lvl up to the root
	for(int i = levels.size() - 1; i >= 0; --i)
	{
		// build the tree level by level
		treeString->push(lvlString(first, branch, prevBetween, levels.get(i)->item));

		// calculate factors for spacing the row correctly
		powCount++;
		branch      = pow(2,(levels.size() - i)) * nodeLength;
		first       = between - nodeLength - branch;
		prevBetween = between;
		between     = (pow(2,powCount) - 1) * nodeLength;
	}

	return treeString;
}


/***********************************************************************************************************************
 * METHOD
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will print all levels of the Trifectopod.
 **********************************************************************************************************************/
template<class type>
string AVL<type> :: paddedInt(int n, int width)
{
	ostringstream buffer;

	buffer << pad('0', 0, width - digits(n), (new ostringstream));
	buffer << n;

	return buffer.str();
}


/***********************************************************************************************************************
 * METHOD pad
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will return a pad of the character c repeated length times.
 **********************************************************************************************************************/
template<class type>
string AVL<type> :: pad(char c, int length)
{
	return pad(c, 0, length, (new ostringstream));
}


/***********************************************************************************************************************
 * METHOD pad
 * ---------------------------------------------------------------------------------------------------------------------
 * This is an iterative helper method for pad.
 **********************************************************************************************************************/
template<class type>
string AVL<type> :: pad(char c, int i, int n, ostringstream *buffer)
{
	if(i != n)
	{
		*buffer << c;
		return pad(c, i + 1, n, buffer);
	}

	return buffer->str();
}


/***********************************************************************************************************************
 * METHOD lvlString
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will return a string representation of the tree level.
 **********************************************************************************************************************/
template<class type>
string AVL<type> :: lvlString(int first, int branch, int between, List<Node<type> *> *level)
{
	ostringstream buffer;
	buffer << pad(' ', first);

	for(int i = 0; i < level->size(); ++i)
	{
		buffer << pad('_', branch);
		buffer << paddedInt(level->get(i)->item->id, maxDigits);
		buffer << ' ' << level->get(i)->item->state;
		buffer << pad('_', branch);
		buffer << pad(' ', between);
	}

	return buffer.str();
}


/***********************************************************************************************************************
 * METHOD digits
 * ---------------------------------------------------------------------------------------------------------------------
 * This method tells the width of the digits of an integer.
 **********************************************************************************************************************/
template<class type>
int AVL<type> :: digits(int n)
{
	if(n > 0)
		return log10(n) + 1;
	else if(n < 0)
		return log10(fabs(n)) + 2;

	return 1;
}


#endif /* AVL_H_ */
