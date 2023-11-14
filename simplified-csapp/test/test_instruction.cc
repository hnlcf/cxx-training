#include <csapp_test/test_autoheader.h>

void
print_separate_line()
{
    printf("\n<<-----------------------------------------------------------------"
           "-----------");
    printf("----------------->>\n");
}

TEST_CASE("Instruction parse", "[inst][parse]")
{
    SECTION("Parse oprands")
    {
        s_str_t opd_case[11] = {
            s_str("$0x1234"),
            s_str("%rax"),
            s_str("0xabcd"),
            s_str("(%rsp)"),
            s_str("0xabcd(%rsp)"),
            s_str("(%rsp,%rbx)"),
            s_str("0xabcd(%rsp,%rbx)"),
            s_str("(,%rbx,8)"),
            s_str("0xabcd(,%rbx,8)"),
            s_str("(%rsp,%rbx,8)"),
            s_str("0xabcd(%rsp,%rbx,8)"),
        };

        print_separate_line();
        printf("rax %p\n", &(g_cpu_reg.rax));
        printf("rsp %p\n", &(g_cpu_reg.rsp));
        printf("rbx %p\n", &(g_cpu_reg.rbx));

        for (auto i : opd_case) {
            s_inst_operand_t od;
            s_inst_parse_operand(&i, &od);
            printf("\n%s\n", (char *)i.data);

            printf("od enum type: %x\n", od.type);
            printf("od imm: %lx\n", od.imm);
            printf("od reg1: %lx\n", od.reg1);
            printf("od reg2: %lx\n", od.reg2);
            printf("od scal: %lx\n", od.scal);
        }
        print_separate_line();
    }
}

