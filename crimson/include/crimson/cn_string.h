
#ifndef CN_STRING_H_INCLUDE
#define CN_STRING_H_INCLUDE

#include <crimson/cn_array.h>
#include <crimson/cn_str.h>

typedef struct cn_string_s cn_string_t;

/// Growable string object allocated as `Vec<u8>` on stack.
struct cn_string_s
{
    cn_array_t data;
};

cn_string_t *
cn_string_new();

cn_string_t *
cn_string_with_capacity(size_t capacity);

cn_string_t *
cn_string_from_raw(cn_uchar *str);

cn_string_t *
cn_string_from_str(cn_str_t *str);

size_t
cn_string_len(cn_string_t *s);

cn_bool
cn_string_is_empty(cn_string_t *s);

void
cn_string_retake(cn_string_t *s);

void
cn_string_dispatch(cn_string_t *s);

void
cn_string_drop(cn_string_t *s);

#endif // CN_STRING_H_INCLUDE
