#include <csapp/s_array.h>

typedef enum
{
    FREE_BLOCK       = (1 << 0),
    PRESERVE_WRAPPER = (1 << 1),
} array_free_flag_e;

S_LOCAL s_byte *
array_free(s_array_t *arr, array_free_flag_e flag)
{
    s_return_val_if_null(arr, NULL);

    s_ptr buf;
    buf = arr->data;

    if (flag & FREE_BLOCK) {
        if (arr->clear_fn) {
            s_size i;
            for (i = 0; i < arr->len; i++) {
                arr->clear_fn(s_array_elt_pos(arr, i));
            }
        }

        s_free(buf);
    }

    if (flag & PRESERVE_WRAPPER) {
        arr->data     = NULL;
        arr->len      = 0;
        arr->capacity = 0;
    } else {
        s_free(arr);
    }

    return buf;
}

s_array_t *
s_array_new(s_bool trail_zero, s_bool clear, s_size elt_size, s_size capacity)
{
    s_return_val_if_fail(elt_size > 0, NULL);

    return s_array_size_new(trail_zero, clear, elt_size, capacity);
}

s_byte *
s_array_free(s_array_t *arr, s_bool free_data)
{
    array_free_flag_e flag;

    flag = (free_data ? FREE_BLOCK : 0);

    return array_free(arr, flag);
}

s_array_t *
s_array_size_new(s_bool trail_zero, s_bool clear, s_size elt_size, s_size capacity)
{
    s_return_val_if_fail(elt_size > 0, NULL);

    s_array_t *arr;

    arr = s_alloc_new_type(s_array_t);
    s_return_val_if_null(arr, NULL);

    arr->data       = NULL;
    arr->elt_size   = elt_size;
    arr->len        = 0;
    arr->capacity   = 0;
    arr->trail_zero = trail_zero;
    arr->clear      = clear;
    arr->clear_fn   = NULL;

    s_array_may_expand(arr, capacity);

    return arr;
}

void
s_array_may_expand(s_array_t *arr, s_size len)
{
    s_size new_len;

    new_len = arr->len + len + arr->trail_zero;

    if (new_len > arr->capacity) {
        arr->data = s_realloc(arr->data, s_array_elt_len(arr, new_len));

        if (arr->clear) {
            s_memzero(s_array_elt_pos(arr, arr->capacity),
                      s_array_elt_len(arr, new_len - arr->capacity));
        }

        arr->capacity = (arr->trail_zero ? new_len - 1 : new_len);
    }
}

s_array_t *
s_array_append_vals(s_array_t *arr, const s_ptr data, s_size len)
{
    s_return_val_if_null(arr, NULL);

    if (len == 0) {
        return arr;
    }

    s_array_may_expand(arr, len);

    s_memcpy(s_array_elt_pos(arr, arr->len), data, s_array_elt_len(arr, len));

    arr->len += len;

    s_array_set_trail_zero(arr);

    return arr;
}
