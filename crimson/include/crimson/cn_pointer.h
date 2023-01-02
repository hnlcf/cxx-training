
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
    size_t size;
};

/// Initialize `size` bytes memory for fat pointer.
cn_raw_ptr
cn_pointer_init(cn_pointer_t *p, size_t size);

/// Free all memory owned by fat pointer.
void
cn_pointer_retake(cn_pointer_t *p);

#endif // CN_POINTER_H_INCLUDE
