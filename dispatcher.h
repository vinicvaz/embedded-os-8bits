/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef DISPATCHER_H
#define	DISPATCHER_H

#include "types.h"
#include "scheduler.h"
#include <xc.h>

extern fila_aptos_t f_aptos;
int index;


/**
 * Rotina para salvar o contexto da tarefa
 */
#define SAVE_CONTEXT(state) \
do { \
    if (f_aptos.ready_queue[f_aptos.task_running].task_stack.size > 0) { \
        index = 0;\
        f_aptos.ready_queue[f_aptos.task_running].task_state        = state;\
        f_aptos.ready_queue[f_aptos.task_running].w_reg             = WREG;\
        f_aptos.ready_queue[f_aptos.task_running].bsr_reg           = BSR;\
        f_aptos.ready_queue[f_aptos.task_running].status_reg        = STATUS;\
        f_aptos.ready_queue[f_aptos.task_running].task_stack.size   = 0;\
        do {\
            f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index] = TOS;\
            index++;\
            __asm("POP");\
        } while (STKPTR);\
     } \
     else { \
        f_aptos.ready_queue[f_aptos.task_running].task_state        = state;\
     } \
} while (0);


/**
 * Rotina para restaurar o contexto da tarefa
 */
#define RESTORE_CONTEXT() \
do {\
    f_aptos.task_running = scheduler();\
    if (f_aptos.ready_queue[f_aptos.task_running].task_stack.size == 0) {\
        TOS = f_aptos.ready_queue[f_aptos.task_running].task_func;\
    }\
    else {\
        f_aptos.ready_queue[f_aptos.task_running].task_state = RUNNING;\
        WREG    = f_aptos.ready_queue[f_aptos.task_running].w_reg;\
        BSR     = f_aptos.ready_queue[f_aptos.task_running].bsr_reg;\
        STATUS  = f_aptos.ready_queue[f_aptos.task_running].status_reg;\
        index   = f_aptos.ready_queue[f_aptos.task_running].task_stack.size - 1;\
        STKPTR  = 0;\
        while(index) {\
            __asm("PUSH");\
            TOS = f_aptos.ready_queue[f_aptos.task_running].task_stack.stack[index];\
            index--;\
        }\
    }\
    ENABLE_INTERRUPTS();\
} while (0);


#endif	/* DISPATCHER_H */

