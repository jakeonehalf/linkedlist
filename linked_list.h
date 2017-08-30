#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct node {
	void *data;
	struct node *next;
	struct node *prev;
} node_t;

typedef struct linked_list {
	node_t *head;
	node_t *tail;
	size_t size;
} linked_list_t;

linked_list_t *ll_new(void);
void ll_destroy(linked_list_t *ll); 
size_t ll_size(linked_list_t *ll);
void ll_push_tail(linked_list_t *ll, void *data);
void ll_push_head(linked_list_t *ll, void *data);
void *ll_pop_head(linked_list_t *ll);
void *ll_pop_tail(linked_list_t *ll);
void *ll_pop_by(linked_list_t *ll, bool (*compare_function)(void *cmd_data));
void *ll_pop_by_index(linked_list_t *ll, int index);
void ll_print_list(linked_list_t *ll);

#endif //__LINKED_LIST_H__