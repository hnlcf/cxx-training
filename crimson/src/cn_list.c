#include <crimson/cn_list.h>

#define list_node_alloc() cn_alloc_type(cn_list_node_t)

static inline cn_raw_ptr
list_data_init(cn_list_data_t *data, cn_usize size)
{
    return cn_pointer_init(data, size);
}

static inline void
list_data_retake(cn_list_data_t *data)
{
    // As the `data` is a part of struct node, so the data will be freed when
    // we free node. If free the data before, it will panic with double free.
    cn_pointer_retake(data);
}

static inline cn_list_node_t *
list_node_new(cn_usize size)
{
    cn_raw_ptr      p    = NULL;
    cn_list_node_t *node = NULL;

    node                 = list_node_alloc();
    if (node == NULL) {
        return NULL;
    }

    p = list_data_init(&node->data, size);
    if (p == NULL) {
        return NULL;
    }

    node->next = NULL;
    return node;
}

static inline void
list_node_drop(cn_list_node_t *list)
{
    if (list != NULL) {
        list_data_retake(&list->data);
        cn_free(list);
    }
}

cn_list_t *
cn_list_new(cn_usize len, cn_usize size)
{
    cn_list_t       *list   = NULL;
    cn_list_node_t  *head   = NULL;
    cn_list_node_t  *temp   = NULL;
    cn_list_node_t **p_temp = NULL;

    list                    = cn_alloc_type(cn_list_t);
    head                    = list_node_new(size);
    p_temp                  = &head->next;

    for (cn_usize i = 0; i < len; i++) {
        temp    = list_node_new(size);

        *p_temp = temp;
        p_temp  = &(temp->next);
    }

    list->head = head;
    list->len  = len;

    return list;
}

void
cn_list_drop(cn_list_t *list)
{
    if (list != NULL) {
        cn_list_node_t *node = NULL;
        cn_list_node_t *tmp  = NULL;

        node                 = list->head;
        while (node != NULL) {
            tmp = node->next;

            list_node_drop(node);
            node = tmp;
        }

        cn_free(list);
    }
}
