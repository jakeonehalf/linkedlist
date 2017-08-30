#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Remove the tail of the provided linked list and adjust the tail. */
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

/* Iterate through the list from head to find the requested node using the provided compare function. */
void *ll_pop_by(linked_list_t *ll, bool (*compare_function)(void *cmd_data)) {
	// Point at the head of the linked list.
	node_t *current = ll->head;

	// If the linked list is empty, return null.
	if (ll->size == 0) {
		return NULL;
	}

	// Corner case where there is only one node.
	if (ll->size == 1) {
		if(compare_function(current->data)) {
			// Grab the data before doing anything with the linked list.
			void *data = current->data;

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
	}

	// If the head is the correct node.
	if (compare_function(current->data)) {
		// Remove node from the head of the linked list.
		return ll_pop_head(ll);
	}

	// While there are nodes in the linked list.
	while(current->next != NULL) {
		// If the provided compare_function returns true.
		if(compare_function(current->data)) {
			// Grab the data before doing anything with the pointer.
			void *data = current->data;

			// The previous node now points to the current node's next node.
			current->prev->next = current->next;

			// The next node now points to the current node's previous node.
			current->next->prev = current->prev;

			// Now that the node is out of the linked list, free the node.
			free(current);

			// Decrease the size.
			ll->size--;

			// Return the data.
			return data;
		}

		// Point to the next node in the linked list.
		current = current->next;
	}

	// If the tail is the correct node.
	if (compare_function(current->data)) {
		// Remove node from the tail of the linked list.
		return ll_pop_tail(ll);
	}

	// Node wasn't found in the list.
	return NULL;
}

/* Remove the node at the specified index. */
void *ll_pop_by_index(linked_list_t *ll, int index) {
	// If the specified index is the head node.
	if (index == 0) {
		return ll_pop_head(ll);
	}

	// If the specified index is the tail node.
	if (index == ll->size - 1) {
		return ll_pop_tail(ll);
	}

	// If the index is beyond the size of the linked list.
	if (index >= ll->size) {
		return NULL;
	}

	// Point to the head node.
	node_t *current = ll->head;

	// Iterate over the list until reached the specified index.
	int i;
	for(i = 0; i != index; i++) {
		current = current->next;
	}

	// Grab the data before doing anything with the pointer.
	void *data = current->data;

	// Point the previous node at the next in the list.
	current->prev->next = current->next;

	// Point the next node at the previous node in the list.
	current->next->prev = current->prev;

	// Node is ready to be deallocated.
	free(current);

	// Return the data.
	return data;
}

/* Prints the information about all nodes in the linked list. Useful for debug information. */
void ll_print_list(linked_list_t *ll) {
	// Point at the head of the linked list.
	node_t *current = ll->head;

	// Integer to track of which node we are on.
	int index = 0;

	// While we are still within the list.
	while(current != NULL) {

		// Print a table describing the address
		printf(	"Node %d:\n" 			// Print the index of the node we are at.
				"\tAddress: %08x\n"		// Print the address of the pointer to the node.
				"\tData: %08x\n"		// Print the address of the pointer to the data.
				"\tPrevious: %08x\n"	// Print the address of the pointer to the previous node.
				"\tNext: %08x\n",		// Print the address of the pointer to the next node.
				index++,				// Increment the index after using.
				current,
				current->data,
				current->prev,
				current->next
			);

		// Point to the next node in the list.
		current = current->next;
	}
}