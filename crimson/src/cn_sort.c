
#include <crimson/cn_sort.h>

static void
cn_swap_bits(void *first, void *second, size_t len)
{
    cn_raw_ptr   p = NULL;
    cn_pointer_t buf;

    p = cn_pointer_init(&buf, len / 8 + 1);
}

#define swap_i32(first, second) cn_swap_bits((first), (second), 32)

void
bubble_sort(int32_t *arr, size_t n)
{
    for (size_t i = n; i > 0; i--) {
        for (size_t j = 1; j < i; j++) {
            if (arr[j - 1] > arr[j]) {
                swap_i32(arr + j - 1, arr + j);
            }
        }
    }
}

void
insertion_sort(int32_t *arr, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        int32_t p = arr[i];
        size_t  j = i;
        while ((j > 0) && (arr[j - 1] > p)) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = p;
    }
}
