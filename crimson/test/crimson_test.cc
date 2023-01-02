#include <crimson_test/cn_test_header.h>

int
main(int argc, char *argv[])
{
    int result = Catch::Session().run(argc, argv);

    return result;
}
