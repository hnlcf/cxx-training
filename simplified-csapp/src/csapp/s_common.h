#pragma once

#ifndef CSAPP_COMMON_H
#define CSAPP_COMMON_H

#ifdef __cpluscplus
extern "C"
{
#endif

#include <csapp/s_macros.h>
#include <csapp/s_string.h>

    /**
 * Printf wrapper when debug
 *
 * @param open_set debug flag
 * @param format format string
 * @param ... arguments
 * @return
 */
    S_API s_u64 debug_printf(s_u64 open_set, const char *format, ...);

    /**
 * Type converter, convert `uint32` to its equivalent float with rounding
 */
    S_API s_u32 uint2float(s_u32 u);

    /**
 * Convert string dec or hex to the integer bitmap
 *
 * @param str a decimal integer
 */
    S_API s_u64 string2uint(s_byte *str);
    S_API s_u64 string2uint_range(s_byte *str, s_i64 start, s_i64 end);

#ifdef __cpluscplus
}
#endif

#endif // CSAPP_COMMON_H
