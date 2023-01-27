#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"
#include "list.h"

//structure for the set
typedef struct set{
	int count;
	int length;
	LIST **lists;
	int(*compare)();
	unsigned (*hash)();
}SET;

//structure for the node
typedef struct node{
	void *data;
	struct node *next;
	struct node *prev;
}NODE;

//structure for the list
typedef struct list{
	int count;
	int (*compare)();
	NODE *head;
}LIST;

//worst case O(n) average case O(n)
//creates the set and allocates all pointers
SET *createSet(int maxElts, int(*compare)(), unsigned(*hash)()){
	SET *sp;
	
	assert(compare != NULL && hash != NULL);
	
	sp=malloc(sizeof(SET));
	assert(sp != NULL);
	
	sp->length = maxElts / 20;
	sp->compare = compare;
	sp->hash = hash;
	sp->count = 0;
	
	sp->lists = malloc(sizeof(LIST*)*sp->length);
	assert(sp->lists != NULL);
	
	for(int i=0; i<sp->length; i++){
		sp->lists[i] = createList(compare);
	}
	return sp;
}

//worst case O(n) average case O(n)
//destroys all lists and frees all memory
void destroySet(SET *sp){
	assert(sp != NULL);
	for(int i=0; i<sp->length; i++){
		destroyList(sp->lists[i]);
	}
	free(sp->lists);
	free(sp);
}

//worst case O(1) average case O(1)
//returns number of elements in the set
int numElements(SET *sp){
	assert(sp != NULL);
	return sp->count;
}

//worst case O(n) average case O(1)
//adds element to the the list at the hash index
void addElement(SET *sp, void *elt){
	assert(sp != NULL);
	int hash = ((sp->hash)(elt))%(sp->length);
	if(findItem(sp->lists[hash], elt) == NULL){
		addLast(sp->lists[hash], elt);
		sp->count++;
	}
}

//worst case O(n) average case O(n)
//using hash index if element is found in list removes it
void removeElement(SET *sp, void *elt){
	assert(sp != NULL);
	int hash = ((sp->hash)(elt))%(sp->length);
	if(findItem(sp->lists[hash], elt) != NULL){
		removeItem(sp->lists[hash], elt);
		sp->count--;
	}
}

//worst case O(n) average case O(n)
//if element is in the set returns element else returns NULL
void *findElement(SET *sp, void *elt){
	assert(sp != NULL);
	int hash = ((sp->hash)(elt))%(sp->length);
	
	return findItem(sp->lists[hash], elt);
}

//worst case O(n) average case O(n)
//creates and returns an array with all elements in the set
void *getElements(SET *sp){
	assert(sp != NULL);
	void**setElts;
	setElts = malloc(sizeof(void*)*sp->count);
	assert(setElts != NULL);
	int index=0;

	for(int i=0; i<sp->length; i++){
		void**listElts = getItems(sp->lists[i]);
		
		for(int j=0; j<numItems(sp->lists[i]); j++){
			setElts[index+j] = listElts[j];	
		}
		index += numItems(sp->lists[i]);

	}
	return setElts;
}
