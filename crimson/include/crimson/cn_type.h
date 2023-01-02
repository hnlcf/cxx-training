
#ifndef CN_TYPE_H_INCLUDE
#define CN_TYPE_H_INCLUDE

#include <crimson/cn_config.h>

enum cn_bool_e
{
    FALSE = 0,
    TRUE  = 1,
};
typedef enum cn_bool_e cn_bool;

typedef char               cn_byte;
typedef unsigned char      cn_uchar;
typedef long long          cn_int;
typedef unsigned long long cn_uint;

typedef float  cn_float;
typedef double cn_double;

typedef char       *cn_raw_ptr;
typedef void       *cn_any_ptr;
typedef const void *cn_const_ptr;

typedef void (*cn_func)(cn_any_ptr data, cn_any_ptr other);
typedef void (*cn_void_func)();
typedef void (*cn_free_func)(cn_any_ptr data);
typedef void (*cn_print_func)(cn_uchar *str);
typedef void (*cn_log_func)(cn_uchar *log_domain);

#endif // CN_TYPE_H_INCLUDE
