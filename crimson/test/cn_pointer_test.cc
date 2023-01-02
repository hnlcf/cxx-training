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