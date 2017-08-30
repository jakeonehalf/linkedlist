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

typedef struct {
	linked_list_t *ll;
	bool reversed;
} ll_print_list_args;

#define ll_print_list(...) var_ll_print_list((ll_print_list_args){__VA_ARGS__});

linked_list_t *ll_new(void);
void ll_destroy(linked_list_t *ll); 
size_t ll_size(linked_list_t *ll);
void ll_push_tail(linked_list_t *ll, void *data);
void ll_push_head(linked_list_t *ll, void *data);
void ll_push_after(linked_list_t *ll, void *data, bool (*compare_function)(void *cmd_data));
void ll_push_before(linked_list_t *ll, void *data, bool (*compare_function)(void *cmd_data));
void ll_push_at_index(linked_list_t *ll, void *data, int index);
void *ll_pop_head(linked_list_t *ll);
void *ll_pop_tail(linked_list_t *ll);
void *ll_pop_by(linked_list_t *ll, bool (*compare_function)(void *cmd_data));
void *ll_pop_by_index(linked_list_t *ll, int index);

void ll_print_list_base(linked_list_t *ll, bool reversed);
void var_ll_print_list(ll_print_list_args args);

#endif //__LINKED_LIST_H__