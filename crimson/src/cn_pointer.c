#include <crimson/cn_alloc.h>
#include <crimson/cn_pointer.h>

cn_raw_ptr
cn_pointer_init(cn_pointer_t *p, cn_usize size)
{
    if (p == NULL) {
        return NULL;
    }

    cn_raw_ptr ptr = NULL;

    ptr            = cn_alloc(size);
    if (ptr != NULL) {
        p->ptr  = ptr;
        p->size = size;
    }

    return ptr;
}

void
cn_pointer_retake(cn_pointer_t *p)
{
    if (p != NULL) {
        cn_free(p->ptr);
        p->size = 0;
    }
}
