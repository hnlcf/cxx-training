

#ifndef CSAPP_LIST_H
#define CSAPP_LIST_H

#include <csapp/s_alloc.h>
#include <csapp/s_macros.h>

typedef struct s_list_s      s_list_t;
typedef struct s_list_node_s s_list_node_t;

struct s_list_node_s
{
    s_list_node_t *prev;
    s_list_node_t *next;
    s_byte        *data;     // the data pointer
    s_size         elt_size; // the size of an element in bytes
};

struct s_list_s
{
    s_list_node_t *head;       // the first node
    s_list_node_t *trail;      // the last node
    s_size         elt_num;    // the number of list nodes
    s_size         capacity;   // the number of elements to be allocated
    s_clear_fn     clear_func; // the memory free function for list node
};

s_list_t *
s_list_create(s_size capacity, s_size elt_size);
void
s_list_init(s_list_t *list, s_size capacity, s_size elt_size);
s_list_node_t *
s_list_node_create(s_size size);

#endif // CSAPP_LIST_H
