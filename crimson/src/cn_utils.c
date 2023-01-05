
#include <crimson/cn_utils.h>

void
print_array(cn_any_ptr arr, cn_usize n)
{
    char     fmt[10] = "%d ";
    int32_t *p_arr   = (int32_t *)arr;

    printf("[ ");
    for (cn_usize i = 0; i < n; ++i) {
        printf(fmt, p_arr[i]);
    }
    printf("]\n");
}

void
input_array(cn_any_ptr *arr, cn_usize *n)
{
    char     fmt[10] = "%d ";
    int32_t *p_arr;

    scanf("%lu", n);
    p_arr = (int32_t *)malloc((*n) * sizeof(int32_t));

    for (cn_usize i = 0; i < *n; ++i) {
        scanf(fmt, p_arr + i);
    }
    *arr = p_arr;
}
