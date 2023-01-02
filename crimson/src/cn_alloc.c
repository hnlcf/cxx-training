#include <crimson/cn_alloc.h>

cn_raw_ptr
cn_alloc(size_t size)
{
    cn_raw_ptr p = NULL;
    if (size) {
        p = malloc(size);
    }

    return p;
}

cn_raw_ptr
cn_calloc(size_t size)
{
    cn_raw_ptr p = cn_alloc(size);
    if (p != NULL) {
        cn_memzero(p, size);
    }

    return p;
}

cn_raw_ptr
cn_realloc(cn_raw_ptr mem, size_t size)
{
    if (mem == NULL) {
        return NULL;
    }

    cn_raw_ptr new_mem = NULL;
    if (size) {
        new_mem = realloc(mem, size);
    }

    cn_free(mem);
    return new_mem;
}
