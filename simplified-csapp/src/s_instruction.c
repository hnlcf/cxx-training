#include <csapp/s_instruction.h>

/* Handler table storing the handlers to different instruction types */
typedef void (*inst_handler_t)(s_inst_operand_t *, s_inst_operand_t *);

/**
 * Manipulate memory to execute operations based operand.
 *
 * @param od pointer to the address to store the parsed operand
 * @return a memory address which is different in different states
 */
S_LOCAL s_u64
inst_compute_operand(s_inst_operand_t *od)
{
    if (od->type == INST_OPD_IMM) {
        // immediate signed number can be negative: convert to bitmap
        return *(s_u64 *)&od->imm;
    } else if (od->type == INST_OPD_REG) {
        // default register 1
        return od->reg1;
    } else if (od->type == INST_OPD_EMPTY) {
        return 0;
    } else {
        // access memory: return the physical address
        s_u64 vaddr = 0;

        if (od->type == INST_OPD_MEM_IMM) {
            vaddr = od->imm;
        } else if (od->type == INST_OPD_MEM_REG1) {
            vaddr = *((s_u64 *)od->reg1);
        } else if (od->type == INST_OPD_MEM_IMM_REG1) {
            vaddr = od->imm + (*((s_u64 *)od->reg1));
        } else if (od->type == INST_OPD_MEM_REG1_REG2) {
            vaddr = (*((s_u64 *)od->reg1)) + (*((s_u64 *)od->reg2));
        } else if (od->type == INST_OPD_MEM_IMM_REG1_REG2) {
            vaddr = od->imm + (*((s_u64 *)od->reg1)) + (*((s_u64 *)od->reg2));
        } else if (od->type == INST_OPD_MEM_REG2_SCAL) {
            vaddr = (*((s_u64 *)od->reg2)) * od->scal;
        } else if (od->type == INST_OPD_MEM_IMM_REG2_SCAL) {
            vaddr = od->imm + (*((s_u64 *)od->reg2)) * od->scal;
        } else if (od->type == INST_OPD_MEM_REG1_REG2_SCAL) {
            vaddr = (*((s_u64 *)od->reg1)) + (*((s_u64 *)od->reg2)) * od->scal;
        } else if (od->type == INST_OPD_MEM_IMM_REG1_REG2_SCAL) {
            vaddr = od->imm + (*((s_u64 *)od->reg1)) + (*((s_u64 *)od->reg2)) * od->scal;
        }
        return vaddr;
    }

    // empty
    return 0;
}

/**
 * Update the rip pointer to the next instruction sequentially.
 */
S_LOCAL S_INLINE void
inst_increase_pc(void)
{
    // we are handling the fixed-length of assembly string here
    // but their size can be variable as true X86 instructions
    // that's because the operands' sizes follow the specific encoding rule
    // the risc-v is a fixed length ISA
    g_cpu_pc.rip = g_cpu_pc.rip + sizeof(char) * INST_MAX_NUM_STR_LEN;
}

/**
 * Instruction handlers.
 *
 *
 * In this simulator, the instructions have been decoded and fetched
 * so there will be no page fault during fetching
 * otherwise the instructions must handle the page fault (swap in from disk)
 * first and then re-fetch the instruction and do decoding and finally re-run
 * the instruction
 */
S_LOCAL void
inst_handler_mov(s_inst_operand_t *src_od, s_inst_operand_t *dst_od)
{
    s_u64 src = inst_compute_operand(src_od);
    s_u64 dst = inst_compute_operand(dst_od);

    if (src_od->type == INST_OPD_REG && dst_od->type == INST_OPD_REG) {
        // src: register
        // dst: register
        *(s_u64 *)dst = *(s_u64 *)src;

        inst_increase_pc();
        g_cpu_flag.flag_value = 0;
    } else if (src_od->type == INST_OPD_REG && dst_od->type >= INST_OPD_MEM_IMM) {
        // src: register
        // dst: virtual address
        s_dram_write64bits(s_va2pa(dst), *(s_u64 *)src);

        inst_increase_pc();
        g_cpu_flag.flag_value = 0;
    } else if (src_od->type >= INST_OPD_MEM_IMM && dst_od->type == INST_OPD_REG) {
        // src: virtual address
        // dst: register
        *(s_u64 *)dst = s_dram_read64bits(s_va2pa(src));

        inst_increase_pc();
        g_cpu_flag.flag_value = 0;
    } else if (src_od->type == INST_OPD_IMM && dst_od->type == INST_OPD_REG) {
        // src: immediate number (s_u64 bit map)
        // dst: register
        *(s_u64 *)dst = src;

        inst_increase_pc();
        g_cpu_flag.flag_value = 0;
    }
}

