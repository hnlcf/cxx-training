
#ifndef CN_ARRAY_H_INCLUDE
#define CN_ARRAY_H_INCLUDE

#include <crimson/cn_alloc.h>
#include <crimson/cn_pointer.h>

/// A contiguous growable array.
typedef struct cn_array_s cn_array_t;

struct cn_array_s
{
    /// the size of element
    cn_usize elt_size;

    /// the number of elements in array
    cn_usize len;

    /// the number of elements array can hold
    cn_usize capacity;

    /// the fat pointer to a heap memory chunk stores all elements
    cn_pointer_t data;

    /// the pointer to the end address of array
    cn_raw_ptr current;
};

/// Create a new array with specified capacity and size of element stored.
cn_array_t *
cn_array_new(cn_usize capacity, cn_usize elt_size);

/// Initialize a specified array with capacity and size of element stored.
cn_bool
cn_array_init(cn_array_t *vec, cn_usize capacity, cn_usize elt_size);

/// Clear the array and release its resources(heap memory).
void
cn_array_retake(cn_array_t *vec);

/// Release resources and destroy the array object.
void
cn_array_drop(cn_array_t *vec);

/// Determine whether the array is empty.
cn_bool
cn_array_is_empty(cn_array_t *vec);

/// Determine whether the array is full.
cn_bool
cn_array_is_full(cn_array_t *vec);

/// Clear all elements of the array without releasing resources.
cn_bool
cn_array_clear(cn_array_t *vec);

/// Expand the array to double of array capacity if it is full now.
cn_bool
cn_array_may_expand(cn_array_t *vec);

/// Shrink the array to half of array capacity if length less than quarter of capacity.
cn_bool
cn_array_may_shrink(cn_array_t *vec);

/// Append an element to the back of the array.
///
/// The newly added element is copied byte by byte from `elt` to the end of the array.
cn_bool
cn_array_push(cn_array_t *vec, const cn_any_ptr elt);

/// Remove the last element from the array and returns TRUE, or FALSE if it is empty.
///
/// The removed element is returned by `ret_elt`, the pointer passed in.
cn_bool
cn_array_pop(cn_array_t *vec, cn_any_ptr ret_elt);

/// Remove the element specified index from the array and return it by `ret_elt`.
cn_bool
cn_array_remove(cn_array_t *vec, cn_usize index, cn_any_ptr ret_elt);

typedef struct cn_stack_s cn_stack_t;
struct cn_stack_s
{
    cn_array_t *vec;
    cn_bool (*push)(cn_stack_t *st, const cn_any_ptr elt);
};

#endif // CN_ARRAY_H_INCLUDE
