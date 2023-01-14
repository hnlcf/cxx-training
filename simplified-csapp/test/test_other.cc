#include <csapp_test/test_autoheader.h>

s_u64
fib(s_u64 n)
{
    if (n == 0) {
        return 0;
    }

    s_u64 p = 1;
    s_u64 g = 1;

    for (s_u64 i = 2; i < n; ++i) {
        g = p + g;
        p = g - p;
    }

    return g;
}

TEST_CASE("Fibonacci number", "[fib]")
{
    s_u64 expect[11] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };

    for (s_u64 i = 0; i < 11; ++i) {
        REQUIRE(fib(i) == expect[i]);
    }
}

TEST_CASE("Math macros", "[macro]")
{
    SECTION("MAX")
    {
        int p      = 3;
        int b      = 4;
        int actual = std::max(p++, b);
        REQUIRE(actual == 4);
    }

    SECTION("ABS")
    {
        int data[] = {
            0,
            1,
            99,
            -1,
        };
        int expect[] = {
            0,
            1,
            99,
            1,
        };
        for (int i = 0; i < 4; ++i) {
            REQUIRE(std::abs(data[i]) == expect[i]);
        }
    }
}

TEST_CASE("Create custom string", "[core][string]")
{
    SECTION("Static allocate")
    {
        s_str_t str;

        const char *arr[8] = {
            "",
            "\0",
            "1",
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaa"
            "aaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "gbuiaeorsngbuiprqnqghprioehjqrgefqewhinqegdjingrqenuqrgefinodsgqewjri9"
            "oghqeriogqrgehiq"
            "rgq"
            "n",
            "%^$#(&*^%^ED%^D^&RFV^%&%^&&)()&UI)&*)GR^YFUO&*OUJPOP?<:LJK&TVGRBP(Y*)"
            "Y*H*(H*H)P(U)(Y("
            "P*)"
            ")",
            "345547389267428105564578151689567456161459801648564145751",
            "][\\]=-]\\=]-=_+_<>?<';",
        };

        for (int i = 0; i < 8; ++i) {
            str = s_str(arr[i]);

            REQUIRE(str.len == sizeof(arr[i]) - 1);
            REQUIRE(str.remain == 0);
            REQUIRE(str.is_heap == 0);

            for (size_t j = 0; j < str.len; ++j) {
                REQUIRE(str.data[j] == arr[i][j]);
            }
        }
    }

    SECTION("Dynamic allocate")
    {
        s_str_t *str;

        for (int i = 0; i < 1000; ++i) {
            str = s_str_new(1024);
            REQUIRE_FALSE(str == NULL);

            s_str_free(str);
        }
    }
}

TEST_CASE("Create dynamic array", "[core][array]")
{
    s_array_t *arr = s_array_new(TRUE, FALSE, sizeof(s_i32), 4);

    REQUIRE(arr->data != NULL);
    REQUIRE(arr->elt_size == 4);
    REQUIRE(arr->len == 0);
    REQUIRE(arr->capacity == 4);
    REQUIRE(arr->trail_zero == TRUE);
    REQUIRE(arr->clear == FALSE);
    REQUIRE(arr->clear_fn == NULL);

    s_array_free(arr, TRUE);
}
