/*******************************************************************************
 * PROGRAMMER : ERIC OLAVESON
 * DATE       : 8/29/2017
 ******************************************************************************/
#ifndef TREEPRINTER_H_
#define TREEPRINTER_H_

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

#include "Node.h"
#include "List.h"
#include "ListIter.h"


/*******************************************************************************
 * CLASS - TreePrinter
 * -----------------------------------------------------------------------------
 * This class encapsulates methods to print a binary tree. The methods included
 * in this class help to correctly space and add empty nodes wherever needed.
 * The output is useful to see how a binary tree is being manipulated as well
 * as where things may have gone wrong.
 ******************************************************************************/
template<class type>
class TreePrinter
{

public:
	void print(Node<type>*, ostream&);

private:
	int height;
	int maxDigits;
	int nodeLength;
	Node<type> *root;
	List<Node<type>*> leaves;
	List<List<Node<type>*>*> levels;

	// PRINT helpers
	int  getHeight(Node<type>*);
	int  getMaxDigits(Node<type>*);
	int  digits(int);

	void inOrder(Node<type>*, int);
	void fillPrinterForEmptyNode(int);

	List<string>* constructLevels();
	void cleanLevels();

	string pad(char, int);
	string paddedInt(int);
	string lvlString(int, int, int, List<Node<type>*>*);
};

template<class type>
void TreePrinter<type> :: print(Node<type> *root, ostream& out)
{
	cout << endl << endl
		 << "~~~~~~~~~~~~~~~~~~~~~~~~" << endl
		 << "TREE PRINTER!!" << endl
		 << "~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	height = getHeight(root);
	maxDigits = getMaxDigits(root);
	nodeLength = maxDigits + 2;

	// INITIALIZE - empty tree levels in list form for printer
	for (int i = 0; i < height; ++i)
		levels.add(new List<Node<type> *>);

	// TRAVERSE in order to gather tree node data
	inOrder(root, 0);
	List<string> *treeString = constructLevels();

	// OUTPUT - print the tree level by level
	for(int i = 0; i < treeString->size(); ++i)
		out << treeString->get(i)->item << endl;

	// CLEAN PRINTER
	height = 0;
	maxDigits = 0;
	cleanLevels();
}

template<class type>
void TreePrinter<type> :: inOrder(Node<type> *root, int level)
{
	if(root != NULL)
	{
		// recurse left
		inOrder(root->left, level + 1);

		// SET - root height and add to levels
		Node<type> *printNode = new Node<type>(root->id, root->item);
		printNode->height = level;
		printNode->state  = root->state;
		levels.get(level)->item->add(printNode);

		if(root->left == NULL)
			fillPrinterForEmptyNode(level);
		if(root->right == NULL)
			fillPrinterForEmptyNode(level);

		// recurse right
		inOrder(root->right, level + 1);
	}
}

template<class type>
void TreePrinter<type> :: fillPrinterForEmptyNode(int currentTreeLevel)
{
	for(int i = currentTreeLevel + 1; i < levels.size(); ++i)
	{
		int filler = pow(2, (i - 1) - currentTreeLevel);

		for(int k = 0; k < filler; ++k)
		{
			Node<type> *fillNode = new Node<type>(-1, -1);
			fillNode->state = '*';
			levels.get(i)->item->add(fillNode);
		}
	}
}

template<class type>
List<string>* TreePrinter<type> :: constructLevels()
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

template<class type>
string TreePrinter<type> :: lvlString(int first, int branch, int between, List<Node<type> *> *level)
{
	ostringstream buffer;
	buffer << pad(' ', first);

	for(int i = 0; i < level->size(); ++i)
	{
		buffer << pad('_', branch);
		buffer << paddedInt(level->get(i)->item->id);
		buffer << ' ' << level->get(i)->item->state;
		buffer << pad('_', branch);
		buffer << pad(' ', between);
	}

	return buffer.str();
}

template<class type>
string TreePrinter<type> :: paddedInt(int n)
{
	int l = maxDigits - digits(n);
	ostringstream buffer;

	for(int i = 0; i < l; ++i)
		buffer << 0;

	buffer << n;
	return buffer.str();
}

template<class type>
string TreePrinter<type> :: pad(char c, int length)
{
	ostringstream s;

	for(int i = 0; i < length; ++i)
		s << c;

	return s.str();
}

template<class type>
void TreePrinter<type> :: cleanLevels()
{
	for(int i = 0; i < levels.size(); ++i)
		(levels.get(i)->item)->~List();
	levels.~List();
}


template<class type>
int TreePrinter<type> :: digits(int n)
{
	if(n > 0)
		return log10(n) + 1;
	else if(n < 0)
		return log10(fabs(n)) + 2;
	else
		return 1;
}

template<class type>
int TreePrinter<type> :: getHeight(Node<type> *node)
{
	if (node)
		return max(getHeight(node->left), getHeight(node->right)) + 1;
	else
		return 0;
}

template<class type>
int TreePrinter<type> :: getMaxDigits(Node<type> *node)
{
	if (node)
	{
		int childMaxDigits = max(getMaxDigits(node->left), getMaxDigits(node->right));
		return max(childMaxDigits, digits((int)(node->id)));
	}
	else
		return 0;
}


#endif /* TREEPRINTER_H_ */
