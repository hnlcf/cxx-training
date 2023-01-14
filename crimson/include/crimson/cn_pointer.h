
#ifndef CN_POINTER_H_INCLUDE
#define CN_POINTER_H_INCLUDE

#include <crimson/cn_type.h>

typedef struct cn_pointer_s cn_pointer_t;

/// Fat pointer, only allocated on stack.
struct cn_pointer_s
{
    /// the memory address on heap.
    cn_raw_ptr ptr;

    /// the memory length in bytes.
    cn_usize size;
};

/// Allocate `size` bytes memory and initialize it for fat pointer.
cn_raw_ptr
cn_pointer_init(cn_pointer_t *p, cn_usize size);

/// Free all memory owned by fat pointer.
void
cn_pointer_retake(cn_pointer_t *p);


/// Compare two fat pointers with given cmp function.
cn_bool
cn_pointer_cmp(cn_pointer_t *p1, cn_pointer_t *p2, cn_cmp_fn cmp);

/// Compare whether two dat pointers are equal(include memory on heap).
cn_bool
cn_pointer_is_equal(cn_pointer_t *p1, cn_pointer_t *p2);

#endif // CN_POINTER_H_INCLUDE
