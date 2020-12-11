/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <xc.h>
#include "config.h"

#define STACK_SIZE 32
#define IDLE 1

#define DISABLE_INTERRUPTS() INTCONbits.GIE = 0
#define ENABLE_INTERRUPTS() INTCONbits.GIE = 1

typedef unsigned char u_char;
typedef unsigned int u_int;

// Estados da tarefa
typedef enum
{
    READY = 0,
    RUNNING,
    WAITING_SEM,
    WAITING_DELAY,
    FINISHED
} task_state_t;

// Ponteiro para a tarefa
typedef void (*task_ptr)(void);

// Dados da pilha
typedef struct task_stack
{
    u_char stack[STACK_SIZE];
    u_int size;
} task_stack_t;

// Dados da TCB (Task Control Block)
typedef struct tcb
{
    u_int task_id;
    task_state_t task_state;
    u_int task_prior;
    task_ptr task_func;
    task_stack_t task_stack;
    u_char bsr_reg;
    u_char w_reg;
    u_char status_reg;
    u_int delay;
} tcb_t;

// Fila de aptos
typedef struct fila_aptos
{
    tcb_t ready_queue[MAX_TASKS + IDLE];
    u_int ready_queue_size;
    u_int task_running;
} fila_aptos_t;

// Clothes
typedef struct clothes_control
{
    //u_int id;
    u_int color; //colorida = 0; branca = 1;
    //u_int qtd; //quantidade pode ser usada pra saber o tempo de lavagem e de passagem
    u_int washing_cycles; //ciclo de lavagem
    u_int state;          // 0 suja; 1 limpa; 2 pronta

} clothes_control_t;

typedef struct fila_clothes
{
    clothes_control_t clothes_waiting[MAX_CLOTHES]; //fila de roupas prontas para serem lavadas
    u_int fila_size;                                // tamanho da fila
    clothes_control_t clothes_washing;
    clothes_control_t clothes_ironing;
    clothes_control_t clothes_finished[MAX_CLOTHES];
    u_int clothes_finished_size;

    //u_int clothes_washing;
} fila_clothes_t;

#endif /* TYPES_H */