S_LOCAL void
inst_handler_push(s_inst_operand_t *src_od, s_inst_operand_t *dst_od __attribute__((unused)))
{
    s_u64 src = inst_compute_operand(src_od);
    // s_u64 dst = inst_compute_operand(dst_od);

    if (src_od->type == INST_OPD_REG) {
        // src: register
        // dst: empty
        g_cpu_reg.rsp = g_cpu_reg.rsp - 8;
        s_dram_write64bits(s_va2pa(g_cpu_reg.rsp), *(s_u64 *)src);

        inst_increase_pc();
        g_cpu_flag.flag_value = 0;
    }
}

S_LOCAL void
inst_handler_pop(s_inst_operand_t *src_od, s_inst_operand_t *dst_od __attribute__((unused)))
{
    s_u64 src = inst_compute_operand(src_od);
    // s_u64 dst = inst_compute_operand(dst_od);

    if (src_od->type == INST_OPD_REG) {
        // src: register
        // dst: empty
        s_u64 old_val = s_dram_read64bits(s_va2pa(g_cpu_reg.rsp));

        g_cpu_reg.rsp = g_cpu_reg.rsp + 8;
        *(s_u64 *)src = old_val;

        inst_increase_pc();
        g_cpu_flag.flag_value = 0;
    }
}

S_LOCAL void
inst_handler_leave(s_inst_operand_t *src_od __attribute__((unused)),
                   s_inst_operand_t *dst_od __attribute__((unused)))
{
    // `movq %rbp, %rsp` Set stack pointer to beginning of frame
    g_cpu_reg.rsp = g_cpu_reg.rbp;

    // `popq %rbp`  Restore saved %rbp and set stack ptr to end of caller’s frame
    s_u64 old_val = s_dram_read64bits(s_va2pa(g_cpu_reg.rsp));

    g_cpu_reg.rsp = g_cpu_reg.rsp + 8;
    g_cpu_reg.rbp = old_val;

    inst_increase_pc();
    g_cpu_flag.flag_value = 0;
}

S_LOCAL void
inst_handler_call(s_inst_operand_t *src_od, s_inst_operand_t *dst_od __attribute__((unused)))
{
    s_u64 src = inst_compute_operand(src_od);
    // s_u64 dst = inst_compute_operand(dst_od);

    // src: immediate number: virtual address of target function starting
    // dst: empty
    // push the return value
    g_cpu_reg.rsp = g_cpu_reg.rsp - 8;
    s_dram_write64bits(s_va2pa(g_cpu_reg.rsp), g_cpu_pc.rip + sizeof(char) * INST_MAX_NUM_STR_LEN);

    // jump to target function address
    g_cpu_pc.rip          = src;
    g_cpu_flag.flag_value = 0;
}

S_LOCAL void
inst_handler_ret(s_inst_operand_t *src_od __attribute__((unused)),
                 s_inst_operand_t *dst_od __attribute__((unused)))
{
    // s_u64 src = inst_compute_operand(src_od);
    // s_u64 dst = inst_compute_operand(dst_od);

    // src: empty
    // dst: empty
    // pop rsp
    s_u64 ret_addr = s_dram_read64bits(s_va2pa(g_cpu_reg.rsp));
    g_cpu_reg.rsp  = g_cpu_reg.rsp + 8;

    // jump to return address
    g_cpu_pc.rip          = ret_addr;
    g_cpu_flag.flag_value = 0;
}

S_LOCAL void
inst_handler_add(s_inst_operand_t *src_od, s_inst_operand_t *dst_od)
{
    s_u64 src = inst_compute_operand(src_od);
    s_u64 dst = inst_compute_operand(dst_od);

    if (src_od->type == INST_OPD_REG && dst_od->type == INST_OPD_REG) {
        // src: register (value: int64_t bit map)
        // dst: register (value: int64_t bit map)
        s_u64 src_val = *(s_u64 *)src;
        s_u64 dst_val = *(s_u64 *)dst;
        s_u64 val     = src_val + dst_val;

        s_u8 val_sign = ((val >> 63) & 0x1);
        s_u8 src_sign = ((src_val >> 63) & 0x1);
        s_u8 dst_sign = ((dst_val >> 63) & 0x1);

        // set condition flags
        g_cpu_flag.CF = (val < src_val);
        g_cpu_flag.ZF = (val == 0);
        g_cpu_flag.SF = val_sign;
        g_cpu_flag.OF = ((src_sign == dst_sign) && (val_sign != src_sign));

        // update registers
        *(s_u64 *)dst = val;

        inst_increase_pc();
        return;
    }
}

