

#ifndef CSAPP_INSTRUCTION_H
#define CSAPP_INSTRUCTION_H

#include <csapp/s_common.h>
#include <csapp/s_cpu.h>
#include <csapp/s_macros.h>
#include <csapp/s_memory.h>
#include <csapp/s_string.h>
#include <csapp/s_utils.h>

#define INST_NUM_HANDLER_TYPE 14
#define INST_MAX_NUM_STR_LEN  64
#define INST_MAX_NUM_CYCLE    100

typedef struct s_instruction_operand_s s_inst_operand_t;
typedef struct s_instruction_s         s_inst_t;

typedef enum
{
    INST_OPR_MOV = 0, // 0
    INST_OPR_PUSH,    // 1
    INST_OPR_POP,     // 2
    INST_OPR_LEAVE,   // 3
    INST_OPR_CALL,    // 4
    INST_OPR_RET,     // 5
    INST_OPR_ADD,     // 6
    INST_OPR_SUB,     // 7
    INST_OPR_CMP,     // 8
    INST_OPR_JNE,     // 9
    INST_OPR_JMP,     // 10
} s_instruction_operator_type_e;

/**
 * The following are 11 operands:
 *
 * 1. $0x1234
 * 2. %rax
 * 3. 0xabcd
 * 4. (%rsp)
 * 5. 0xabcd(%rsp)
 * 6. (%rsp,%rbx)
 * 7. 0xabcd(%rsp,%rbx)
 * 8. (,%rbx,8)
 * 9. 0xabcd(,%rbx,8)
 * 10. (%rsp,%rbx,8)
 * 11. 0xabcd(%rsp,%rbx,8)
 */
typedef enum
{
    INST_OPD_EMPTY = 0,             // 0
    INST_OPD_IMM,                   // 1
    INST_OPD_REG,                   // 2
    INST_OPD_MEM_IMM,               // 3
    INST_OPD_MEM_REG1,              // 4
    INST_OPD_MEM_IMM_REG1,          // 5
    INST_OPD_MEM_REG1_REG2,         // 6
    INST_OPD_MEM_IMM_REG1_REG2,     // 7
    INST_OPD_MEM_REG2_SCAL,         // 8
    INST_OPD_MEM_IMM_REG2_SCAL,     // 9
    INST_OPD_MEM_REG1_REG2_SCAL,    // 10
    INST_OPD_MEM_IMM_REG1_REG2_SCAL // 11
} s_instruction_operand_type_e;

/**
 * The following is a linear parsing state sequence:
 *
 * `|    0    |     1      |    2    |   3   |       4         |   5   |    6 |`
 * `| <blank> | <operator> | <blank> | <src> | <blank> <comma> | <dst> | <blank>
 * |`
 *
 * @example "   mov    -0x20(%rbp) ,  %rax "
 *
 */
typedef enum
{
    INST_PARSE_STATE_HEAD_BLANK = 0, // 0
    INST_PARSE_STATE_OPERATOR,       // 1
    INST_PARSE_STATE_OPR_SRC_BLANK,  // 2
    INST_PARSE_STATE_SRC,            // 3
    INST_PARSE_STATE_SRC_DST_COMMA,  // 4
    INST_PARSE_STATE_DST,            // 5
    INST_PARSE_STATE_TRAIL_BLANK     // 6
} s_inst_parse_state_e;

struct s_instruction_operand_s
{
    s_u64                        imm;  // immediate number
    s_u64                        scal; // scale number to register 2
    s_u64                        reg1; // main register
    s_u64                        reg2; // register 2
    s_instruction_operand_type_e type; // INST_OPD_IMM, INST_OPD_REG, MEM
};

/**
 * Local variables are allocated in stack in run-time,
 * we don't consider local STATIC variables.
 *
 * @ref CSAPP -- Chapter 7 Linking: 7.5 Symbols and Symbol Tables
 */
struct s_instruction_s
{
    s_inst_operand_t              src;  // operand src of instruction
    s_inst_operand_t              dst;  // operand dst of instruction
    s_instruction_operator_type_e type; // enum of operators. e.g. mov, call, etc.
};

/**
 * Instruction cycle is implemented in CPU, the only exposed interface outside
 * CPU.
 */
void
s_instruction_cycle();

/**
 * Parse and map string assembly code to `s_inst_t` instance
 *
 * @param str assembly code string, e.g. `mov $rsp,$rbp`
 * @param inst point of storing the parsed instruction
 */
void
s_inst_parse_instruction(s_str_t *str, s_inst_t *inst);

/**
 * Parse one instruction operand string and store it as a `s_inst_operand_t`
 * instance in `od`.
 *
 * @param str operand string, e.g. `0xabcd(%rsp,%rbx,8)`
 * @param od pointer of storing the parsed operand
 */
void
s_inst_parse_operand(s_str_t *opr_str, s_inst_operand_t *od);

#endif // CSAPP_INSTRUCTION_H
