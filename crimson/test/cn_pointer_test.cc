#include <crimson_test/cn_test_header.h>

TEST_CASE("[crimson-pointer]", "A fat pointer allocated on heap")
{
    constexpr size_t kMemorySize = 4096;
    cn_pointer_t     ptr;

    cn_pointer_init(&ptr, kMemorySize);
    REQUIRE(ptr.size == kMemorySize);
    REQUIRE(ptr.ptr != NULL);

    cn_pointer_retake(&ptr);
    REQUIRE(ptr.size == 0);
    REQUIRE(ptr.ptr == NULL);
}

TEST_CASE("[cn-pointer-eq]", "Fat pointers' equation and comparison ")
{
    constexpr size_t kMemorySize = 4096;
    cn_pointer_t     p1, p2;

    cn_pointer_init(&p1, kMemorySize);
    cn_pointer_init(&p2, kMemorySize);

    REQUIRE(cn_pointer_is_equal(&p1, &p2) == 1);
}