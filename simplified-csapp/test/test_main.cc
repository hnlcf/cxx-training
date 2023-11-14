#include <csapp_test/test_autoheader.h>

s_cpu_register_t g_cpu_reg;
s_cpu_flag_t   g_cpu_flag;
s_cpu_pc_t       g_cpu_pc;

int
main(int argc, const char **argv)
{
    int result = Catch::Session().run(argc, argv);

    return result;
}
