#pragma once

#ifndef CSAPP_CPU_H
#define CSAPP_CPU_H

#ifdef __cpluscplus
extern "C"
{
#endif

#include <csapp/s_config.h>
#include <csapp/s_string.h>

    /** PUBLIC Type Alias */
    typedef union cpu_flag_u             cpu_flag_t;
    typedef struct cpu_register_s        cpu_register_t;
    typedef struct cpu_program_counter_s cpu_pc_t;

    /**
 * CPU registers in each core, the reg is accessible to the core it belong to.
 */
    struct cpu_register_s
    {
        // return value
        union
        {
            s_u64 rax;
            s_u32 eax;
            s_u16 ax;
            struct
            {
                s_u8 al;
                s_u8 ah;
            };
        };

        // callee saved
        union
        {
            s_u64 rbx;
            s_u32 ebx;
            s_u16 bx;
            struct
            {
                s_u8 bl;
                s_u8 bh;
            };
        };

        // 4th argument
        union
        {
            s_u64 rcx;
            s_u32 ecx;
            s_u16 cx;
            struct
            {
                s_u8 cl;
                s_u8 ch;
            };
        };
        // 3rd argument
        union
        {
            s_u64 rdx;
            s_u32 edx;
            s_u16 dx;
            struct
            {
                s_u8 dl;
                s_u8 dh;
            };
        };
        // 2nd argument
        union
        {
            s_u64 rsi;
            s_u32 esi;
            s_u16 si;
            struct
            {
                s_u8 sil;
                s_u8 sih;
            };
        };
        // 1st argument
        union
        {
            s_u64 rdi;
            s_u32 edi;
            s_u16 di;
            struct
            {
                s_u8 dil;
                s_u8 dih;
            };
        };

        // callee saved frame pointer
        union
        {
            s_u64 rbp;
            s_u32 ebp;
            s_u16 bp;
            struct
            {
                s_u8 bpl;
                s_u8 bph;
            };
        };
        // stack pointer
        union
        {
            s_u64 rsp;
            s_u32 esp;
            s_u16 sp;
            struct
            {
                s_u8 spl;
                s_u8 sph;
            };
        };

        // 5th argument
        union
        {
            s_u64 r8;
            s_u32 r8d;
            s_u16 r8w;
            s_u8  r8b;
        };
        // 6th argument
        union
        {
            s_u64 r9;
            s_u32 r9d;
            s_u16 r9w;
            s_u8  r9b;
        };

        // caller saved
        union
        {
            s_u64 r10;
            s_u32 r10d;
            s_u16 r10w;
            s_u8  r10b;
        };
        // caller saved
        union
        {
            s_u64 r11;
            s_u32 r11d;
            s_u16 r11w;
            s_u8  r11b;
        };

        // callee saved
        union
        {
            s_u64 r12;
            s_u32 r12d;
            s_u16 r12w;
            s_u8  r12b;
        };
        // callee saved
        union
        {
            s_u64 r13;
            s_u32 r13d;
            s_u16 r13w;
            s_u8  r13b;
        };
        // callee saved
        union
        {
            s_u64 r14;
            s_u32 r14d;
            s_u16 r14w;
            s_u8  r14b;
        };
        // callee saved
        union
        {
            s_u64 r15;
            s_u32 r15d;
            s_u16 r15w;
            s_u8  r15b;
        };
    };

    /**
 * The components of an SRAM cache address:
 *
 * Note: SRAM cache address is the translated physical address.
 *
 * +-----------------------------------------------+
 * |    TAG_LEN    |   INDEX_LEN   |   OFFSET_LEN  |
 * +---------------+---------------+---------------+
 * |     tag       |     index     |     offset    |
 * +---------------+---------------+---------------+
 * |                   ADDR_LEN                    |
 * +-----------------------------------------------+
 *
 */
    struct sram_cache;

    /**
 * CPU Flags
 *
 * @ref CSAPP -- Chapter 3 Machine-Level Representation of Program: 3.6 Control
 */
    union cpu_flag_u
    {
        s_u64 flag_value;
        struct
        {
            s_u16 CF; // carry flag: detect overflow for unsigned operations
            s_u16 ZF; // zero flag: result is zero
            s_u16 SF; // sign flag: result is negative: highest bit
            s_u16 OF; // overflow flag: detect overflow for signed operations
        };
    };

    /**
 * Condition code flags of most recent (latest) operation that will only be set
 * by the following integer arithmetic instructions.
 *
 * Integer arithmetic instructions:
 *   - inc     increment 1
 *   - dec     decrement 1
 *   - neg     negate
 *   - not     complement
 *   - ----------------------------
 *   - add     add
 *   - sub     subtract
 *   - imul    multiply
 *   - xor     exclusive or
 *   - or      or
 *   - and     and
 *   - ----------------------------
 *   - sal     left shift
 *   - shl     left shift (same as sal)
 *   - sar     arithmetic right shift
 *   - shr     logical right shift
 *
 * Comparison and test instructions
 *   - cmp     compare
 *   - test    test
 */

    /**
 * CPU Core
 */
    struct cpu_program_counter_s
    {
        union
        {
            s_u64 rip;
            s_u32 eip;
        }; // program counter or instruction pointer
    };

    /**
 * Get the value stored in the register based on its name.
 *
 * @param str register name
 * @return
 */
    S_API s_u64 s_cpu_reflect_register(s_byte *str);

    /**
 * Global define cpu core parts
 *   - register
 *   - conditional flags
 *   - program counter
 */
    S_GLOBAL_VAR cpu_register_t g_cpu_reg;
    S_GLOBAL_VAR cpu_flag_t     g_cpu_flag;
    S_GLOBAL_VAR cpu_pc_t       g_cpu_pc;

#ifdef __cpluscplus
}
#endif

#endif // CSAPP_CPU_H
