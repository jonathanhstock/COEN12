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

static int search(SET *sp, char*elt, bool *found);

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
	assert (sp != NULL);
	return sp->count;
}

//O(n)
//adding an element in a sorted order
void addElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	//checking if element already exists within the set
	int x = search(sp, elt, &found);
	
	if(found == false){
		if(sp->count < sp->length){
			for(int i=sp->count-1; i>=x; i--){
				sp->data[i+1] = sp->data[i];
			}	
			sp->data[x] = strdup(elt);
			sp->count++;
		}
	}
}

//O(n)
//removing a specific element and filling in the empty spot with a sorted element
void removeElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	int x = search(sp, elt, &found);

	if(found == true){
		free(sp->data[x]);
		for(int i=x; i<sp->count-1; i++){
			sp->data[i] = sp->data[i+1];
		}
		sp->count--;
	}
}

//O(logn)
//using search function to find a specific element and returning it
char *findElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	int x = search(sp, elt, &found);
	
	if(found == false){
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
	char** x;
	x = malloc(sizeof(char*)*sp->count);
	assert(x != NULL);
	for(int i=0; i<sp->count; i++){
		x[i] = sp->data[i];
	}
	return x;
}

//O(logn)
//binary searching through the set
static int search(SET *sp, char *elt, bool *found){
	int lo, hi, mid;
	lo = 0;
	mid = 0;
	hi = sp->count-1;
	while(lo<=hi){
		mid = (lo+hi)/2;
		int num = strcmp(sp->data[mid], elt);
		if(num == 0){
			*found = true;
			return mid;
		}
		else if(num < 1){
			hi = mid - 1;
		}
		else{
			lo = mid +1;
		}
	}
	*found = false;
	return lo;
}
