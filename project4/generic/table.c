#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

//defines the three flags for the array
#define FILLED 1
#define EMPTY 0
#define DELETED 2

//creates the structure for the set
typedef struct set{
	void** data;
	int length;
	int count;
	char* flags;
	int (*compare)();
	unsigned (*hash)();
}SET;

//O(n)
//search function that uses linear probing to search through the set
static int search(SET *sp, char *elt, bool *found){
        int hash = (*sp->hash)(elt) % sp->length;
        int available = -1;
	
	for(int i = 0; i<sp->length; i++){
		int locn = (hash+i) % sp->length;

		if(sp->flags[locn] == EMPTY){
			*found = false;
			if(available == -1){
				return locn;
			}
			return available;
		}
		else if(sp->flags[locn] == DELETED){
			if(available == -1){
				available = locn;
			}
		}
		else if((*sp->compare)(sp->data[locn], elt) == 0){
			*found = true;
			return locn;
		}
	}		
        *found = false;
	return available; 
}

//O(n)
//creates the set for generic pointer array and flags array 
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()){
	int i;
	SET *sp;
	
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	
	sp->data = malloc(sizeof(void*)*maxElts);
	assert(sp->data != NULL);
	
	sp->flags = malloc(sizeof(char)*maxElts);
	assert(sp->flags != NULL);
	
	sp->hash = hash;
	sp->compare = compare;
	sp->length = maxElts;
	sp->count = 0;
	
	for(i = 0; i<maxElts; i++){
		sp->flags[i] = EMPTY;
	}
	return sp;
}

//O(1)
//destroys the array of data and flags
void destroySet(SET *sp){
	assert(sp != NULL);
	
	free(sp->data);
	free(sp->flags);
	free(sp);
}

//O(1)
//returns the count of elements
int numElements(SET *sp){
	assert(sp != NULL);
	return sp->count;
}

//O(n)
//adds an element to the set if it doesn't already exist
void addElement(SET *sp, void *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	int x = search(sp, elt, &found);
	
	if(!found){
		assert(sp->count < sp->length);
		sp->data[x] = elt;
		sp->flags[x] = FILLED;
		sp->count++;
	}
}

//O(n)
//removes and element from the set updates flags and count
void removeElement(SET *sp, void *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	int x = search(sp, elt, &found);
	
	if(found){
		sp->flags[x] = DELETED;
		sp->count--;
	}
}

//O(n)
//searches for element and if found returns it otherwise returns null
void *findElement(SET *sp, void *elt){
	assert(sp != NULL && elt != NULL);
	bool found;

	int x = search(sp, elt, &found);
	
	if(found){
		return sp->data[x];
	}
	return NULL;
}

//O(n)
//creates an array and fills it with elements from the set and returns
void *getElements(SET *sp){
	assert(sp != NULL);
	void**x;
	x = malloc(sizeof(void*)*sp->count);
	assert(x != NULL);
	
	int j = 0;
	for(int i = 0; i<sp->length && j<sp->count; i++){
		if(sp->flags[i] == FILLED){
			x[j] = sp->data[i];
			j++;
		}
	}
	return x;
}
