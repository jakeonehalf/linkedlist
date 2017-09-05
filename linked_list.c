#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "linked_list.h"

/* Creates a new empty linked list. */
linked_list_t *ll_new(void) {
	// Allocate memory for linked list management node.
	linked_list_t *ll = malloc(sizeof(linked_list_t));

	// Head points to null.
	ll->head = NULL;

	// Tail points to null.
	ll->tail = NULL;

	// Allocate memory for mutex.
	pthread_mutex_t *ll_mutex = malloc(sizeof(pthread_mutex_t));

	// Initialize mutex.
	pthread_mutex_init(ll_mutex, NULL);

	// Point the structure's lock to the new mutex.
	ll->lock = (void *)ll_mutex;

	// Initial size is zero.
	ll->size = 0;

	// Return linked list management node.
	return ll;
}

/* Destroys the linked list and all associated data. */
void ll_destroy(linked_list_t *ll) {
	while (ll->size > 0) {
		free(ll_pop_head(ll));
	}

	// Free mutex.
	pthread_mutex_destroy(ll->lock);
	free(ll->lock);

	// Free the linked list.
	free(ll);
}

/* Returns the size of the linked list. */
size_t ll_size(linked_list_t *ll) {
	return ll->size;
}

static node_t *ll_partition(node_t *low, node_t *high, int (*compare_function)(void *first_data, void *second_data)) {
	node_t *i = low->prev;
	node_t *j;

	for (j = low; j != high; j = j->next) {
		if (compare_function(j->data, high->data) < 0) {
			i = (i == NULL) ? low : i->next;
			void *t = i->data;
			i->data = j->data;
			j->data = t;
		}
	}

	i = (i == NULL) ? low : i->next;
	void *t = i->data;
	i->data = high->data;
	high->data = t;

	return i;
}

static void ll_quick_sort(node_t* low, node_t* high, int (*compare_function)(void *first_data, void *second_data)) {
	// If high is not null, low is not point at high and low is not high's next.
	if ( high != NULL && low != high && low != high->next) {
		node_t *p = ll_partition(low, high, compare_function);
		ll_quick_sort(low, p->prev, compare_function);
		ll_quick_sort(p->next, high, compare_function);
	}
}

void ll_sort(linked_list_t *ll, int (*compare_function)(void *first_data, void *second_data)) {
	// Lock the linked list while being sorted.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Sort the linked list.
	ll_quick_sort(ll->head, ll->tail, compare_function);

	// Unlock the linked list now that the list is sorted.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);
}

/* Push data onto the tail of the provided linked list and adjust the tail. */
void ll_push_tail(linked_list_t *ll, void *data) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Corner case where there are no elements on the linked list.
	if (ll->size == 0) {
		// Make a new node at the tail pointer.
		ll->tail = malloc(sizeof(node_t));

		// Set the node to point to the data;
		ll->tail->data = data;

		// Ensure that the tail points to NULL.
		ll->tail->next = NULL;

		// Head and tail are the same.
		ll->head = ll->tail;

		// Ensure that the head points back at NULL.
		ll->head->prev = NULL;

		// Increase the size.
		ll->size++;

		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

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

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);
}

/* Push data onto the head of the provided linked list and adjust the head. */
void ll_push_head(linked_list_t *ll, void *data) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Corner case where there are no elements on the linked list.
	if (ll->size == 0) {
		// Make a new node at the head pointer.
		ll->head = malloc(sizeof(node_t));

		// Set the node to point to the data;
		ll->head->data = data;

		// Ensure that the head points back at NULL.
		ll->head->prev = NULL;

		// Head and tail are the same.
		ll->tail = ll->head;

		// Ensure the tail points at NULL.
		ll->tail->next = NULL;

		// Increase the size.
		ll->size++;

		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

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

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);
}

/* Push data into the list after the element found by using the provided compare function. */
void ll_push_after(linked_list_t *ll, void *data, bool (*compare_function)(void *cmd_data)) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Point at the head of the linked list.
	node_t *current = ll->head;

	// If the linked list is empty or there is only one node, push the data.
	if (ll->size <= 1) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		ll_push_tail(ll, data);

		return;
	}

	// If the head is the correct node.
	if (compare_function(current->data)) {
		// Make a new node.
		node_t *new_node = malloc(sizeof(node_t));

		// Point the new_node at the provided data.
		new_node->data = data;

		// Point the previous pointer at the current node.
		new_node->prev = current;

		// Point the next pointer to the current node's next node.
		new_node->next = current->next;

		// Point the next in list pointer back at the new node.
		current->next->prev = new_node;

		// Point the current next pointer at the new node.
		current->next = new_node;

		// Increase the size.
		ll->size++;

		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		return;
	}

	// While there are nodes in the linked list.
	while (current->next != NULL) {
		// If the provided compare_function returns true.
		if (compare_function(current->data)) {
			// Make a new node.
			node_t *new_node = malloc(sizeof(node_t));

			// Point the new_node at the provided data.
			new_node->data = data;

			// Point the previous pointer at the current node.
			new_node->prev = current;

			// Point the next pointer to the current node's next node.
			new_node->next = current->next;

			// Point the next in list pointer back at the new node.
			current->next->prev = new_node;

			// Point the current next pointer at the new node.
			current->next = new_node;

			// Increase the size.
			ll->size++;

			// Unlock the linked list.
			pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

			return;
		}

		// Point to the next node in the linked list.
		current = current->next;
	}

	// If the tail is the correct node.
	if (compare_function(current->data)) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		ll_push_tail(ll, data);

		return;
	}

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

	// If didn't find the node, just push it at the end.
	ll_push_tail(ll, data);
}

