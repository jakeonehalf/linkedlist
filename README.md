# Linked List
This is a simple doubly linked list implementation for use in C projects.

## Functionality

### ll_new(void)
This function initializes a management node for a linked list and returns a pointer to the node.

### ll_destroy(linked_list_t * ll)
This function destroys the linked list and all associated data. All data pointers supplied are freed upon calling this function.

### ll_size(linked_list_t * ll)
This function returns the number of nodes in the linked list. This can be retrieved by calling the linked_list_t.size parameter of the linked list management node structure.

### ll_push_tail(linked_list_t * ll, void * data)
This function pushes the data pointer onto the tail of the provided linked list and adjusts the tail pointer.

### ll_push_head(linked_list_t * ll, void * data)
This function pushes the data pointer onto the head of the provided linked list and adjusts the head pointer.

### ll_pop_head(linked_list_t * ll)
This function returns the data of the head node and adjusts the head pointer to point at the next node.

### ll_pop_tail(linked_list_t * ll)
This function returns the data of the tail node and adjusts the tail pointer to point at the previous node.

### ll_pop_by(linked_list_t * ll, bool (*compare_function)(void *cmd_data))
This function returns the data of the first node that a true is returned from the supplied compare function. The compare function **MUST** return a boolean and accept a void pointer as a passed parameter. The node is removed from the list and the list is adjusted.

### ll_print_list(linked_list_t * ll)
This function prints for each node:
  * The address of the node,
  * The address of the data,
  * The address of the previous node,
  * The address of the next node
This is useful for debugging.

## Version History
### v1.0:
* Initial creation of doubly linked list with head and tail pointers in management node.
* Can push and pop from either end of the linked list.
* Can destroy entire linked list, including all data supplied to the list.
* Can iterate through list to find a specific node based on its data.
