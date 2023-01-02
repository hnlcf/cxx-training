#pragma once

#ifndef CSAPP_TYPES_H
#define CSAPP_TYPES_H

#ifdef __cpluscplus
extern "C"
{
#endif

    /** Custom re-define primitive integer, float and character types */

    /** Boolean value */
    typedef unsigned char s_bool;
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

    typedef unsigned char  s_u8;
    typedef unsigned short s_u16;
    typedef unsigned int   s_u32;
    typedef unsigned long  s_u64;
    typedef unsigned long  s_size;
    typedef char           s_i8;
    typedef short          s_i16;
    typedef int            s_i32;
    typedef long           s_i64;
    typedef float          s_f32;
    typedef double         s_f64;
    typedef char           s_char;
    typedef unsigned char  s_byte;
    typedef void          *s_ptr;

    /** Function prototype */
    typedef void (*s_clear_fn)(s_ptr data);

#define type_cast(type, var) ((type)(var))

#ifdef __cpluscplus
}
#endif

#endif // CSAPP_TYPES_H