TEST_CASE("Execute instruction", "[inst][handle]")
{
    SECTION("Handle add function call and computation")
    {
        uint64_t init_reg_info[8] = {
            0xabcd,         // 0. rax
            0x8000670,      // 1. rbx
            0x8000670,      // 2. rcx
            0x12340000,     // 3. rdx
            0x7ffffffee208, // 4. rsi
            0x1,            // 5. rdi
            0x7ffffffee110, // 6. rbp
            0x7ffffffee0f0, // 7. rsp
        };

        // init state
        g_cpu_reg.rax         = init_reg_info[0];
        g_cpu_reg.rbx         = init_reg_info[1];
        g_cpu_reg.rcx         = init_reg_info[2];
        g_cpu_reg.rdx         = init_reg_info[3];
        g_cpu_reg.rsi         = init_reg_info[4];
        g_cpu_reg.rdi         = init_reg_info[5];
        g_cpu_reg.rbp         = init_reg_info[6];
        g_cpu_reg.rsp         = init_reg_info[7];
        g_cpu_flag.flag_value = 0;

        s_dram_write64bits(s_va2pa(0x7ffffffee110), 0x0000000000000000);
        s_dram_write64bits(s_va2pa(0x7ffffffee108), 0x0000000000000000);
        s_dram_write64bits(s_va2pa(0x7ffffffee100), 0x0000000012340000);
        s_dram_write64bits(s_va2pa(0x7ffffffee0f8), 0x000000000000abcd);
        s_dram_write64bits(s_va2pa(0x7ffffffee0f0), 0x0000000000000000);

        s_str_t assembly[15] = {
            s_str("push   %rbp"),             // 0
            s_str("mov    %rsp,%rbp"),        // 1
            s_str("mov    %rdi,-0x18(%rbp)"), // 2
            s_str("mov    %rsi,-0x20(%rbp)"), // 3
            s_str("mov    -0x18(%rbp),%rdx"), // 4
            s_str("mov    -0x20(%rbp),%rax"), // 5
            s_str("add    %rdx,%rax"),        // 6
            s_str("mov    %rax,-0x8(%rbp)"),  // 7
            s_str("mov    -0x8(%rbp),%rax"),  // 8
            s_str("pop    %rbp"),             // 9
            s_str("retq"),                    // 10
            s_str("mov    %rdx,%rsi"),        // 11: start
            s_str("mov    %rax,%rdi"),        // 12
            s_str("callq  0x00400000"),       // 13
            s_str("mov    %rax,-0x8(%rbp)"),  // 14
        };
        // copy to physical memory
        for (int i = 0; i < 15; ++i) {
            s_dram_write_inst(s_va2pa(i * 0x40 + 0x00400000), assembly + i);
        }
        g_cpu_pc.rip = INST_MAX_NUM_STR_LEN * sizeof(char) * 11 + 0x00400000;

        s_print_register();
        for (int i = 0; i < 15; i++) {
            print_separate_line();

            s_instruction_cycle();
            s_print_register();
            s_print_stack();
        }

        // gdb state ret from func
        bool match = false;
        match      = (g_cpu_reg.rax == 0x1234abcd) && (g_cpu_reg.rbx == 0x8000670) &&
                (g_cpu_reg.rcx == 0x8000670) && (g_cpu_reg.rdx == 0xabcd) &&
                (g_cpu_reg.rsi == 0x12340000) && (g_cpu_reg.rdi == 0xabcd) &&
                (g_cpu_reg.rbp == 0x7ffffffee110) && (g_cpu_reg.rsp == 0x7ffffffee0f0);

        REQUIRE(match == true);

        match = (s_dram_read64bits(s_va2pa(0x7ffffffee110)) == 0x0000000000000000) &&
                (s_dram_read64bits(s_va2pa(0x7ffffffee108)) == 0x000000001234abcd) &&
                (s_dram_read64bits(s_va2pa(0x7ffffffee100)) == 0x0000000012340000) &&
                (s_dram_read64bits(s_va2pa(0x7ffffffee0f8)) == 0x000000000000abcd) &&
                (s_dram_read64bits(s_va2pa(0x7ffffffee0f0)) == 0x0000000000000000);

        REQUIRE(match == true);
    }

    SECTION("Handle sum recursive call condition")
    {
        uint64_t init_reg_info[8] = {
            0x8000630,      // 0. rax
            0x0,            // 1. rbx
            0x8000650,      // 2. rcx
            0x7ffffffee328, // 3. rdx
            0x7ffffffee318, // 4. rsi
            0x1,            // 5. rdi
            0x7ffffffee230, // 6. rbp
            0x7ffffffee220, // 7. rsp
        };

        // init state
        g_cpu_reg.rax         = init_reg_info[0];
        g_cpu_reg.rbx         = init_reg_info[1];
        g_cpu_reg.rcx         = init_reg_info[2];
        g_cpu_reg.rdx         = init_reg_info[3];
        g_cpu_reg.rsi         = init_reg_info[4];
        g_cpu_reg.rdi         = init_reg_info[5];
        g_cpu_reg.rbp         = init_reg_info[6];
        g_cpu_reg.rsp         = init_reg_info[7];
        g_cpu_flag.flag_value = 0;

        s_dram_write64bits(s_va2pa(0x7ffffffee230), 0x0000000008000650);
        s_dram_write64bits(s_va2pa(0x7ffffffee228), 0x0000000000000000);
        s_dram_write64bits(s_va2pa(0x7ffffffee220), 0x00007ffffffee310);

        s_str_t assembly[19] = {
            s_str("push   %rbp"),            // 0
            s_str("mov    %rsp,%rbp"),       // 1
            s_str("sub    $0x10,%rsp"),      // 2
            s_str("mov    %rdi,-0x8(%rbp)"), // 3
            s_str("cmpq   $0x0,-0x8(%rbp)"), // 4
            s_str("jne    0x400200"),        // 5: jump to 8
            s_str("mov    $0x0,%eax"),       // 6
            s_str("jmp    0x400380"),        // 7: jump to 14
            s_str("mov    -0x8(%rbp),%rax"), // 8
            s_str("sub    $0x1,%rax"),       // 9
            s_str("mov    %rax,%rdi"),       // 10
            s_str("callq  0x00400000"),      // 11
            s_str("mov    -0x8(%rbp),%rdx"), // 12
            s_str("add    %rdx,%rax"),       // 13
            s_str("leaveq "),                // 14
            s_str("retq   "),                // 15
            s_str("mov    $0x3,%edi"),       // 16: start
            s_str("callq  0x00400000"),      // 17
            s_str("mov    %rax,-0x8(%rbp)"), // 18
        };

        // copy to physical memory
        for (int i = 0; i < 19; ++i) {
            s_dram_write_inst(s_va2pa(i * 0x40 + 0x00400000), assembly + i);
        }
        g_cpu_pc.rip = INST_MAX_NUM_STR_LEN * sizeof(char) * 16 + 0x00400000;

        s_print_register();
        for (int i = 0; (g_cpu_pc.rip <= 18 * 0x40 + 0x00400000) && (i < INST_MAX_NUM_STR_LEN);
             i++) {
            print_separate_line();

            s_instruction_cycle();
            s_print_register();
            s_print_stack();
        }

        // final reg info
        bool match = false;
        match      = g_cpu_reg.rax == 0x6 && g_cpu_reg.rbx == 0x0 && g_cpu_reg.rcx == 0x8000650 &&
                g_cpu_reg.rdx == 0x3 && g_cpu_reg.rsi == 0x7ffffffee318 && g_cpu_reg.rdi == 0x0 &&
                g_cpu_reg.rbp == 0x7ffffffee230 && g_cpu_reg.rsp == 0x7ffffffee220;

        REQUIRE(match == true);

        match = (s_dram_read64bits(s_va2pa(0x7ffffffee230)) == 0x0000000008000650) &&
                (s_dram_read64bits(s_va2pa(0x7ffffffee228)) == 0x0000000000000006) &&
                (s_dram_read64bits(s_va2pa(0x7ffffffee220)) == 0x00007ffffffee310);

        REQUIRE(match == true);
    }
}
