#ifndef TEST_C_CACHELINE_TEST_H
#define TEST_C_CACHELINE_TEST_H

/// Test whether read/write on cacheline is atomic or not.
///
/// Only single cacheline is atomic.
void
test_cacheline_atomic(int offset);

#endif // TEST_C_CACHELINE_TEST_H