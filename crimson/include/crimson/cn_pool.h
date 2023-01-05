
#ifndef CN_POOL_H_INCLUDE
#define CN_POOL_H_INCLUDE

#include <crimson/cn_list.h>
#include <crimson/cn_pointer.h>

typedef struct cn_pool_s       cn_pool_t;
typedef struct cn_pool_large_s cn_pool_large_t;
typedef struct cn_pool_data_s  cn_pool_data_t;

/// Larger chunk for pool
struct cn_pool_large_s
{
    cn_pool_large_t *next;
    cn_pointer_t     alloc;
};

/// Data of memory pool with general size
struct cn_pool_data_s
{
    cn_raw_ptr last; // start of unused memory in current chunk
    cn_raw_ptr end;  // end of current memory dochunk
    cn_u64     failed;

    cn_list_entry_t link; // link to next memory node
};

struct cn_pool_s;

#endif // CN_POOL_H_INCLUDE
