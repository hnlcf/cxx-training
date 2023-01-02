
#ifndef CN_SORT_H_INCLUDE
#define CN_SORT_H_INCLUDE

#include <crimson/cn_pointer.h>

/// Insertion Sort -- input-sensitive algorithm.
///
/// @complexity
/// Time:
/// 	- Best: `O(n)`
///     - Worst: `O(n^2)`
///     - Average: `O(n^2)`
/// Space:
///     - `O(1)`
void
insertion_sort(int32_t *arr, size_t n);

void
bubble_sort(int32_t *arr, size_t n);

#endif // CN_SORT_H_INCLUDE
