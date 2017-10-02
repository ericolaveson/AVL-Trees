/*******************************************************************************
 * PROGRAMMER : ERIC OLAVESON
 * DATE       : 8/29/2017
 ******************************************************************************/
#include "AVL.h"
#include "TreePrinter.h"

#include <algorithm>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>


/*******************************************************************************
 * FUNCTION knuthRand
 * -----------------------------------------------------------------------------
 * This method will randomly shuffle a list based on the Knuth Random shuffling
 * algorithm.
 ******************************************************************************/
void knuthRand(int list[], int BOUND)
{
	int shuffles[BOUND];

	// generate random numbers
	for(int i = 0; i < BOUND; ++i)
	{
		list[i] = i;

		// CALC - hash the random number to an index
		shuffles[i] = rand() % BOUND;
	}

	// perform shuffles
	for(int i = 0; i < BOUND; ++i)
	{
		int temp = list[i];
		list[i] = list[shuffles[i]];
		list[shuffles[i]] = temp;
	}
}


/*******************************************************************************
 * FUNCTION - heightCheck
 * -----------------------------------------------------------------------------
 * This function will recursively check each node on a binary tree to see if
 * both node's children heights are within the bounds of an AVL tree node.
 * -----------------------------------------------------------------------------
 * Return:
 * 		If the tree's node heights are valid for the definition of an AVL tree,
 * 		returns True else False
 ******************************************************************************/
bool heightCheckOK = true;
int heightCheck(Node<int> *node)
{
	if (node)
	{
		int lHeight = heightCheck(node->left);
		int rHeight = heightCheck(node->right);
		int heightDiff = abs(lHeight - rHeight);

		heightCheckOK = heightCheckOK && (heightDiff <= 1);

		return max(lHeight, rHeight) + 1;
	}
	else
		return 0;
}
bool AVLTest_heightCheck(Node<int> *node)
{
	heightCheck(node);
	return heightCheckOK;
}


/*******************************************************************************
 * FUNCTION - completeAndOrdered
 * -----------------------------------------------------------------------------
 * This function will recursively check each node on a binary tree to see if
 * 		1) The AVL tree contains all nodes we have added
 * 		2) Nodes are in the order expected by a binary tree
 *      3) There are no duplicate nodes
 * -----------------------------------------------------------------------------
 * Return:
 * 		If the tree's node placements are valid for the definition of an AVL
 * 		tree, returns True else False
 ******************************************************************************/
bool completeAndOrderedOK = true;
int nodeCount = 0;
Node<int> *lastLeaf = NULL;
void completeAndOrdered(Node<int> *node)
{
	if (node)
	{
		completeAndOrdered(node->left);

		++nodeCount;
		if (node->left)
			completeAndOrderedOK &= (node->id > node->left->id);

		if (node->right)
			completeAndOrderedOK &= (node->id < node->right->id);

		if (!node->left && !node->right)
		{
			if (lastLeaf)
				completeAndOrderedOK &= (node->id > lastLeaf->id);
			lastLeaf = node;
		}

		completeAndOrdered(node->right);
	}
}
bool AVLTest_completeAndOrdered(Node<int> *node, int iterationBound)
{
	nodeCount = 0;
	lastLeaf  = NULL;

	completeAndOrdered(node);
	return completeAndOrderedOK && (nodeCount <= iterationBound);
}


/*******************************************************************************
 *  __  __          _____ _   _
 * |  \/  |   /\   |_   _| \ | |
 * | \  / |  /  \    | | |  \| |
 * | |\/| | / /\ \   | | | . ` |
 * | |  | |/ ____ \ _| |_| |\  |
 * |_|  |_/_/    \_\_____|_| \_|
 ******************************************************************************/
int main()
{
	const int BOUND = 100;
	int sequence[BOUND];
	AVL<int> avl = AVL<int>();
	TreePrinter<int> tPrinter;

	// INITIALIZE - randomize the order in which elements are added to the
	//              AVL tree.
	knuthRand(sequence, BOUND);

	// PRINT - sequence in which we will add elements to AVL tree
	cout << "\nSEQUENCE: ";
	for (int i = 0; i < BOUND; ++i)
		cout << " " << sequence[i];
	cout << endl;
	cout << endl;

	// BUILD TREES
	for(int i = 0; i < BOUND; ++i)
	{
		// INSERT - element into AVL tree
		avl.insert(sequence[i], sequence[i]);

		// TEST - check heights of each node's children to make sure the AVL
		//        height property holds throughout the tree
		if (!AVLTest_heightCheck(avl.root))
		{
			cout << "HEIGHT CHECK FAILED\n\n";
			tPrinter.print(avl.root, cout);
			break;
		}

		// TEST - 1) The AVL tree contains all nodes we have added
		//        2) Nodes are in the order expected by a binary tree
		//        3) There are no duplicate nodes
		if (!AVLTest_completeAndOrdered(avl.root, i+1))
		{
			cout << "COMPLETE CHECK FAILED\n\n";
			tPrinter.print(avl.root, cout);
			break;
		}
	}

	// If the AVL tree passes the tests for each node inserted then our final
	//    representation of the tree is in fact an AVL tree.
	if (heightCheckOK && completeAndOrderedOK)
	{
		cout << "THE AVL TREE HAS PASSED" << endl << endl;
		tPrinter.print(avl.root, cout);
	}

    return 0;
}



