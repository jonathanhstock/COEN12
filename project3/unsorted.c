#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

typedef struct set{
	char** data;
	int length;
	int count;
}SET;

static int search(SET *sp, char *elt);

//O(1)
//creates a set
SET *createSet(int maxElts){
	SET* sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data != NULL);
	return sp;
}

//O(n)
//destroying the set
void destroySet(SET *sp){
	assert(sp != NULL);
	for(int i=0; i<sp->count; i++){
		free(sp->data[i]);
	}
	free(sp->data);
	free(sp);
}

//O(1)
//returning the number of elements
int numElements(SET *sp){
	assert(sp != NULL);
	return sp->count;
}

//O(n)
//adding an element
void addElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	//checking if element already exists within the set
	int x = search(sp, elt);
	
	if (x == -1){
		assert(sp->count < sp->length);
		//allocates memory using malloc, copies string, returns pointer to the copy
		sp->data[sp->count] = strdup(elt);
		assert(sp->data[sp->count] != NULL);
		sp->count++;
	}
}

//O(n)
//removing a specific element and filling in the empty spot
void removeElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	int x = search(sp, elt);
	
	if(x != -1){
		free(sp->data[x]);
		sp->data[x] = sp->data[sp->count-1];
		sp->count--;
	}
}

//O(n)
//using search function to find a specific element and returning it
char *findElement(SET*sp, char *elt){
	assert(sp != NULL && elt != NULL);
	int x = search(sp, elt);
	
	if(x == -1){
		return NULL;
	}
	else{
		return sp->data[x];
	}
}

//O(n)
//returning every element in the set
char **getElements(SET *sp){
	assert(sp != NULL);
	//make array with malloc, copy pointers into it
	char** x;
	x = malloc(sizeof(char*)*sp->count);
	assert(x != NULL);
	for(int i=0; i<sp->count; i++){
		x[i] = sp->data[i];
	}
	return x;
}

//O(n)
//sequentially searching through the set
static int search(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	for(int i=0; i<sp->count; i++){
		if(strcmp(sp->data[i],elt) == 0){
			return i;	
			break;
		}
	}
	return -1;
}
