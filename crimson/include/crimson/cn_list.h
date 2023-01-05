
#ifndef CN_LIST_H_INCLUDE
#define CN_LIST_H_INCLUDE

#include <crimson/cn_alloc.h>
#include <crimson/cn_pointer.h>

typedef struct cn_list_s      cn_list_t;
typedef struct cn_list_node_s cn_list_node_t;
typedef struct cn_pointer_s   cn_list_data_t;

/// Embedded in the structure to form a chain list.
typedef struct cn_list_entry_s cn_list_entry_t;
struct cn_list_entry_s
{
    cn_list_entry_t *next;
};

struct cn_list_node_s
{
    cn_list_node_t *next;
    cn_list_data_t  data;
};

struct cn_list_s
{
    cn_list_node_t *head;
    cn_usize        len;
};

cn_list_t *
cn_list_new(cn_usize len, cn_usize size);
void
cn_list_drop(cn_list_t *list);

#endif // CN_LIST_H_INCLUDE
