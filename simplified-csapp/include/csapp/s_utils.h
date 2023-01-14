

#ifndef CSAPP_UTILS_H
#define CSAPP_UTILS_H

#include <csapp/s_memory.h>

/**
 * Print all register values in cpu core
 */
void
s_print_register(void);

/**
 * Print +/-10 values of rsp(stack pointer) of register in cpu core
 */
void
s_print_stack(void);

#endif // CSAPP_UTILS_H
