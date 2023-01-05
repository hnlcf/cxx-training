
#ifndef CN_UTILS_H_INCLUDE
#define CN_UTILS_H_INCLUDE

#include <crimson/cn_type.h>

/// Output all elements in array to `stdout`.
///
/// @param arr: pointer to array
/// @param n: size of array
void
print_array(cn_any_ptr arr, cn_usize n);

/// Read input array size `n` and all elements from `stdin`.
///
/// @param arr: pointer to pointer to array
/// @param n: pointer to size of array
void
input_array(cn_any_ptr *arr, cn_usize *n);

#endif // CN_UTILS_H_INCLUDE
