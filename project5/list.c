#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"

//node structure with next and prev pointers and data
typedef struct node{
        void *data;
        struct node *next;
        struct node *prev;
}NODE;

//list structure with head pointer and counter
typedef struct list{
	int count;	
	NODE *head;
	int (*compare)();
}LIST;

//O(1)
//creates list and dummy node
LIST *createList(int (*compare)()){
	LIST *lp;
	lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	lp->count = 0;
	lp->compare = compare;
	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	return lp;
}

//O(n)
//destroys entire list and frees all memory
void destroyList(LIST *lp){
	assert(lp != NULL);
	NODE *temp = lp->head->next;
	while(temp != lp->head){
		lp->head->next = temp->next;
		free(temp);
		temp = lp->head->next;
	}
	free(lp->head);
	free(lp);
}

//O(1)
//returns the number of items in the list
int numItems(LIST *lp){
	assert(lp != NULL);
	return lp->count;
}

//O(1)
//adds a node in the first slot after the head pointer with item as data
void addFirst(LIST *lp, void *item){
	assert(lp != NULL);
	NODE *new;
	new = malloc(sizeof(NODE));
	new->data = item;
	new->next = lp->head->next;
	new->prev = lp->head;
	lp->head->next->prev = new;
	lp->head->next = new;
	lp->count++;
}

//O(1)
//adds a node in the last slot before the head pointer with item as data
void addLast(LIST *lp, void *item){
	assert(lp != NULL);
	NODE *new;
	new = malloc(sizeof(NODE));
	new->data = item;
	new->next = lp->head;
	new->prev = lp->head->prev;
	lp->head->prev->next = new;
	lp->head->prev = new;
	lp->count++;
}

//O(1)
//removes the first node after the head pointer
void *removeFirst(LIST *lp){
	assert(lp != NULL && lp->count != 0);
	NODE *temp = lp->head->next;
	void* data = temp->data;
	temp->next->prev = lp->head;
	lp->head->next = temp->next;
	temp->next = NULL;
	temp->prev = NULL;
	free(temp);
	lp->count--;
	return data;
}

//O(1)
//removes the last node before the head pointer
void *removeLast(LIST *lp){
	assert(lp != NULL);
	NODE *temp = lp->head->prev;
	void* data = temp->data;
	temp->prev->next = lp->head;
	lp->head->prev = temp->prev;
	temp->next = NULL;
	temp->prev = NULL;
	free(temp);
	lp->count--;
	return data;
}

//O(1)
//returns the data of the first node in the list
void *getFirst(LIST *lp){
	assert(lp != NULL && lp->count != 0);
	return lp->head->next->data;
}

//O(1)
//returns the data of the last node in the list
void *getLast(LIST *lp){
	assert(lp != NULL && lp->count != 0);
	return lp->head->prev->data;
}

//O(n)
//removes a specific item in the list
void removeItem(LIST *lp, void *item){
	assert(lp != NULL);
	NODE *current = lp->head->next;
	
	while(current->data != lp->head){
		if((*lp->compare)(current->data, item) == 0){
			current->next->prev = current->prev;
			current->prev->next = current->next;
			free(current);
			lp->count--;
			break;
		}
		current = current->next;
	}
	
}

//O(n)
//finds a specific item in the list 
void *findItem(LIST *lp, void *item){
	assert(lp != NULL);
	NODE *current;
	current = lp->head->next;
	while(current != lp->head){
		assert(current->data != NULL);
		if((*lp->compare)(current->data, item) == 0){
			return current->data;
		}
		current = current->next;	
	} 
	return NULL;
}

//O(n)
//gets all items in the list
void *getItems(LIST *lp){
	assert(lp != NULL);
	NODE *current = lp->head->next;
	void**array = malloc(sizeof(void*)*lp->count);
	assert(array != NULL);
	int i = 0;
	while(current != lp->head && i<lp->count){
		current->data = array[i];
		current = current->next;
		i++;
	}
	return array;;
}
