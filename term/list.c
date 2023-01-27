#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "list.h"
#define LIST struct list
#define NODE struct node

/*
*	commands to compile
*	./qsort books/anybook.txt
*	./radix string of numbers then type ctrl-D
*	./radix < test with numbers in it
*	./maze
*/

//structure for the node
struct node{
	int length;
	int count;
	int start;
	void**array;
	struct node *next;
	struct node *prev;
};

//structure for the list
struct list{
	int count;
 	NODE *head;
	NODE *tail;
};

//O(1)
//list constructor
//returns pointer to the created list
LIST *createList(void){
	LIST *lp;
	lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	
	lp->count = 0;
	lp->head = NULL;
	lp->tail = NULL;
	
	return lp;
}

//O(n)
//list destructor
//deallocates all memory in the list
void destroyList(LIST *lp){
	assert(lp != NULL);

	NODE *temp = lp->head;

	while(temp != NULL){
		temp = temp->next;
		free(lp->head);
		lp->head = temp;
	}

	free(lp);
}

//O(1)
//returns the number of items in the list
int numItems(LIST *lp){
	assert(lp != NULL);
	
	return lp->count;
}

//O(1)
//adds an item as the first item in the list
void addFirst(LIST *lp, void *item){
	assert(lp != NULL);
	
	NODE *current = lp->head;
	
	//case 1 empty list
	if(current == NULL){
                NODE *only;
                only = malloc(sizeof(NODE));
                assert(only != NULL);

                only->length = 1;

                only->array = malloc(sizeof(void*)*only->length);
                assert(only->array != NULL);

                only->next = NULL;
                only->prev = NULL;
                lp->tail = only;
                lp->head = only;

                only->start = 0;
                only->array[only->start] = item;

                only->count = 1;
                lp->count++;
	}

	//case 2 node is full
	else if(current->count == current->length){
		NODE *first;
		first = malloc(sizeof(NODE));
		assert(first != NULL);

		first->length = 2 * current->length;

		first->array = malloc(sizeof(void*)*first->length);
		assert(first->array != NULL);

		first->next = lp->head;
		first->prev = NULL;
		first->next->prev = first;
		lp->head = first;

		first->start = 0;
		first->array[first->start] = item;

		first->count = 1;
		lp->count++;
	}

	//case 3 any other possibility
	else{
		current->start = current->start-1;
		if(current->start < 0){
			current->start = current->length-1;
		}
		current->array[current->start] = item;

		current->count++;
		lp->count++;
	}
}

//O(1)
//adds an item as the last item in the list
void addLast(LIST *lp, void *item){
	assert(lp != NULL);

	NODE *current = lp->tail;

	//case 1 list is empty
	if(current == NULL){
		NODE *only;
		only = malloc(sizeof(NODE));
		assert(only != NULL);

		only->length = 1;

		only->array = malloc(sizeof(void*)*only->length);
		assert(only->array != NULL);

		only->next = NULL;
		only->prev = NULL;
		lp->tail = only;
		lp->head = only;

		only->start = 0;
		only->array[only->start] = item;


		only->count = 1;
		lp->count++;
	}

	//case 2 node is full
	else if(current->count == current->length){
		NODE *last;
		last = malloc(sizeof(NODE));
		assert(last != NULL);

		last->length = 2*current->length;

		last->array = malloc(sizeof(void*) * last->length);
		assert(last->array != NULL);

		last->next = NULL;
		last->prev = lp->tail;
		last->prev->next = last;
		lp->tail = last;

		last->start = 0;
		last->array[last->start] = item;

		last->count = 1;
		lp->count++;
	}

	//case 3 any other possibility
	else{
		int x = (current->start+current->count)%current->length;
		current->array[x] = item;

		current->count++;
		lp->count++;
	}
}

//O(1)
//removes and returns the first item in the list
void *removeFirst(LIST *lp){
	assert(lp != NULL && lp->count >= 1);

	NODE *current = lp->head;
	
	//frees the empty node
	if(current->count == 0){
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(current->array);
		free(current);
	}
	
	NODE *curr = lp->head;
	void *item = curr->array[curr->start];
	curr->array[curr->start] = NULL;
	curr->start = (curr->start+1)%curr->length;
	
	curr->count--;
	lp->count--;
	
	return item;
}

//O(1)
//removes and returns the last item in the list
void *removeLast(LIST *lp){
	assert(lp != NULL && lp->count >= 1);

	NODE *current = lp->tail;

	//frees the empty node
	if(current->count == 0){
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(current->array);
		free(current);
	}
	
	NODE *curr = lp->tail;
	int x = (curr->start+curr->count-1)%curr->length;
	void *item = curr->array[x];
	curr->array[x] = NULL;
	
	curr->count--;
	lp->count--;
	
	return item;
}

//O(logn)
//returns the item at a given index in the list if the index is in the range
void *getItem(LIST *lp, int index){
	assert(lp != NULL);

	NODE *current = lp->head;

	while(index >= current->count){
		index -= current->count;
		current = current->next;
	}

	assert(index <= current->count);
	
	index = (index + current->start)%current->length;
	return current->array[index];
}

//O(logn)
//updates the item at a given index in the list if the index is in the range
void setItem(LIST *lp, int index, void *item){
	assert(lp != NULL);

	NODE *current = lp->head;

	while(index >= current->count){
		index -= current->count;
		current = current->next;
	}

	assert(index <= current->count);
	
	index = (index + current->start)%current->length;
	current->array[index] = item;
}