S_LOCAL void
inst_handler_sub(s_inst_operand_t *src_od, s_inst_operand_t *dst_od)
{
    s_u64 src = inst_compute_operand(src_od);
    s_u64 dst = inst_compute_operand(dst_od);

    if (src_od->type == INST_OPD_IMM && dst_od->type == INST_OPD_REG) {
        // src: immediate number (value: int64_t bit map)
        // dst: register (value: int64_t bit map)

        // dst = dst - src
        s_u64 dst_val = *(s_u64 *)dst;
        s_u64 val     = dst_val + (~src + 1);

        s_u8 val_sign = ((val >> 63) & 0x1);
        s_u8 src_sign = ((src >> 63) & 0x1);
        s_u8 dst_sign = ((dst_val >> 63) & 0x1);

        // set condition flags
        g_cpu_flag.CF = (val > dst_val);
        g_cpu_flag.ZF = (val == 0);
        g_cpu_flag.SF = val_sign;
        g_cpu_flag.OF = (src_sign != dst_sign) && (val_sign != dst_sign);

        // update registers
        *(s_u64 *)dst = val;

        inst_increase_pc();
        return;
    }
}

S_LOCAL void
inst_handler_cmp(s_inst_operand_t *src_od, s_inst_operand_t *dst_od)
{
    s_u64 src = inst_compute_operand(src_od);
    s_u64 dst = inst_compute_operand(dst_od);

    if (src_od->type == INST_OPD_IMM && dst_od->type >= INST_OPD_MEM_IMM) {
        // src: immediate number (value: int64_t bit map)
        // dst: memory (value: int64_t bit map)
        s_u64 dst_val = s_dram_read64bits(s_va2pa(dst));
        s_u64 val     = dst_val + (~src + 1);

        s_u8 val_sign = ((val >> 63) & 0x1);
        s_u8 src_sign = ((src >> 63) & 0x1);
        s_u8 dst_sign = ((dst_val >> 63) & 0x1);

        // set condition flags
        g_cpu_flag.CF = (val > dst_val);
        g_cpu_flag.ZF = (val == 0);
        g_cpu_flag.SF = val_sign;
        g_cpu_flag.OF = (src_sign != dst_sign) && (val_sign != dst_sign);

        inst_increase_pc();
        return;
    }
}

S_LOCAL void
inst_handler_jne(s_inst_operand_t *src_od, s_inst_operand_t *dst_od __attribute__((unused)))
{
    s_u64 src = inst_compute_operand(src_od);
    if (src_od->type == INST_OPD_MEM_IMM) {
        if (g_cpu_flag.ZF == 0) {
            g_cpu_pc.rip = src;
        } else {
            inst_increase_pc();
        }
    }

    g_cpu_flag.flag_value = 0;
}

S_LOCAL void
inst_handler_jmp(s_inst_operand_t *src_od, s_inst_operand_t *dst_od __attribute__((unused)))
{
    s_u64 src             = inst_compute_operand(src_od);
    g_cpu_pc.rip          = src;

    g_cpu_flag.flag_value = 0;
}

// look-up table of pointers to function
S_LOCAL inst_handler_t l_inst_handler_table[INST_NUM_HANDLER_TYPE] = {
    &inst_handler_mov,   // 0
    &inst_handler_push,  // 1
    &inst_handler_pop,   // 2
    &inst_handler_leave, // 3
    &inst_handler_call,  // 4
    &inst_handler_ret,   // 5
    &inst_handler_add,   // 6
    &inst_handler_sub,   // 7
    &inst_handler_cmp,   // 8
    &inst_handler_jne,   // 9
    &inst_handler_jmp,   // 10
};

void
s_instruction_cycle(void)
{
    s_inst_t       inst;
    s_str_t       *inst_str;
    inst_handler_t inst_handler;

    // Allocate instruction string
    inst_str = s_str_new(INST_MAX_NUM_STR_LEN + 10);
    if (inst_str == NULL) {
        return;
    }

    // FETCH: get the instruction string by program counter
    s_dram_read_inst(s_va2pa(g_cpu_pc.rip), inst_str);
    debug_printf(DEBUG_PRINT_INST_CYCLE, "%lx    %s\n", g_cpu_pc.rip, inst_str->data);

    // DECODE: decode the run-time instruction operands
    s_inst_parse_instruction(inst_str, &inst);

    // EXECUTE: get the function pointer or inst_handler by the operator
    inst_handler = l_inst_handler_table[inst.type];

    // EXECUTE: run instruction
    inst_handler(&(inst.src), &(inst.dst));

    // Free instruction string
    s_str_free(inst_str);
}

