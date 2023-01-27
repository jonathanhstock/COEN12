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


//declaring the structure for the set
typedef struct set{
	char** data;
	int length;
	int count;
	char* flags;
}SET;

//O(n)
//given hash function
static unsigned strhash(char *s){
        unsigned hash = 0;

        while(*s != '\0'){
                hash = 31 * hash + * s++;
        }
        return hash;
}

//O(n)
//search function that uses linear probing to search for element
static int search(SET *sp, char *elt, bool *found){
        int hash = strhash(elt) % sp->length;
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
		else if(strcmp(sp->data[locn], elt) == 0){
			*found = true;
			return locn;
		}
	}		
        *found = false;
	return available; 
}

//O(n)
//creates the set with an array for the strings and array for the flags 
SET *createSet(int maxElts){
	int i;
	SET *sp;
	
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	
	sp->data = malloc(sizeof(char*)*maxElts);
	assert(sp->data != NULL);
	
	sp->flags = malloc(sizeof(char)*maxElts);
	assert(sp->flags != NULL);
	
	sp->length = maxElts;
	sp->count = 0;
	
	for(i = 0; i<maxElts; i++){
		sp->flags[i] = EMPTY;
	}
	return sp;
}

//O(n)
//destroys every element of data for each element that flag is filled
void destroySet(SET *sp){
	assert(sp != NULL);
	
	for(int i = 0; i < sp->length; i++){
		if(sp->flags[i] == FILLED){
			free(sp->data[i]);
		}
	}
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
//adds a single element in the set if it doesn't already exist
void addElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	int x = search(sp, elt, &found);
	
	if(!found){
		assert(sp->count < sp->length);
		sp->data[x] = strdup(elt);
		sp->flags[x] = FILLED;
		sp->count++;
	}
}

//O(n)
//removes a single element in the set if it exists
void removeElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	bool found;
	int x = search(sp, elt, &found);
	
	if(found){
		free(sp->data[x]);
		sp->flags[x] = DELETED;
		sp->count--;
	}
}

//O(n)
//uses search function to find an element in the set and returns it if true or returns null
char *findElement(SET *sp, char *elt){
	assert(sp != NULL && elt != NULL);
	bool found;

	int x = search(sp, elt, &found);
	
	if(found){
		return sp->data[x];
	}
	return NULL;
}

//O(n)
//creates an array of length count and fills it with elements from the set and returns
char **getElements(SET *sp){
	assert(sp != NULL);
	char**x;
	x = malloc(sizeof(char*)*sp->count);
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
