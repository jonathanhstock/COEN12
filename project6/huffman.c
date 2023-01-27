#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"
#include "pack.h"
#include <ctype.h>

/* Commands to compile and run
make huffman
./huffman input.txt output.txt
*/

// Function that makes the new node from the left and right child
struct node *mknode(int data, struct node *left, struct node *right)
{
	struct node *np = malloc(sizeof(struct node));
	assert(np != NULL);
	
	np->parent = NULL;
	np->count = data;
	
	if (left != NULL)
	{
		left->parent = np;
	}
	if (right != NULL)
	{
		right->parent = np;
	}
	
	return np;
}

// Function that compares two nodes and returns -1 if arg1 < arg2, 1 if arg1 > arg2, and 0 if arg1 == arg2
static int nodecmp(struct node *t1, struct node *t2)
{
	assert(t1 != NULL);
	assert(t2 != NULL);
	
	if (t1->count < t2->count)
	{
		return -1;
	}
	else if (t1->count > t2->count)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// Function that finds the height of the huffman tree
static int depth(struct node *np)
{
	assert(np != NULL);
	
	int height = -1;

	while (np != NULL)
	{
		height++;
		np = np->parent;
	}	
	
	return height;	
}

// Main function
int main(int argc, char *argv[])
{
	FILE *fp;
	
	assert(argc > 2);
	
	struct node *leaves[257] = {NULL};

	int counts[257] = {0};
	
	char *infile = argv[1];
	char *outfile = argv[2];
	
	fp = fopen(infile, "r");
	
	// Checks if file exists
	if (infile == NULL)
	{
		printf("Cannot open file\n");
		return -1;
	}
	
	int c;
	
	// Counts the number of characters in the file
	while ((c = fgetc(fp)) != EOF)
	{
		counts[c]++;
	}
	
	// Makes a node for every character if there are any occurrences
	for (int l = 0; l < 256; l++)
	{
		if (counts[l] != 0)
		{
			leaves[l] = mknode(counts[l], NULL, NULL);	
		}
	}
	
	leaves[256] = mknode(0, NULL, NULL);
	
	PQ *pq;
	
	// Creates the priority queue
	pq = createQueue(nodecmp);
	
	// Adds all of the nodes into the priority queue
	for (int x = 0; x <= 256; x++)
	{
		if (leaves[x] != NULL)
		{
			addEntry(pq, leaves[x]);	
		}
	}	
	
	// Creates the huffman tree in the priority queue
	while(numEntries(pq) > 1)
	{
		struct node *templeft = removeEntry(pq);
		struct node *tempright = removeEntry(pq);
		struct node *np = mknode((templeft->count + tempright->count), templeft, tempright);
		addEntry(pq, np); 
	}	 
	
	// For loop that traverses the node array to print characters, occurrences, and lengths
	int bitlength = 0;	

	for (int i = 0; i < 257; i++)
	{
		if (leaves[i] != NULL)
		{
			if (isprint(i))
			{
				printf("'%c': %d x %d = %d bits", i, leaves[i]->count, depth(leaves[i]), (leaves[i]->count * depth(leaves[i])));
				printf("\n");
				bitlength += (leaves[i]->count * depth(leaves[i])); 	
			}
			else
			{
				printf("%03o: %d x %d = %d bits", i, leaves[i]->count, depth(leaves[i]), (leaves[i]->count * depth(leaves[i])));
				printf("\n");
				bitlength += (leaves[i]->count * depth(leaves[i]));
			}
		}
	}
	printf("Total bits are %d\n", bitlength);
	
	pack(infile, outfile, leaves);

	fclose(fp);

	return 0;
}