/* Push data into the list before the element found by using the provided compare function. */
void ll_push_before(linked_list_t *ll, void *data, bool (*compare_function)(void *cmd_data)) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Point at the head of the linked list.
	node_t *current = ll->head;

	// If the linked list is empty or there is only one node, push the data.
	if (ll->size <= 1) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		ll_push_head(ll, data);

		return;
	}

	// If the head is the correct node.
	if (compare_function(current->data)) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		ll_push_head(ll, data);

		return;
	}

	// While there are nodes in the linked list.
	while (current->next != NULL) {
		// If the provided compare_function returns true.
		if (compare_function(current->data)) {
			// Make a new node.
			node_t *new_node = malloc(sizeof(node_t));

			// Point the new_node at the provided data.
			new_node->data = data;

			// Point the next pointer at the current node.
			new_node->next = current;

			// Point the previous pointer to the current node's previous node.
			new_node->prev = current->prev;

			// Point the node that came before the current node at the new node.
			current->prev->next = new_node;

			// Point the current node back at the new node.
			current->prev = new_node;

			// Increase the size.
			ll->size++;

			// Unlock the linked list.
			pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

			return;
		}

		// Point to the next node in the linked list.
		current = current->next;
	}

	// If the tail is the correct node.
	if (compare_function(current->data)) {
		// Make a new node.
		node_t *new_node = malloc(sizeof(node_t));

		// Point the new_node at the provided data.
		new_node->data = data;

		// Point the next pointer at the current node.
		new_node->next = current;

		// Point the previous pointer to the current node's previous node.
		new_node->prev = current->prev;

		// Point the node that came before the current node at the new node.
		current->prev->next = new_node;

		// Point the current node back at the new node.
		current->prev = new_node;

		// Increase the size.
		ll->size++;

		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);
		return;
	}

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

	// If didn't find the node, just push it at the beginning.
	ll_push_head(ll, data);
}

/* Push data into the list at the specified index assuming a 0 indexed list.
 * This will shift the element at the specified index to be after the inserted node. */
void ll_push_at_index(linked_list_t *ll, void *data, int index) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Create a pointer to keep track of location in list.
	node_t *current = NULL;

	// If the linked list is empty or index is head, just push as the head.
	if (ll->size == 0 || index == 0) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		ll_push_head(ll, data);

		return;
	}

	// If the index is the last item in the list, just push as the tail.
	if (index == ll->size) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		ll_push_tail(ll, data);

		return;
	}

	if (index > 0) {
		// Point to the head node.
		current = ll->head;

		// Iterate over the list until reached the specified index.
		int i;
		for (i = 0; i < index; i++) {
			current = current->next;
		}
	} else {
		
		// Point to the tail node.
		current = ll->tail;

		// Traverse the list backwards until reached the specified index.
		int i;
		for (i = 1; i < 0 - (index); i++) {
			printf("%d\n", i);
			current = current->prev;
		}
	}

	// Make a new node to be inserted.
	node_t *new_node = malloc(sizeof(node_t));

	// Point the new node at the provided data.
	new_node->data = data;

	// Current node is in new node's spot, point at it.
	new_node->next = current;

	// Steal the current node's previous pointer.
	new_node->prev = current->prev;

	// Make the current node's previous node point at the new node.
	current->prev->next = new_node;

	// Current node is now looking back at new node.
	current->prev = new_node;

	// Increase the size.
	ll->size++;

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);
}

/* Remove the head of the provided linked list and adjust the head. */
void *ll_pop_head(linked_list_t *ll) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	void *data = NULL;

	// If the linked list is empty, return null.
	if (ll->size == 0) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		return NULL;
	}

	// Grab the data before doing anything with the head pointer.
	data = ll->head->data;

	// Corner case where there is only one element on the linked list.
	if (ll->size == 1) {
		// Free at the head pointer.
		free(ll->head);

		// The linked list is empty.
		ll->head = NULL;
		ll->tail = NULL;

		// Decrease the size.
		ll->size--;

		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

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

	// Unlock the linked list.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Return the data.
	return data;
}

