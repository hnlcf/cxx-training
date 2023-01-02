#include <csapp/s_cpu.h>

// lookup table
S_LOCAL const s_str_t lk_reg_name_list[72] = {
    s_str("%rax"),  s_str("%eax"),  s_str("%ax"),   s_str("%ah"),   s_str("%al"),   s_str("%rbx"),
    s_str("%ebx"),  s_str("%bx"),   s_str("%bh"),   s_str("%bl"),   s_str("%rcx"),  s_str("%ecx"),
    s_str("%cx"),   s_str("%ch"),   s_str("%cl"),   s_str("%rdx"),  s_str("%edx"),  s_str("%dx"),
    s_str("%dh"),   s_str("%dl"),   s_str("%rsi"),  s_str("%esi"),  s_str("%si"),   s_str("%sih"),
    s_str("%sil"),  s_str("%rdi"),  s_str("%edi"),  s_str("%di"),   s_str("%dih"),  s_str("%dil"),
    s_str("%rbp"),  s_str("%ebp"),  s_str("%bp"),   s_str("%bph"),  s_str("%bpl"),  s_str("%rsp"),
    s_str("%esp"),  s_str("%sp"),   s_str("%sph"),  s_str("%spl"),  s_str("%r8"),   s_str("%r8d"),
    s_str("%r8w"),  s_str("%r8b"),  s_str("%r9"),   s_str("%r9d"),  s_str("%r9w"),  s_str("%r9b"),
    s_str("%r10"),  s_str("%r10d"), s_str("%r10w"), s_str("%r10b"), s_str("%r11"),  s_str("%r11d"),
    s_str("%r11w"), s_str("%r11b"), s_str("%r12"),  s_str("%r12d"), s_str("%r12w"), s_str("%r12b"),
    s_str("%r13"),  s_str("%r13d"), s_str("%r13w"), s_str("%r13b"), s_str("%r14"),  s_str("%r14d"),
    s_str("%r14w"), s_str("%r14b"), s_str("%r15"),  s_str("%r15d"), s_str("%r15w"), s_str("%r15b"),
};

s_u64
s_cpu_reflect_register(s_byte *str) {
    // lookup table

    s_u64 reg_addr[72] = {
        (s_u64) & (g_cpu_reg.rax),  (s_u64) & (g_cpu_reg.eax),  (s_u64) & (g_cpu_reg.ax),
        (s_u64) & (g_cpu_reg.ah),   (s_u64) & (g_cpu_reg.al),   (s_u64) & (g_cpu_reg.rbx),
        (s_u64) & (g_cpu_reg.ebx),  (s_u64) & (g_cpu_reg.bx),   (s_u64) & (g_cpu_reg.bh),
        (s_u64) & (g_cpu_reg.bl),   (s_u64) & (g_cpu_reg.rcx),  (s_u64) & (g_cpu_reg.ecx),
        (s_u64) & (g_cpu_reg.cx),   (s_u64) & (g_cpu_reg.ch),   (s_u64) & (g_cpu_reg.cl),
        (s_u64) & (g_cpu_reg.rdx),  (s_u64) & (g_cpu_reg.edx),  (s_u64) & (g_cpu_reg.dx),
        (s_u64) & (g_cpu_reg.dh),   (s_u64) & (g_cpu_reg.dl),   (s_u64) & (g_cpu_reg.rsi),
        (s_u64) & (g_cpu_reg.esi),  (s_u64) & (g_cpu_reg.si),   (s_u64) & (g_cpu_reg.sih),
        (s_u64) & (g_cpu_reg.sil),  (s_u64) & (g_cpu_reg.rdi),  (s_u64) & (g_cpu_reg.edi),
        (s_u64) & (g_cpu_reg.di),   (s_u64) & (g_cpu_reg.dih),  (s_u64) & (g_cpu_reg.dil),
        (s_u64) & (g_cpu_reg.rbp),  (s_u64) & (g_cpu_reg.ebp),  (s_u64) & (g_cpu_reg.bp),
        (s_u64) & (g_cpu_reg.bph),  (s_u64) & (g_cpu_reg.bpl),  (s_u64) & (g_cpu_reg.rsp),
        (s_u64) & (g_cpu_reg.esp),  (s_u64) & (g_cpu_reg.sp),   (s_u64) & (g_cpu_reg.sph),
        (s_u64) & (g_cpu_reg.spl),  (s_u64) & (g_cpu_reg.r8),   (s_u64) & (g_cpu_reg.r8d),
        (s_u64) & (g_cpu_reg.r8w),  (s_u64) & (g_cpu_reg.r8b),  (s_u64) & (g_cpu_reg.r9),
        (s_u64) & (g_cpu_reg.r9d),  (s_u64) & (g_cpu_reg.r9w),  (s_u64) & (g_cpu_reg.r9b),
        (s_u64) & (g_cpu_reg.r10),  (s_u64) & (g_cpu_reg.r10d), (s_u64) & (g_cpu_reg.r10w),
        (s_u64) & (g_cpu_reg.r10b), (s_u64) & (g_cpu_reg.r11),  (s_u64) & (g_cpu_reg.r11d),
        (s_u64) & (g_cpu_reg.r11w), (s_u64) & (g_cpu_reg.r11b), (s_u64) & (g_cpu_reg.r12),
        (s_u64) & (g_cpu_reg.r12d), (s_u64) & (g_cpu_reg.r12w), (s_u64) & (g_cpu_reg.r12b),
        (s_u64) & (g_cpu_reg.r13),  (s_u64) & (g_cpu_reg.r13d), (s_u64) & (g_cpu_reg.r13w),
        (s_u64) & (g_cpu_reg.r13b), (s_u64) & (g_cpu_reg.r14),  (s_u64) & (g_cpu_reg.r14d),
        (s_u64) & (g_cpu_reg.r14w), (s_u64) & (g_cpu_reg.r14b), (s_u64) & (g_cpu_reg.r15),
        (s_u64) & (g_cpu_reg.r15d), (s_u64) & (g_cpu_reg.r15w), (s_u64) & (g_cpu_reg.r15b),
    };

    s_i32 i;
    for (i = 0; i < 72; ++i) {
        if (s_strcmp(str, lk_reg_name_list[i].data) == 0) {
            return reg_addr[i];
        }
    }

    printf("parse register %s error\n", (char *) str);
    exit(0);
}
