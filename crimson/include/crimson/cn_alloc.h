
#ifndef CN_ALLOC_H_INCLUDE
#define CN_ALLOC_H_INCLUDE

#include <crimson/cn_config.h>
#include <crimson/cn_type.h>

/// Allocate `size` bytes memory.
cn_raw_ptr
cn_alloc(size_t size);

/// Allocate `size` bytes memory and initialize to zero.
cn_raw_ptr
cn_calloc(size_t size);

/// Re-allocate the memory block addressed at `mem` to `size` bytes.
cn_raw_ptr
cn_realloc(cn_raw_ptr mem, size_t size);

#define cn_alloc_type(type) (type *)cn_alloc(sizeof(type))
#define cn_free(ptr)                                                                               \
    do {                                                                                           \
        free(ptr);                                                                                 \
        (ptr) = NULL;                                                                              \
    } while (0)

#define cn_memcpy(dst, src, len)  memcpy((dst), (src), (len))
#define cn_memmove(dst, src, len) memmove((dst), (src), (len))
#define cn_memset(ptr, ch, len)   memset((ptr), (ch), (len))
#define cn_memzero(ptr, len)      cn_memset(ptr, 0, len)

#define CN_ALLOC_ALIGN_SIZE (1 << 3)
#define CN_ALLOC_BLOCK_SIZE (1 << 12)

#endif // CN_ALLOC_H_INCLUDE
