#include <csapp/s_common.h>

/**
 * Wrapper of stdio printf, controlled by the debug verbose bit set
 */
s_u64
debug_printf(s_u64 open_set, const char *format, ...)
{
    if ((open_set & DEBUG_PRINT_VERBOSE_SET) == 0x0) {
        return 0x1;
    }

    // implement of stdio printf()
    va_list p_arg;
    va_start(p_arg, format);
    vfprintf(stderr, format, p_arg);
    va_end(p_arg);

    return 0x0;
}

// convert s_u32 to its float
s_u32
uint2float(s_u32 u)
{
    if (u == 0x00000000) {
        return 0x00000000;
    }
    // must be NORMALIZED
    // counting the position of highest 1: u[n]
    s_i32 n = 31;
    while (0 <= n && (((u >> n) & 0x1) == 0x0)) {
        n = n - 1;
    }
    s_u32 e, f;
    //    seee eeee efff ffff ffff ffff ffff ffff
    // <= 0000 0000 1111 1111 1111 1111 1111 1111
    if (u <= 0x00ffffff) {
        // no need rounding
        s_u32 mask = 0xffffffff >> (32 - n);
        f          = (u & mask) << (23 - n);
        e          = n + 127;
        return (e << 23) | f;
    }
    // >= 0000 0001 0000 0000 0000 0000 0000 0000
    else {
        // need rounding
        // expand to 64 bit for situations like 0xffffffff
        s_u64 a = 0;
        a += u;
        // compute g, r, s
        s_u32 g = (a >> (n - 23)) & 0x1; // Guard bit, the lowest bit of the result
        s_u32 r = (a >> (n - 24)) & 0x1; // Round bit, the highest bit to be removed
        s_u32 s = 0x0;                   // Sticky bit, the OR of remaining bits in the
        // removed part (low)
        for (int j = 0; j < n - 24; ++j) {
            s = s | ((u >> j) & 0x1);
        }
        // compute carry
        a = a >> (n - 23);
        // 0    1    ?    ... ?
        // [24] [23] [22] ... [0]
        /* Rounding Rules
        +-------+-------+-------+-------+
        |   G   |   R   |   S   |       |
        +-------+-------+-------+-------+
        |   0   |   0   |   0   |   +0  | round down
        |   0   |   0   |   1   |   +0  | round down
        |   0   |   1   |   0   |   +0  | round down
        |   0   |   1   |   1   |   +1  | round up
        |   1   |   0   |   0   |   +0  | round down
        |   1   |   0   |   1   |   +0  | round down
        |   1   |   1   |   0   |   +1  | round up
        |   1   |   1   |   1   |   +1  | round up
        +-------+-------+-------+-------+
    carry = R & (G | S) by K-Map
    */
        if ((r & (g | s)) == 0x1) {
            a = a + 1;
        }
        // check carry
        if ((a >> 23) == 0x1) {
            // 0    1    ?    ... ?
            // [24] [23] [22] ... [0]
            f = a & 0x007fffff;
            e = n + 127;
            return (e << 23) | f;
        } else if ((a >> 23) == 0x2) {
            // 1    0    0    ... 0
            // [24] [23] [22] ... [0]
            e = n + 1 + 127;
            return (e << 23);
        }
    }
    // inf as default error
    return 0x7f800000;
}

// covert string to s_i64
s_u64
string2uint(s_byte *str)
{
    return string2uint_range(str, 0, -1);
}

// start: starting index inclusive
// end: ending index inclusive
s_u64
string2uint_range(s_byte *str, s_i64 start, s_i64 end)
{
    if (end == -1) {
        end = (s_i64)s_strlen(str) - 1;
    }

    s_u64 uv       = 0;
    s_i8  sign_bit = 0; // 0 - positive; 1 - negative

    // DFA: deterministic finite automata to scan string and get value
    s_i8 state = 0;

    for (s_i64 i = start; i <= end; ++i) {
        char ch = (char)str[i];

        if (state == 0) {
            if (ch == '0') {
                state = 1;
                uv    = 0;
                continue;
            } else if ('1' <= ch && ch <= '9') {
                state = 2;
                uv    = ch - '0';
                continue;
            } else if (ch == '-') {
                state    = 3;
                sign_bit = 1;
                continue;
            } else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                // skip leading spaces
                state = 0;
                continue;
            } else {
                goto fail;
            }
        } else if (state == 1) {
            // check zero
            if ('0' <= ch && ch <= '9') {
                state = 2;
                uv    = uv * 10 + ch - '0';
                continue;
            } else if (ch == 'x') {
                state = 4;
                continue;
            } else if (ch == ' ') {
                state = 6;
                continue;
            } else {
                goto fail;
            }
        } else if (state == 2) {
            // dec number
            if ('0' <= ch && ch <= '9') {
                state    = 2;
                s_u64 pv = uv;
                uv       = uv * 10 + ch - '0';
                // maybe overflow
                if (pv > uv) {
                    printf("(s_u64)%s overflow: cannot convert\n", (char *)str);
                    goto fail;
                }
                continue;
            } else if (ch == ' ') {
                state = 6;
                continue;
            } else {
                goto fail;
            }
        } else if (state == 3) {
            // negative
            if (ch == '0') {
                state = 1;
                continue;
            } else if ('1' <= ch && ch <= '9') {
                state = 2;
                uv    = ch - '0';
                continue;
            } else {
                goto fail;
            }
        } else if (state == 4 || state == 5) {
            // hex number
            if ('0' <= ch && ch <= '9') {
                state    = 5;
                s_u64 pv = uv;
                uv       = uv * 16 + ch - '0';
                // maybe overflow
                if (pv > uv) {
                    printf("(s_u64)%s overflow: cannot convert\n", (char *)str);
                    goto fail;
                }
                continue;
            } else if ('a' <= ch && ch <= 'f') {
                state    = 5;
                s_u64 pv = uv;
                uv       = uv * 16 + ch - 'a' + 10;
                // maybe overflow
                if (pv > uv) {
                    printf("(s_u64)%s overflow: cannot convert\n", (char *)str);
                    goto fail;
                }
                continue;
            } else if (state == 5 && (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')) {
                state = 6;
                continue;
            } else {
                goto fail;
            }
        } else {
            if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                // skip tailing spaces
                state = 6;
                continue;
            } else {
                goto fail;
            }
        }
    }

    if (sign_bit == 0) {
        return uv;
    } else {
        if ((uv & 0x8000000000000000) != 0) {
            printf("(s_i64)%s: signed overflow: cannot convert\n", (char *)str);
            exit(0);
        }
        s_i64 sv = -1 * (s_i64)uv;
        return *((s_u64 *)&sv);
    }

fail:
    printf("type converter: <%s> cannot be converted to integer\n", (char *)str);
    exit(0);
}