/* Remove the tail of the provided linked list and adjust the tail. */
void *ll_pop_tail(linked_list_t *ll) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	void *data = NULL;

	// If the linked list is empty, return null.
	if (ll->size == 0) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		return NULL;
	}

	// Grab the data before doing anything with the tail pointer.
	data = ll->tail->data;

	// Corner case where there is only one element on the linked list.
	if (ll->size == 1) {
		// Free at the tail pointer.
		free(ll->tail);

		// The linked list is empty.
		ll->head = NULL;
		ll->tail = NULL;

		// Decrease the size.
		ll->size--;

		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

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

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

	// Return the data.
	return data;
}

/* Iterate through the list from head to find the requested node using the provided compare function. */
void *ll_pop_by(linked_list_t *ll, bool (*compare_function)(void *cmd_data)) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Point at the head of the linked list.
	node_t *current = ll->head;

	// If the linked list is empty, return null.
	if (ll->size == 0) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

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

			// Unlock the linked list.
			pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

			// Return the data;
			return data;
		}
	}

	// If the head is the correct node.
	if (compare_function(current->data)) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		// Remove node from the head of the linked list.
		return ll_pop_head(ll);
	}

	// While there are nodes in the linked list.
	while (current->next != NULL) {
		// If the provided compare_function returns true.
		if (compare_function(current->data)) {
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

			// Unlock the linked list.
			pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

			// Return the data.
			return data;
		}

		// Point to the next node in the linked list.
		current = current->next;
	}

	// If the tail is the correct node.
	if (compare_function(current->data)) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		// Remove node from the tail of the linked list.
		return ll_pop_tail(ll);
	}

	// Node wasn't found in the list.
	return NULL;
}

/* Remove the node at the specified index. A negative number traverses the list from the tail (-1 indexed). */
void *ll_pop_by_index(linked_list_t *ll, int index) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// If the specified index is the head node.
	if (index == 0 || index == (ll->size * -1)) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		return ll_pop_head(ll);
	}

	// If the specified index is the tail node.
	if (index == ll->size - 1 || index == -1) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		return ll_pop_tail(ll);
	}

	// If the index is beyond the size of the linked list.
	if (index >= ll->size) {
		// Unlock the linked list.
		pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

		return NULL;
	}

	void *data = NULL;

	node_t *current = NULL;

	if (index > 0) {
		// Point to the head node.
		current = ll->head;

		// Iterate over the list until reached the specified index.
		int i;
		for (i = 0; i != index; i++) {
			current = current->next;
		}
	} else {
		
		// Point to the tail node.
		current = ll->tail;

		// Traverse the list backwards until reached the specified index.
		int i;
		for (i = 0; i < 0 - index; i++) {
			current = current->prev;
		}
	}

	// Grab the data before doing anything with the pointer.
	data = current->data;

	// Point the previous node at the next in the list.
	current->prev->next = current->next;

	// Point the next node at the previous node in the list.
	current->next->prev = current->prev;

	// Node is ready to be deallocated.
	free(current);

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);

	// Return the data.
	return data;
}

/* Prints the information about all nodes in the linked list. Useful for debug information. */
static void ll_print_list_base(linked_list_t *ll, bool reversed) {
	// Lock the linked list to ensure mutual exclusion.
	pthread_mutex_lock((pthread_mutex_t *)ll->lock);

	// Point at the head (or tail if reversed) of the linked list.
	node_t *current = (reversed) ? ll->tail : ll->head;

	// Integer to track of which node we are on.
	int index = (reversed) ? ll->size - 1 : 0;

	// While we are still within the list.
	while(current != NULL) {

		// Print a table describing the address
		printf(	"Node %d:\n" 					// Print the index of the node we are at.
				"\tPrevious: %p\n"				// Print the address of the pointer to the previous node.
				"\tAddress: %p\n"				// Print the address of the pointer to the node.
				"\tData: %p\n"					// Print the address of the pointer to the data.
				"\tNext: %p\n",					// Print the address of the pointer to the next node.
				(reversed) ? index-- : index++,	// Increment (or decrement if reversed) the index after using.
				(void *)current->prev,
				(void *)current,
				(void *)current->data,
				(void *)current->next
			);

		// Point to the next (or previous if reversed) node in the list.
		current = (reversed) ? current->prev : current->next;
	}

	// Unlock the linked list.
	pthread_mutex_unlock((pthread_mutex_t *)ll->lock);
}

/* Variation function to allow for a singular or secondary parameter to reverse the print of the list. */
void var_ll_print_list(ll_print_list_args args) {
	// Point to the linked list provided in the arguments.
	linked_list_t *ll = args.ll;

	// If reversed is provided, set reversed to the provided value, otherwise assume it is false.
	bool reversed = args.reversed ? args.reversed : false;

	// Run the base function.
	ll_print_list_base(ll, reversed);
}