# Linked List
This is a simple doubly linked list implementation for use in C projects.

## Functionality

### ll_new(void)
This function initializes a management node for a linked list and returns a pointer to the node.

### ll_destroy(linked_list_t * ll)
This function destroys the linked list and all associated data. All data pointers supplied are freed upon calling this function.

### ll_size(linked_list_t * ll)
This function returns the number of nodes in the linked list. This can be retrieved by calling the linked_list_t.size parameter of the linked list management node structure.

### ll_sort(linked_list_t *ll, int (*compare_function)(void *first_data, void *second_data))
This function sorts the provided linked list in place using quick sort with the supplied compare function. The compare function **MUST** return an integer and accept two void pointers as passed parameters. The compare function must return a negative number if the second parameter is larger than the first, a zero if equal or a positive number if the first parameter is larger than the second.

### ll_push_tail(linked_list_t * ll, void * data)
This function pushes the data pointer onto the tail of the provided linked list and adjusts the tail pointer.

### ll_push_head(linked_list_t * ll, void * data)
This function pushes the data pointer onto the head of the provided linked list and adjusts the head pointer.

### ll_push_after(linked_list_t *ll, void *data, bool (*compare_function)(void *cmd_data))
**-- Unimplemented --**

This function will push after the specified node based on the compare function.

*Need to determine functionality if node with compare is not found*

### ll_push_before(linked_list_t *ll, void *data, bool (*compare_function)(void *cmd_data))
**-- Unimplemented --**

This function will push before the specified node based on the compare function.

*Need to determine functionality if node with compare is not found*

### ll_push_at_index(linked_list_t *ll, void *data, int index)
**-- Unimplemented --**
This function will push at the specified index and adjust the list.

### ll_pop_head(linked_list_t * ll)
This function returns the data of the head node and adjusts the head pointer to point at the next node.

### ll_pop_tail(linked_list_t * ll)
This function returns the data of the tail node and adjusts the tail pointer to point at the previous node.

### ll_pop_by(linked_list_t * ll, bool (*compare_function)(void *cmd_data))
This function returns the data of the first node that a true is returned from the supplied compare function. The compare function **MUST** return a boolean and accept a void pointer as a passed parameter. The node is removed from the list and the list is adjusted.

### ll_pop_by_index(linked_list_t * ll, int index)
This function returns the specified node based on the provided index. 
* This is based on a 0 index (0 is head).
* Optionally, can use negative indexing (-1 is tail) to traverse backwards.

### ll_print_list(linked_list_t * ll, (optionally) bool reversed)
This function prints for each node:
  * The address of the node,
  * The address of the data,
  * The address of the previous node,
  * The address of the next node

The reversed parameter is option, but adding supplying reversed as true will print the list in reverse.
  
This is useful for debugging.

## Version History
### v1.0:
* Initial creation of doubly linked list with head and tail pointers in management node.
* Can push and pop from either end of the linked list.
* Can destroy entire linked list, including all data supplied to the list.
* Can iterate through list to find a specific node based on its data.
