#include <csapp/s_alloc.h>

s_ptr
s_malloc(s_size size)
{
    if (size) {
        s_ptr mem;

        mem = malloc(size);
        if (mem) {
            return mem;
        }
    }

    return NULL;
}

s_ptr
s_realloc(s_ptr mem, s_size size)
{
    s_ptr new_mem;

    if (size) {
        new_mem = realloc(mem, size);

        if (new_mem) {
            return new_mem;
        }
    }

    s_free(mem);

    return NULL;
}
