#include <csapp/s_utils.h>

void
s_print_register(void)
{
    if ((DEBUG_PRINT_VERBOSE_SET & DEBUG_PRINT_REGISTER) == 0x0) {
        return;
    }

    cpu_register_t reg = g_cpu_reg;

    printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n",
           reg.rax,
           reg.rbx,
           reg.rcx,
           reg.rdx);
    printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n",
           reg.rsi,
           reg.rdi,
           reg.rbp,
           reg.rsp);
    printf("rip = %16lx\n", g_cpu_pc.rip);
    printf("CF = %u\tZF = %u\tSF = %u\tOF = %u\n",
           g_cpu_flag.CF,
           g_cpu_flag.ZF,
           g_cpu_flag.SF,
           g_cpu_flag.OF);
}

void
s_print_stack(void)
{
    if ((DEBUG_PRINT_VERBOSE_SET & DEBUG_PRINT_STACK) == 0x0) {
        return;
    }

    s_i32 n  = 10;
    s_u64 va = g_cpu_reg.rsp + n * 8;

    s_u64 *high;
    high = (s_u64 *)&g_physical_mem[s_va2pa(g_cpu_reg.rsp)];
    high = &high[n];

    s_i32  i;
    s_u64 *ptr;
    for (i = 0; i < 2 * n; ++i) {
        ptr = (s_u64 *)(high - i);
        printf("0x%16lx : %16lx", va, (s_u64)*ptr);

        if (i == n) {
            printf(" <== rsp");
        }
        printf("\n");
        va -= 8;
    }
}
