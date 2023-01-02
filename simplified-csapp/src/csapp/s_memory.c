// Dynamic Random Access Memory
#include <csapp/s_memory.h>

s_u8 g_physical_mem[MEM_PHYSICAL_SPACE];

/**
 * Be careful with the x86-64 little endian integer encoding
 * e.g. write `0x 00 00 7f d3 57 a0 2a e0` to cache, the memory lapping should
 * be: `e0 2a a0 57 d3 7f 00 00`
 */

s_u64
s_dram_read64bits(s_u64 p_addr) {
    if (DEBUG_ENABLE_SRAM_CACHE == 1) {
        // try to load s_u64 from SRAM cache
        // little-endian
    } else {
        // read from DRAM directly
        // little-endian
        s_u64 val = 0x0;

        s_i32 i;
        for (i = 0; i < 8; i++) {
            val += (((s_u64) g_physical_mem[p_addr + i]) << (i << 3));
        }

        return val;
    }
}

void
s_dram_write64bits(s_u64 p_addr, s_u64 data) {
    if (DEBUG_ENABLE_SRAM_CACHE == 1) {
        // try to write s_u64 to SRAM cache
        // little-endian
        return;
    } else {
        // write to DRAM directly
        // little-endian
        s_i32 i;
        for (i = 0; i < 8; i++) {
            g_physical_mem[p_addr + i] = (data >> (i << 3)) & 0xff;
        }
    }
}

void
s_dram_read_inst(s_u64 p_addr, s_str_t *buf) {
    s_i32 i;
    for (i = 0; i < INST_MAX_NUM_STR_LEN; i++) {
        buf->data[i] = (char) g_physical_mem[p_addr + i];
        buf->len++;

        if (s_str_is_dynamic(buf)) {
            buf->remain--;
        }
    }
}

void
s_dram_write_inst(s_u64 p_addr, s_str_t *str) {
    assert(str->len < INST_MAX_NUM_STR_LEN);

    s_u64 i;
    for (i = 0; i < INST_MAX_NUM_STR_LEN; i++) {
        if (i < str->len) {
            g_physical_mem[p_addr + i] = (s_u8) str->data[i];
        } else {
            g_physical_mem[p_addr + i] = 0;
        }
    }
}

s_u64
s_va2pa(s_u64 v_addr) {
    return v_addr & (0xffffffffffffffff >> (64 - MEM_MAX_PHYSICAL_PAGE_INDEX));
}
