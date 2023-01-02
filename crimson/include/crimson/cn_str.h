
#ifndef CN_STR_H_INCLUDE
#define CN_STR_H_INCLUDE

#include <crimson/cn_type.h>

typedef struct cn_str_s cn_str_t;

/// String literal only at stack or global segement.
struct cn_str_s
{
    size_t    size;
    cn_uchar *data;
};

#define cn_new_str(str)                                                                            \
    {                                                                                              \
        sizeof(str) - 1, (cn_uchar *)(str)                                                         \
    }

#define cn_null_str                                                                                \
    {                                                                                              \
        0, NULL                                                                                    \
    }

#define cn_str_set(str, text)                                                                      \
    do {                                                                                           \
        (str)->len  = sizeof(text) - 1;                                                            \
        (str)->data = (cn_uchar *)text;                                                            \
    } while (0)

#define cn_str_clear(str)                                                                          \
    do {                                                                                           \
        (str)->len  = 0;                                                                           \
        (str)->data = NULL;                                                                        \
    } while (0)

#endif // CN_STR_H_INCLUDE
