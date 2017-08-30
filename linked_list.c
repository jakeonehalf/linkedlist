#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/* Creates a new empty linked list. */
linked_list_t *ll_new(void) {
	// Allocate memory for linked list management node.
	linked_list_t *ll = malloc(sizeof(linked_list_t));

	// Head points to null.
	ll->head = NULL;

	// Tail points to null.
	ll->tail = NULL;

	// Initial size is zero.
	ll->size = 0;

	// Return linked list management node.
	return ll;
}

/* Destroys the linked list and all associated data. */
void ll_destroy(linked_list_t *ll) {
	// Iterate through the list and destroy the data and nodes.
	while(ll->head->next != NULL) {
		// Change head to point to the next node.
		ll->head = ll->head->next;

		// Free the previous head's data.
		free(ll->head->prev->data);

		// Free the previous head.
		free(ll->head->prev);
	}

	// Free the tail's data.
	free(ll->tail->data);

	// Free the tail.
	free(ll->tail);

	// Free the linked list.
	free(ll);
}

/* Returns the size of the linked list. */
size_t ll_size(linked_list_t *ll) {
	return ll->size;
}

/* Push data onto the tail of the provided linked list and adjust the tail. */
void ll_push_tail(linked_list_t *ll, void *data) {
	// Corner case where there are no elements on the linked list.
	if(ll->size == 0) {
		// Make a new node at the tail pointer.
		ll->tail = malloc(sizeof(node_t));

		// Set the node to point to the data;
		ll->tail->data = data;

		// Head and tail are the same.
		ll->head = ll->tail;

		// Increase the size.
		ll->size++;

		return;
	}

	// Make a new node at the end.
	ll->tail->next = malloc(sizeof(node_t));

	// Set the node to point to the data;
	ll->tail->next->data = data;

	// Set the node to point back at the previous tail.
	ll->tail->next->prev = ll->tail;

	// New node is the new tail, so next node is null.
	ll->tail->next->next = NULL;

	// Set the tail to be the new node.
	ll->tail = ll->tail->next;

	// Increase the size.
	ll->size++;
}

/* Push data onto the head of the provided linked list and adjust the head. */
void ll_push_head(linked_list_t *ll, void *data) {
	// Corner case where there are no elements on the linked list.
	if(ll->size == 0) {
		// Make a new node at the tail pointer.
		ll->head = malloc(sizeof(node_t));

		// Set the node to point to the data;
		ll->head->data = data;

		// Head and tail are the same.
		ll->tail = ll->head;

		// Increase the size.
		ll->size++;

		return;
	}

	// Make a new node at the front.
	ll->head->prev = malloc(sizeof(node_t));

	// Set the node to point to the data.
	ll->head->prev->data = data;

	// Set the node to point to the previous head.
	ll->head->prev->next = ll->head;

	// New node is the new head, so previous node is null.
	ll->head->prev->prev = NULL;
	
	// Set the head to be the new node.

	ll->head = ll->head->prev;

	// Increase the size.
	ll->size++;
}

/* Remove the head of the provided linked list and adjust the head. */
void *ll_pop_head(linked_list_t *ll) {
	void *data = NULL;

	// If the linked list is empty, return null.
	if(ll->size == 0) {
		return NULL;
	}

	// Grab the data before doing anything with the head pointer.
	data = ll->head->data;

	// Corner case where there is only one element on the linked list.
	if(ll->size == 1) {
		// Free at the head pointer.
		free(ll->head);

		// The linked list is empty.
		ll->head = NULL;
		ll->tail = NULL;

		// Decrease the size.
		ll->size--;

		// Return the data;
		return data;
	}
	
	// Head's next is the new head.
	ll->head->next->prev = NULL;

	// Keep track of the new head.
	node_t *new_head = ll->head->next;

	// Free the head pointer.
	free(ll->head);

	// Point at the new head pointer.
	ll->head = new_head;

	// Decrease the size.
	ll->size--;

	// Return the data.
	return data;
}

void *ll_pop_tail(linked_list_t *ll) {
	void *data = NULL;

	// If the linked list is empty, return null.
	if(ll->size == 0) {
		return NULL;
	}

	// Grab the data before doing anything with the tail pointer.
	data = ll->tail->data;

	// Corner case where there is only one element on the linked list.
	if(ll->size == 1) {
		// Free at the tail pointer.
		free(ll->tail);

		// The linked list is empty.
		ll->head = NULL;
		ll->tail = NULL;

		// Decrease the size.
		ll->size--;

		// Return the data;
		return data;
	}

	// Tail's previous is the new tail.
	ll->tail->prev->next = NULL;

	// Keep track of the new tail.
	node_t *new_tail = ll->tail->prev;

	// Free the tail pointer.
	free(ll->tail);

	// Point at the new tail pointer.
	ll->tail = new_tail;

	// Decrease the size.
	ll->size--;

	// Return the data.
	return data;
}
