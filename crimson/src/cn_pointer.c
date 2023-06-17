#include <crimson/cn_alloc.h>
#include <crimson/cn_pointer.h>

cn_bool
pointer_default_cmp(cn_pointer_t *p1, cn_pointer_t *p2)
{
    if (p1 == p2) {
        return TRUE;
    }
    if (p1->size != p2->size) {
        return FALSE;
    }

    cn_usize i;
    for (i = 0; i < p1->size; i++) {
        if (p1->ptr[i] != p2->ptr[i]) {
            i = UINT64_MAX;
            break;
        }
    }

    return i == UINT64_MAX ? FALSE : TRUE;
}

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

cn_bool
cn_pointer_clear(cn_pointer_t *p)
{
    if (p == NULL) {
        return FALSE;
    }

    cn_memzero(p->ptr, p->size);
    return TRUE;
}

cn_bool
cn_pointer_cmp(cn_pointer_t *p1, cn_pointer_t *p2, cn_cmp_fn cmp)
{
    return cmp(p1, p2);
}

cn_bool
cn_pointer_is_equal(cn_pointer_t *p1, cn_pointer_t *p2)
{
    return cn_pointer_cmp(p1, p2, (cn_cmp_fn)pointer_default_cmp);
}