void
s_inst_parse_instruction(s_str_t *str, s_inst_t *inst)
{
    s_str_t *opr_str = s_str_new(64);
    s_str_t *src_str = s_str_new(64);
    s_str_t *dst_str = s_str_new(64);

    s_byte ch;
    s_u32  num_parentheses     = 0;

    s_inst_parse_state_e state = INST_PARSE_STATE_HEAD_BLANK;

    s_byte blank               = ' ';
    s_byte comma               = ',';

    s_u64 i;
    for (i = 0; i < str->len; i++) {
        ch = str->data[i];

        // record
        if (ch == '(' || ch == ')') {
            num_parentheses++;
        }

        // switch parse state
        if (state == INST_PARSE_STATE_HEAD_BLANK && ch != blank) {
            state = INST_PARSE_STATE_OPERATOR;
        } else if (state == INST_PARSE_STATE_OPERATOR && ch == blank) {
            state = INST_PARSE_STATE_OPR_SRC_BLANK;
            continue;
        } else if (state == INST_PARSE_STATE_OPR_SRC_BLANK && ch != blank) {
            state = INST_PARSE_STATE_SRC;
        } else if (state == INST_PARSE_STATE_SRC &&
                   (num_parentheses == 0 || num_parentheses == 2) && (ch == blank || ch == comma)) {
            state = INST_PARSE_STATE_SRC_DST_COMMA;
            continue;
        } else if (state == INST_PARSE_STATE_SRC_DST_COMMA && (ch != blank && ch != comma)) {
            state = INST_PARSE_STATE_DST;
        } else if (state == INST_PARSE_STATE_DST && ch == blank) {
            state = INST_PARSE_STATE_TRAIL_BLANK;
            continue;
        }

        // read operator, operand1(src), operand2(dst)
        if (state == INST_PARSE_STATE_OPERATOR) {
            opr_str->data[opr_str->len++] = ch;
            opr_str->remain--;
            continue;
        } else if (state == INST_PARSE_STATE_SRC) {
            src_str->data[src_str->len++] = ch;
            src_str->remain--;
            continue;
        } else if (state == INST_PARSE_STATE_DST) {
            dst_str->data[dst_str->len++] = ch;
            dst_str->remain--;
            continue;
        }
    }

    opr_str->data[opr_str->len] = '\0';
    src_str->data[src_str->len] = '\0';
    dst_str->data[dst_str->len] = '\0';

    // parse operator
    if (s_strcmp(opr_str->data, "mov") == 0 || s_strcmp(opr_str->data, "movq") == 0) {
        inst->type = INST_OPR_MOV;
    } else if (s_strcmp(opr_str->data, "push") == 0) {
        inst->type = INST_OPR_PUSH;
    } else if (s_strcmp(opr_str->data, "pop") == 0) {
        inst->type = INST_OPR_POP;
    } else if (s_strcmp(opr_str->data, "leaveq") == 0) {
        inst->type = INST_OPR_LEAVE;
    } else if (s_strcmp(opr_str->data, "callq") == 0) {
        inst->type = INST_OPR_CALL;
    } else if (s_strcmp(opr_str->data, "retq") == 0) {
        inst->type = INST_OPR_RET;
    } else if (s_strcmp(opr_str->data, "add") == 0) {
        inst->type = INST_OPR_ADD;
    } else if (s_strcmp(opr_str->data, "sub") == 0) {
        inst->type = INST_OPR_SUB;
    } else if (s_strcmp(opr_str->data, "cmpq") == 0) {
        inst->type = INST_OPR_CMP;
    } else if (s_strcmp(opr_str->data, "jne") == 0) {
        inst->type = INST_OPR_JNE;
    } else if (s_strcmp(opr_str->data, "jmp") == 0) {
        inst->type = INST_OPR_JMP;
    }

    // parse operands
    s_inst_parse_operand(src_str, &inst->src);
    s_inst_parse_operand(dst_str, &inst->dst);

    // debug info
    debug_printf(DEBUG_PRINT_INST_PARSE,
                 "[%s (%d)] [%s (%d)] [%s (%d)]\n",
                 (char *)opr_str->data,
                 inst->type,
                 (char *)src_str->data,
                 inst->src.type,
                 (char *)dst_str->data,
                 inst->dst.type);

    s_str_free(opr_str);
    s_str_free(src_str);
    s_str_free(dst_str);
}

