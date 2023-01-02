#pragma once

#ifndef CSAPP_ALLOC_H
#define CSAPP_ALLOC_H

#ifdef __cpluscplus
extern "C"
{
#endif

#include <csapp/s_config.h>

    /**
 * Allocate `size` bytes of memory
 *
 * @param size the number of bytes to allocate
 * @return a pointer to allocated memory
 */
    S_API s_ptr s_malloc(s_size size);

    /**
 * Reallocate the memory that `mem` point to, so that it now has `size` bytes
 * space
 *
 * @param mem the memory to reallocate
 * @param size the new size of memory in bytes
 * @return the new address of the allocated memory
 */
    S_API s_ptr s_realloc(s_ptr mem, s_size size);

#define s_alloc_new_type(type) ((type *)s_malloc(sizeof(type)))

#define s_free(ptr)                                                                                \
    do {                                                                                           \
        free(ptr);                                                                                 \
        (ptr) = NULL;                                                                              \
    } while (0)

#define s_return_val_if_fail(compare, value)                                                       \
    do {                                                                                           \
        if (!(compare)) {                                                                          \
            return (value);                                                                        \
        }                                                                                          \
    } while (0)

#define s_return_val_if_null(ptr, value)                                                           \
    do {                                                                                           \
        if ((ptr) == NULL) {                                                                       \
            return (value);                                                                        \
        }                                                                                          \
    } while (0)

#define s_memcpy(dst, src, len)   memcpy((s_ptr)(dst), (const s_ptr)(src), (s_size)(len))
#define s_memset(ptr, value, len) memset((s_ptr)(ptr), (s_i32)(value), (s_size)(len))
#define s_memzero(ptr, len)       s_memset(ptr, 0, len)

#ifdef __cpluscplus
}
#endif

#endif // CSAPP_ALLOC_H
