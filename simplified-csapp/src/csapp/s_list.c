#include <csapp/s_list.h>


S_LOCAL S_INLINE void
list_node_create_null(s_list_node_t *node) {
    node->data     = NULL;
    node->elt_size = 0;
    node->next     = NULL;
    node->prev     = NULL;
}


S_LOCAL S_INLINE void
list_node_destroy(s_ptr node) {
    s_free(((s_list_node_t *) node)->data);
    s_free(node);
}


S_LOCAL S_INLINE void
list_set_null(s_list_t *list) {
    list->head       = NULL;
    list->trail      = NULL;
    list->elt_num    = 0;
    list->capacity   = 0;
    list->clear_func = NULL;
}


s_list_t *
s_list_create(s_size capacity, s_size elt_size) {
    s_list_t *list;

    list = s_alloc_new_type(s_list_t);
    s_return_val_if_null(list, NULL);

    list_set_null(list);
    s_list_init(list, capacity, elt_size);

    return list;
}


void
s_list_init(s_list_t *list, s_size capacity, s_size elt_size) {
    s_size         i;
    s_list_node_t *node, *curr;

    // Create head and trail nodes
    list_node_create_null(list->head);
    list_node_create_null(list->trail);

    // Create data nodes and connect each other
    curr = list->head;
    for (i = 0; i < capacity; i++) {
        node = s_list_node_create(elt_size);

        curr->next = node;
        node->prev = curr;
        curr       = node;
    }
    curr->next        = list->trail;
    list->trail->prev = curr;

    // Set field values
    list->capacity   = capacity;
    list->clear_func = list_node_destroy;
}


s_list_node_t *
s_list_node_create(s_size size) {
    s_list_node_t *node;
    s_byte        *buf;

    node = s_alloc_new_type(s_list_node_t);
    s_return_val_if_null(node, NULL);

    buf = s_malloc(size);
    s_return_val_if_null(buf, NULL);

    node->data     = buf;
    node->elt_size = size;

    return node;
}
