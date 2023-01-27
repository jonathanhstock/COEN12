#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

// Definitions
#define p(i) (((i)-1)/2)
#define l(i) ((i)*2+1)
#define r(i) ((i)*2+2)
#define INIT_LENGTH 10

// Structure for priority queue
struct pqueue
{
	int count;
	int length;
	void **data;
	int(*compare)();
};

// O(1)
// Creates a new priority queue using compare
PQ *createQueue(int (*compare)())
{
	PQ *pq;
	
	assert(compare != NULL);
	
	pq = malloc(sizeof(PQ));
	assert(pq != NULL);
	
	pq->compare = compare;
	pq->length = INIT_LENGTH;
	pq->count = 0;
	
	pq->data = malloc(sizeof(void *)*pq->length);
	assert(pq->data != NULL);
		
	return pq;
}

// O(1)
// Deallocates memory for the priority queue
void destroyQueue(PQ *pq)
{
	assert(pq != NULL);
	
	free(pq->data);
	free(pq);
}

// O(1)
// Returns the number of entries in the priority queue
int numEntries(PQ *pq)
{
	assert(pq != NULL);
	
	return pq->count;
}

// O(nlogn)
// Adds an entry in the priority queue
void addEntry(PQ *pq, void *entry)
{
	assert(pq != NULL);
	
	// Creates a new priority queue if current one is full
	if (pq->count == pq->length)
	{
		pq->length *= 2;
		pq->data = realloc(pq->data, sizeof(void*)*pq->length);
	}

	pq->data[pq->count] = entry;
	pq->count++;

	int i = pq->count-1;	

	// While loop that reheaps up
	while(i >= 1)
	{
		if (pq->compare(pq->data[i], pq->data[p(i)]) < 0)
		{
			PQ *temp = pq->data[p(i)];
			pq->data[p(i)] = pq->data[i];
			pq->data[i] = temp; 
			i = p(i);
		}
		else
		{
			break;
		}
	}
}

// O(nlogn)
// Removes and returns the smallest entry in the priority queue
void *removeEntry(PQ *pq)
{
	assert(pq != NULL);

	void *root = pq->data[0];
	pq->data[0] = pq->data[pq->count-1];
	pq->data[pq->count-1] = NULL;
	pq->count--;
	
	int i = 0;
	
	// While loop that reheaps down
	while(l(i) < pq->count)
	{	
		int min;
		
		if (r(i) < pq->count && pq->compare(pq->data[r(i)], pq->data[l(i)]) < 0)
		{
			min = r(i);
		}
		
		else
		{
			min = l(i);
		}
		
		if(pq->compare(pq->data[i], pq->data[min]) >= 0)
		{
			PQ *temp = pq->data[i];
			pq->data[i] = pq->data[min];
			pq->data[min] = temp;
			i = min;		
		}
		else
		{
			break;
		}	
	}
	return root;
}
