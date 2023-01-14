

#ifndef CSAPP_MACROS_H
#define CSAPP_MACROS_H

/** Control flags */
#ifndef S_DEBUG
#define S_ENABLE_DEBUG
#else
#define S_ENABLE_DEBUG
#endif

/** Compiler optimizations and features */
#define S_API
#define S_INLINE     inline
#define S_LOCAL      static
#define S_GLOBAL_VAR extern /* global variable */

#define S_UNUSED(x)   ((void)(x))
#define S_ATTR(opt)   __attribute__(opt)
#define S_ATTR_UNUSED S_ATTR(unused)

#ifndef __cplusplus
/**
 * Utils
 */
#undef MAX
#define MAX(a, b)                                                                                  \
    ({                                                                                             \
        const typeof(a) _a = (a);                                                                  \
        const typeof(b) _b = (b);                                                                  \
        _a > _b ? _a : _b;                                                                         \
    })
#undef MIN
#define MIN(a, b)                                                                                  \
    ({                                                                                             \
        const typeof(a) _a = (a);                                                                  \
        const typeof(b) _b = (b);                                                                  \
        _a < _b ? _a : _b;                                                                         \
    })
#undef ABS
#define ABS(a)                                                                                     \
    ({                                                                                             \
        const typeof(a) _a = (a);                                                                  \
        _a < 0 ? -_a : _a;                                                                         \
    })
#endif

/**
 * Debug Signal Flags
 */
#ifdef S_ENABLE_DEBUG

#define DEBUG_CACHE_DETAILS (0x10)
#define DEBUG_MMU           (0x20)
#define DEBUG_LINKER        (0x40)
#define DEBUG_LOADER        (0x80)

// Debug print flags
#define DEBUG_PRINT_VERBOSE_SET (0x7)
#define DEBUG_PRINT_REGISTER    (0x2)
#define DEBUG_PRINT_STACK       (0x4)
#define DEBUG_PRINT_CACHESET    (0x8)
#define DEBUG_PRINT_INST_CYCLE  (0x1)
#define DEBUG_PRINT_INST_PARSE  (0x8)

// do page walk
#define DEBUG_ENABLE_PAGE_WALK (0)

// use sram cache for memory access
#define DEBUG_ENABLE_SRAM_CACHE (0)
#endif

#endif // CSAPP_MACROS_H
