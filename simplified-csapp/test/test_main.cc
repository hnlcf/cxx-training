#include <test_autoheader.h>

cpu_register_t g_cpu_reg;
cpu_flag_t     g_cpu_flag;
cpu_pc_t       g_cpu_pc;

int
main(int argc, const char **argv)
{
    int result = Catch::Session().run(argc, argv);

    return result;
}
