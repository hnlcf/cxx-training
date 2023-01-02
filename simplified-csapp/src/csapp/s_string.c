#include <csapp/s_string.h>

S_LOCAL void
s_str_init(s_str_t *str, s_size size) {
    s_str_may_expand(str, size);
    s_memset(str->data, 0, str->len);
}

s_str_t *
s_str_new(s_size size) {
    s_str_t *str;

    str = s_alloc_new_type(s_str_t);
    s_return_val_if_null(str, NULL);

    str->data    = NULL;
    str->len     = 0;
    str->remain  = 0;
    str->is_heap = TRUE;

    s_str_init(str, size);
    return str;
}

void
s_str_free(s_str_t *str) {
    if (!s_str_is_dynamic(str)) {
        return;
    }

    s_free(str->data);
    s_free(str);
}


void
s_str_may_expand(s_str_t *str, s_size len) {
    if (len > str->remain) {
        str->remain = len + 1;
        str->data   = s_realloc(str->data, (str->len + str->remain));
    }
}

s_bool
s_str_is_dynamic(s_str_t *str) {
    if (str->is_heap) {
        return TRUE;
    }

    return FALSE;
}
