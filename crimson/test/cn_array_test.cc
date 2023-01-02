#include <crimson_test/cn_test_header.h>

TEST_CASE("[crimson-array]", "A dynamical vector with creating and freeing operations.")
{
    constexpr int kArrayCapacity = 10;

    SECTION("Allocate a new normal array")
    {
        cn_array_t *vec = cn_array_new(kArrayCapacity, sizeof(int));

        REQUIRE(vec->elt_size == sizeof(int));
        REQUIRE(vec->capacity == kArrayCapacity);
        REQUIRE(vec->len == 0);
        REQUIRE(vec->current != NULL);
        REQUIRE(vec->data.ptr != NULL);
        REQUIRE(vec->data.size == vec->capacity * vec->elt_size);

        cn_array_drop(vec);
    }

    SECTION("Initialize a normal array")
    {
        cn_array_t vec;
        cn_bool    is_succeed = cn_array_init(&vec, kArrayCapacity, sizeof(int));

        REQUIRE(is_succeed);
        REQUIRE(vec.elt_size == sizeof(int));
        REQUIRE(vec.capacity == kArrayCapacity);
        REQUIRE(vec.len == 0);
        REQUIRE(vec.current != NULL);
        REQUIRE(vec.data.ptr != NULL);
        REQUIRE(vec.data.size == vec.capacity * vec.elt_size);

        cn_array_retake(&vec);
    }

    SECTION("Initialize a null array")
    {
        cn_array_t *vec        = nullptr;
        cn_bool     is_succeed = cn_array_init(vec, kArrayCapacity, sizeof(int));

        REQUIRE_FALSE(is_succeed);
        REQUIRE(vec == nullptr);
    }

    SECTION("Retake resource owned by a normal array")
    {
        cn_array_t vec;
        cn_array_init(&vec, kArrayCapacity, sizeof(int));
        cn_array_retake(&vec);

        REQUIRE(vec.len == 0);
        REQUIRE(vec.capacity == 0);
        REQUIRE(vec.current == nullptr);
        REQUIRE(vec.elt_size == sizeof(int));
    }
}
