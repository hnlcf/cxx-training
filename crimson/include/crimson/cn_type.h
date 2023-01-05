
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
typedef char               cn_i8;
typedef short              cn_i16;
typedef int                cn_i32;
typedef long long          cn_i64;
typedef unsigned char      cn_u8;
typedef unsigned short     cn_u16;
typedef unsigned int       cn_u32;
typedef unsigned long long cn_u64;

typedef cn_i64 cn_isize;
typedef cn_u64 cn_usize;

typedef float  cn_f32;
typedef double cn_f64;

typedef char            *cn_raw_ptr;
typedef void            *cn_any_ptr;
typedef const cn_any_ptr cn_const_ptr;

typedef void (*cn_fn)(cn_any_ptr data, cn_any_ptr other);
typedef void (*cn_void_fn)();
typedef cn_bool (*cn_cmp_fn)(cn_any_ptr p1, cn_any_ptr p2);
typedef cn_bool (*cn_eq_fn)(cn_any_ptr p1, cn_any_ptr p2);
typedef void (*cn_drop_fn)(cn_any_ptr data);
typedef void (*cn_print_fn)(cn_uchar *str);
typedef void (*cn_log_fn)(cn_uchar *log_domain);

#endif // CN_TYPE_H_INCLUDE
