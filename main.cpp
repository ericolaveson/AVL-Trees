

#include "AVL.h"
#include <algorithm>
#include <time.h>
#include <sys/time.h>

#include <iostream>
#include <fstream>

void knuthRand(int list[], int size);

/***********************************************************************************************************************
 * FUNCTION knuthRand
 * ---------------------------------------------------------------------------------------------------------------------
 * This method will randomly shuffle a list based on the Knuth Random shuffling algorithm.
 **********************************************************************************************************************/
void knuthRand(int list[], int size)
{
	int shuffles[size];

	// generate random numbers
	for(int i = 0; i < size; ++i)
	{
		list[i] = i;

		// CALC - hash the random number to an index
		shuffles[i] = rand() % size;
	}

	// perform shuffles
	for(int i = 0; i < size; ++i)
	{
		int temp = list[i];
		list[i] = list[shuffles[i]];
		list[shuffles[i]] = temp;
	}
}


/***********************************************************************************************************************
 *  __  __          _____ _   _
 * |  \/  |   /\   |_   _| \ | |
 * | \  / |  /  \    | | |  \| |
 * | |\/| | / /\ \   | | | . ` |
 * | |  | |/ ____ \ _| |_| |\  |
 * |_|  |_/_/    \_\_____|_| \_|
 **********************************************************************************************************************/
int main()
{
	const int BOUND  = 1000000;
	const int ROUNDS = 10;

	int     sequence[BOUND];
	AVL<int> avl = AVL<int>();

	// INITIALIZE
	knuthRand(sequence, BOUND);


	for(int i = 0; i < BOUND; ++i)
		avl.insert(sequence[i], sequence[i]);

	cout << "end";

    return 0;
}