S_LOCAL void
inst_reset_operand(s_inst_operand_t *od)
{
    od->type = INST_OPD_EMPTY;
    od->imm  = 0;
    od->scal = 0;
    od->reg1 = 0;
    od->reg2 = 0;
}

void
s_inst_parse_operand(s_str_t *opr_str, s_inst_operand_t *od)
{
    inst_reset_operand(od);

    if (opr_str->len == 0) {
        return;
    }

    s_byte *str = opr_str->data;

    if (str[0] == '$') {
        // immediate number
        od->type = INST_OPD_IMM;
        od->imm  = string2uint_range(str, 1, -1);
    } else if (str[0] == '%') {
        // register
        od->type = INST_OPD_REG;
        od->reg1 = s_cpu_reflect_register(str);
        return;
    } else {
        // memory access
        s_str_t *imm  = s_str_new(64);
        s_str_t *reg1 = s_str_new(64);
        s_str_t *reg2 = s_str_new(64);
        s_str_t *scal = s_str_new(64);

        s_i32 count_a = 0; // count number of ( )
        s_i32 count_b = 0; // count number of comma ,

        s_u64 i;
        for (i = 0; i < opr_str->len; ++i) {
            s_byte ch = str[i];
            if (ch == '(' || ch == ')') {
                count_a++;
                continue;
            } else if (ch == ',') {
                count_b++;
                continue;
            } else {
                // parse imm(reg1,reg2,scal)
                if (count_a == 0) {
                    imm->data[imm->len++] = ch;
                    imm->remain--;
                    continue;
                } else if (count_a == 1) {
                    if (count_b == 0) {
                        reg1->data[reg1->len++] = ch;
                        reg1->remain--;
                        continue;
                    } else if (count_b == 1) {
                        reg2->data[reg2->len++] = ch;
                        reg1->remain--;
                        continue;
                    } else if (count_b == 2) {
                        scal->data[scal->len++] = ch;
                        scal->remain--;
                    }
                }
            }
        }

        imm->data[imm->len]   = '\0';
        reg1->data[reg1->len] = '\0';
        reg2->data[reg2->len] = '\0';
        scal->data[scal->len] = '\0';

        // imm
        if (imm->len > 0) {
            od->imm = string2uint(imm->data);
            if (count_a == 0) {
                od->type = INST_OPD_MEM_IMM;
                goto free_and_return;
            }
        }

        // reg
        if (reg1->len > 0) {
            od->reg1 = s_cpu_reflect_register(reg1->data);
        }
        if (reg2->len > 0) {
            od->reg2 = s_cpu_reflect_register(reg2->data);
        }

        // scal
        if (scal->len > 0) {
            od->scal = string2uint(scal->data);
            if (od->scal != 1 && od->scal != 2 && od->scal != 4 && od->scal != 8) {
                printf("%s is not a legal scalar\n", (char *)scal->data);
                goto free_and_return;
            }
        }

        /* set operand type */
        if (count_b == 0) {
            if (imm->len > 0) {
                od->type = INST_OPD_MEM_IMM_REG1;
                goto free_and_return;
            } else {
                od->type = INST_OPD_MEM_REG1;
                goto free_and_return;
            }
        } else if (count_b == 1) {
            if (imm->len > 0) {
                od->type = INST_OPD_MEM_IMM_REG1_REG2;
                goto free_and_return;
            } else {
                od->type = INST_OPD_MEM_REG1_REG2;
                goto free_and_return;
            }
        } else if (count_b == 2) {
            if (reg1->len > 0) {
                // reg1 exits
                if (imm->len > 0) {
                    od->type = INST_OPD_MEM_IMM_REG1_REG2_SCAL;
                    goto free_and_return;
                } else {
                    od->type = INST_OPD_MEM_REG1_REG2_SCAL;
                    goto free_and_return;
                }
            } else {
                // no reg1
                if (imm->len > 0) {
                    od->type = INST_OPD_MEM_IMM_REG2_SCAL;
                    goto free_and_return;
                } else {
                    od->type = INST_OPD_MEM_REG2_SCAL;
                    goto free_and_return;
                }
            }
        }

    free_and_return:
        s_str_free(imm);
        s_str_free(reg1);
        s_str_free(reg2);
        s_str_free(scal);

        return;
    }
}
