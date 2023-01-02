#pragma once

#ifndef CSAPP_STRING_H
#define CSAPP_STRING_H

#ifdef __cpluscplus
extern "C"
{
#endif

#include <csapp/s_alloc.h>
#include <csapp/s_macros.h>

    typedef struct s_str_s s_str_t;

    /** String object stored in stack or heap*/
    struct s_str_s
    {
        s_byte *data;
        s_size  len;         // size of already used
        s_size  remain;      // size of the rest
        s_bool  is_heap : 1; // allocate is_heap, 0 if in stack, 1 if in heap
    };

/** All following macros only create string literal stored in stack */
#define s_str(str)                                                                                 \
    {                                                                                              \
        (s_byte *)(str), sizeof(str) - 1, 0, FALSE                                                 \
    }

#define s_null_str                                                                                 \
    {                                                                                              \
        NULL, 0, 0, FALSE                                                                          \
    }

#define s_str_set(str, text)                                                                       \
    do {                                                                                           \
        (str)->data    = (uchar *)(text);                                                          \
        (str)->len     = sizeof(text) - 1;                                                         \
        (str)->free    = 0;                                                                        \
        (str)->is_heap = FALSE;                                                                    \
    } while (0)

#define s_str_null(str)                                                                            \
    do {                                                                                           \
        (str)->data    = NULL;                                                                     \
        (str)->len     = 0;                                                                        \
        (str)->free    = 0;                                                                        \
        (str)->is_heap = FALSE;                                                                    \
    } while (0)

/** String literal operations */
#define s_strcmp(s1, s2)   strcmp((const s_char *)(s1), (const s_char *)(s2))
#define s_strlen(s)        strlen((const s_char *)(s))
#define s_strcpy(dst, src) s_memcpy((dst), (src), s_strlen((src)))

    /**
 * Manually create a string which can hold `size` bytes data stored in heap
 *
 * @param size the capacity of string in bytes
 * @return the pointer of string
 */
    S_API s_str_t *s_str_new(s_size size);

    /**
 * Free the string manually allocated in heap
 *
 * @param str the string pointer
 */
    S_API void s_str_free(s_str_t *str);

    /**
 * Expand string's capacity if `len` less than remain of `str`
 *
 * @param str the string pointer
 * @param len the num that string need to remain in bytes
 */
    S_API void s_str_may_expand(s_str_t *str, s_size len);

    /**
 * Determine is_heap of string
 *
 * @param str the pointer of string to be judged
 * @return TRUE if allocated in heap(dynamic), otherwise FALSE
 */
    S_API s_bool s_str_is_dynamic(s_str_t *str);

#ifdef __cpluscplus
}
#endif

#endif // CSAPP_STRING_H
