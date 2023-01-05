#include <crimson/cn_array.h>

static inline cn_bool
array_may_resize(cn_array_t *vec, cn_usize capacity)
{
    if (vec == NULL) {
        return FALSE;
    }

    cn_raw_ptr old_data = vec->data.ptr;
    cn_raw_ptr new_data = cn_pointer_init(&vec->data, capacity * vec->elt_size);
    cn_usize   num      = vec->current - old_data;

    if (new_data == NULL) {
        // TODO: Log failed to allocate memory.
        return FALSE;
    }

    // To avoid memory overlap, move the memory rather than copy.
    cn_memmove(new_data, old_data, num);
    cn_free(old_data);

    vec->capacity = capacity;
    vec->current  = new_data + num;

    return TRUE;
}

cn_array_t *
cn_array_new(cn_usize capacity, cn_usize elt_size)
{
    cn_array_t *vec = cn_alloc_type(cn_array_t);
    if (vec == NULL) {
        return NULL;
    }

    if (!cn_array_init(vec, capacity, elt_size)) {
        cn_free(vec);
        return NULL;
    }

    return vec;
}

cn_bool
cn_array_init(cn_array_t *vec, cn_usize capacity, cn_usize elt_size)
{
    if (vec == NULL) {
        return FALSE;
    }

    cn_usize   total = capacity * elt_size;
    cn_raw_ptr ptr   = cn_pointer_init(&vec->data, total);
    if (ptr == NULL) {
        // TODO: Log failed to allocate
        return FALSE;
    }

    vec->current  = ptr;
    vec->elt_size = elt_size;
    vec->capacity = capacity;
    vec->len      = 0;

    return TRUE;
}

void
cn_array_retake(cn_array_t *vec)
{
    if (vec != NULL) {
        cn_pointer_retake(&vec->data);
        vec->len      = 0;
        vec->capacity = 0;
        vec->current  = NULL;
    }
}

void
cn_array_drop(cn_array_t *vec)
{
    cn_array_retake(vec);
    cn_free(vec);
}

cn_bool
cn_array_is_empty(cn_array_t *vec)
{
    if (vec == NULL) {
        return FALSE;
    }

    if (vec->len == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

cn_bool
cn_array_is_full(cn_array_t *vec)
{
    if (vec == NULL) {
        return FALSE;
    }

    if (vec->len == vec->capacity) {
        return TRUE;
    } else {
        return FALSE;
    }
}

cn_bool
cn_array_clear(cn_array_t *vec)
{
    if (vec == NULL) {
        return FALSE;
    }

    vec->current = vec->data.ptr;
    vec->len     = 0;
    return TRUE;
}

cn_bool
cn_array_may_expand(cn_array_t *vec)
{
    if (cn_array_is_full(vec)) {
        return array_may_resize(vec, vec->capacity * 2);
    }
    return TRUE;
}

cn_bool
cn_array_push(cn_array_t *vec, const cn_any_ptr elt)
{
    if (!cn_array_may_expand(vec)) {
        // Vector is full and fail to expand
        return FALSE;
    }

    cn_memcpy(vec->current, elt, vec->elt_size);
    vec->current += vec->elt_size;
    vec->len += 1;

    return TRUE;
}

cn_bool
cn_array_may_shrink(cn_array_t *vec)
{
    if (vec == NULL) {
        return FALSE;
    }

    if (vec->len < vec->capacity / 4) {
        return array_may_resize(vec, vec->capacity / 2);
    }
    return TRUE;
}

cn_bool
cn_array_pop(cn_array_t *vec, cn_any_ptr ret_elt)
{
    if (cn_array_is_empty(vec)) {
        return FALSE;
    }

    if (!cn_array_may_shrink(vec)) {
        return FALSE;
    }

    vec->len -= 1;
    vec->current -= vec->elt_size;
    cn_memcpy(ret_elt, vec->current, vec->elt_size);

    return TRUE;
}

cn_bool
cn_array_remove(cn_array_t *vec, cn_usize index, cn_any_ptr ret_elt)
{
    if (vec == NULL) {
        return FALSE;
    }

    if (index >= vec->len) {
        // TODO: Log error with out of bounds
        return FALSE;
    }

    if (cn_array_is_empty(vec)) {
        // TODO: Log error with empty vector
        return FALSE;
    }

    // Update vector record fields
    vec->len -= 1;
    vec->current -= vec->elt_size;

    cn_usize   num  = vec->len;
    cn_usize   size = vec->elt_size;
    cn_raw_ptr elt  = vec->data.ptr + index * size;

    // Delete specified element and move the tail
    cn_memcpy(ret_elt, elt, size);
    cn_memcpy(elt, elt + size, (num - index) * size);

    return TRUE;
}
