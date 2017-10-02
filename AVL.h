/*******************************************************************************
 * PROGRAMMER : ERIC OLAVESON
 * DATE       : 8/29/2017
 ******************************************************************************/
#ifndef AVL_H_
#define AVL_H_

#include "List.h"
#include "Node.h"


/*******************************************************************************
 * CLASS - AVL
 * -----------------------------------------------------------------------------
 * This class encapsulates methods which construct a self-balancing binary tree
 * known as the AVL tree. The methods included operate on nodes which contain
 * a balancing state rather than a height variable. This balancing state allows
 * the AVL tree to re-balance its branches without performing comparisons
 * between nodes heights, rather by node states.
 ******************************************************************************/
template<class type>
class AVL
{
public:
	Node<type> *root;

	bool insert(int id, type item);

private:
	Node<type> *first;
	Node<type> *second;
	Node<type> *third;

	// INSERT helpers
	Node<type>* findLeafNode(int);
	void attachNode(Node<type>*, Node<type>*);
	void insertionUpdate(Node<type>*);

	// BALANCE helpers
	void balance(Node<type>*);
	void removalBalance(Node<type>*);
	Node<type>* balance00(Node<type> *node);
	Node<type>* balance01(Node<type> *node);
	Node<type>* balance10(Node<type> *node);
	Node<type>* balance11(Node<type> *node);
	void connectSubtree(Node<type>*);
};


/*******************************************************************************
 * FUNCTION - insert
 * -----------------------------------------------------------------------------
 * This function attempts to insert a node into the AVL tree.
 * -----------------------------------------------------------------------------
 * return: bool - if the insertion was a success
 ******************************************************************************/
template<class type>
bool AVL<type> :: insert(int id, type item)
{
	if (!root)
	{
		root = new Node<type>(id, item);
		return true;
	}
	else
	{
		Node<type> *p_node = findLeafNode(id);
		if (p_node)
		{
			attachNode(p_node, new Node<type>(id, item));
			insertionUpdate(p_node);
			return true;
		}
		return false;
	}
}


/*******************************************************************************
 * FUNCTION - findLeafNode
 * -----------------------------------------------------------------------------
 * This function will search the AVL tree to find a leaf node with which we may
 * insert a node with the given key to.
 * -----------------------------------------------------------------------------
 * return: Leaf node corresponding to key, NULL if the node is contained
 * 		   already.
 ******************************************************************************/
template<class type>
Node<type>* AVL<type> :: findLeafNode(int key)
{
	Node<type> *next = root;
	Node<type> *p_node;

	while (next)
	{
		p_node = next;
		if (key < next->id)
		{
			next->path = '<';
			next = next->left;
		}
		else if (key > next->id)
		{
			next->path = '>';
			next = next->right;
		}
		else
			return NULL;
	}
	return p_node;
}


/*******************************************************************************
 * FUNCTION - attachNode
 * -----------------------------------------------------------------------------
 * This function will attach a node as a child to p_node as its parent. We must
 * check to see if the node will be the parent's left or right child before
 * continuing.
 ******************************************************************************/
template<class type>
void AVL<type> :: attachNode(Node<type>* p_node, Node<type>* node)
{
	if (p_node)
		(p_node->steppedLeft() ? p_node->left : p_node->right) = node;
	else
		root = node;
	node->parent = p_node;
}


/*******************************************************************************
 * FUNCTION - insertionUpdate
 * -----------------------------------------------------------------------------
 * This function will start at an AVL tree node and iterate up the tree looking
 * for a state change or re-balancing opportunity. Once the tree have been
 * verified as balanced by this function, we may ensure that the tree holds
 * AVL height property for each node.
 ******************************************************************************/
template<class type>
void AVL<type> :: insertionUpdate(Node<type> *next)
{
	while (next && next->balanced())
	{
		next->state = next->path;
		next = next->parent;
	}

	if (next)
		balance(next);
}


/*******************************************************************************
 * FUNCTION - balance
 * -----------------------------------------------------------------------------
 * This function changes the state of the node passed in. It will re-balance
 * the surrounding nodes if a doubly unbalanced node is met.
 ******************************************************************************/
template<class type>
void AVL<type> :: balance(Node<type> *node)
{
	if (!node->doubleHeavy())
	{
		node->state = '=';
		return;
	}

	// choose the balancing act to perform
	if (node->steppedRight())
		node->right->steppedRight() ? balance11(node) : balance10(node);
	else
		node->left->steppedRight() ? balance01(node) : balance00(node);
}


/*******************************************************************************
 * FUNCTION - balance00
 * -----------------------------------------------------------------------------
 * This function will perform a left-left balance with the passed in node as
 * the highest node on the tree.
 ******************************************************************************/
template<class type>
Node<type>* AVL<type> :: balance00(Node<type> *node)
{
	first = node;
	second = node->left;
	third = second->left;
	attachNode(node->parent, second);

	// if the second->right child is equal to NULL, then by AVL property, each
	// A,B,C,D child nodes of the tracking branch will be equal to NULL as well
	first->left = second->right;
	if (second->right)
		first->left->parent = first;

	// re-balance
	second->right = first;
	first->parent = second;

	// propagate state
	first->state  = '=';
	second->state = '=';
	return second->parent;
}


/*******************************************************************************
 * FUNCTION - balance01
 * -----------------------------------------------------------------------------
 * This function will perform a left-right balance, with the node passed in as
 * the highest node on the tree.
 ******************************************************************************/
template<class type>
Node<type>* AVL<type> :: balance01(Node<type> *node)
{
	first = node;
	second = node->left;
	third = second->right;
	attachNode(node->parent, third);

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
	return third->parent;
}


/*******************************************************************************
 * FUNCTION - balance10
 * -----------------------------------------------------------------------------
 * This function will perform a right-left balance, with the node passed in as
 * the highest node on the tree.
 ******************************************************************************/
template<class type>
Node<type>* AVL<type> :: balance10(Node<type> *node)
{
	first = node;
	second = node->right;
	third = second->left;
	attachNode(node->parent, third);

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
	return third->parent;
}


/*******************************************************************************
 * FUNCTION - balance11
 * -----------------------------------------------------------------------------
 * This function will perform a right-right balance, with the node passed in
 * as the highest node in the tree.
 ******************************************************************************/
template<class type>
Node<type>* AVL<type> :: balance11(Node<type> *node)
{
	first = node;
	second = node->right;
	third = second->right;
	attachNode(node->parent, second);

	// if the second->left child is equal to NULL, then by AVL property, each
	// A,B,C,D child nodes of the tracking branch will be equal to NULL as well
	first->right = second->left;
	if (second->left)
		first->right->parent = first;

	// re-balance
	second->left  = first;
	first->parent = second;

	// propagate state
	first->state  = '=';
	second->state = '=';
	return second->parent;
}


#endif /* AVL_H_ */
