#pragma once

#ifndef CSAPP_MEMORY_H
#define CSAPP_MEMORY_H

#include <csapp/s_cpu.h>
#include <csapp/s_instruction.h>
#include <csapp/s_macros.h>
#include <csapp/s_string.h>

/**
 * physical memory space is decided by the physical address
 * in this simulator, there are 4 + 6 + 6 = 16 bit physical adderss
 * then the physical space is (1 << 16) = 65536
 * total 16 physical memory
 */

#define MEM_PHYSICAL_SPACE          65536
#define MEM_MAX_PHYSICAL_PAGE_INDEX 15

/** The global physical memory */
S_GLOBAL_VAR s_u8 g_physical_mem[MEM_PHYSICAL_SPACE];

/**
 * Read a 64 bits num from a physical address.
 *
 * @param p_addr the physical address
 * @param cr active cpu core processor
 * @return a unsigned 64 bits integer
 */
S_API s_u64
s_dram_read64bits(s_u64 p_addr);

/**
 * Write a 64 bits num to a physical address.
 *
 * @param p_addr the physical address
 * @param data the 64 bits integer need to be writed
 * @param cr active cpu core processor
 */
S_API void
s_dram_write64bits(s_u64 p_addr, s_u64 data);

/**
 * Read instruction from a physical address.
 *
 * @param p_addr the physical address
 * @param buf target read buffer
 * @param cr active cpu core processor
 */
S_API void
s_dram_read_inst(s_u64 p_addr, s_str_t *buf);

/**
 * Write instruction to a physical address.
 *
 * @param p_addr the physical address
 * @param str instruction string
 * @param cr active cpu core processor
 */
S_API void
s_dram_write_inst(s_u64 p_addr, s_str_t *str);

/**
 * Convert a virtual address to physical address in MMU, each MMU is only owned
 * by each core.
 *
 * @param v_addr the virtual address
 * @param cr active cpu core processor
 * @return the physical address
 */
S_API s_u64
s_va2pa(s_u64 v_addr);

#endif // CSAPP_MEMORY_H
