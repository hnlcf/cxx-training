

#ifndef CSAPP_ARRAY_H
#define CSAPP_ARRAY_H

#include <csapp/s_alloc.h>
#include <csapp/s_macros.h>

typedef struct s_array_s s_array_t;

struct s_array_s
{
    s_byte *data;           // the pointer to the element data
    s_size  elt_size;       // the size of one element in bytes
    s_size  len;            // the number of elements
    s_size  capacity;       // the max number of elements array can hold
    s_bool  trail_zero : 1; // TRUE if array should have a extra element at the end
                            // which is set to 0
    s_bool clear : 1;       // TRUE if all elements should be automatically set to 0
                            // when allocated
    s_clear_fn clear_fn;    // the function of clear an element of array
};

s_array_t *
s_array_new(s_bool trail_zero, s_bool clear, s_size elt_size, s_size capacity);
s_byte *
s_array_free(s_array_t *arr, s_bool free_data);
s_array_t *
s_array_size_new(s_bool trail_zero, s_bool clear, s_size elt_size, s_size capacity);
void
s_array_may_expand(s_array_t *arr, s_size len);
s_array_t *
s_array_append_vals(s_array_t *arr, const s_ptr data, s_size len);

#define s_array_index(arr, type, idx) (((type *)(arr)->data)[(idx)])
#define s_array_elt_len(arr, idx)     ((arr)->elt_size * (idx))
#define s_array_elt_pos(arr, idx)     ((arr)->data + s_array_elt_len((arr), (idx)))
#define s_array_elt_set(arr, pos, value, len)                                                      \
    s_memset(s_array_elt_pos((arr), (pos)), (value), s_array_elt_len((arr), (len)))
#define s_array_elt_zero(arr, pos, len)                                                            \
    s_memzero(s_array_elt_pos((arr), (pos)), s_array_elt_len((arr), (len)))
#define s_array_set_trail_zero(arr)                                                                \
    do {                                                                                           \
        if ((arr)->trail_zero) {                                                                   \
            s_array_elt_zero((arr), (arr)->len, 1);                                                \
        }                                                                                          \
    } while (0)

#endif // CSAPP_ARRAY_H
