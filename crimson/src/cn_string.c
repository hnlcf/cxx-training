#include <crimson/cn_string.h>

static inline cn_string_t *
string_from_raw(cn_uchar *str, size_t len)
{
    cn_string_t *s = cn_string_with_capacity(2 * len);
    if (s == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        cn_array_push(&s->data, str + i);
    }

    return s;
}

cn_string_t *
cn_string_new()
{
    return cn_string_with_capacity(0);
}

cn_string_t *
cn_string_with_capacity(size_t capacity)
{
    cn_string_t *s = cn_alloc_type(cn_string_t);
    if (s == NULL) {
        return NULL;
    }

    if (!cn_array_init(&s->data, capacity, 1)) {
        cn_free(s);
        return NULL;
    }

    return s;
}

cn_string_t *
cn_string_from_raw(cn_uchar *str)
{
    size_t len = sizeof(str) - 1;
    return string_from_raw(str, len);
}

cn_string_t *
cn_string_from_str(cn_str_t *str)
{
    return string_from_raw(str->data, str->size);
}

size_t
cn_string_len(cn_string_t *s)
{
    return s->data.len;
}

cn_bool
cn_string_is_empty(cn_string_t *s)
{
    return cn_string_len(s) == 0;
}

void
cn_string_retake(cn_string_t *s)
{
    if (s != NULL) {
        cn_array_retake(&s->data);
    }
}

void
cn_string_dispatch(cn_string_t *s)
{
    if (s != NULL) {
        cn_free(s);
    }
}

void
cn_string_drop(cn_string_t *s)
{
    cn_string_retake(s);
    cn_string_dispatch(s);
}
