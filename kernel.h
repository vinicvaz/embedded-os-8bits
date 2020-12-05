/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"

// Definição das chamadas de sistema do kernel
uint8_t get_task_id();
void task_delay(u_int ms);
void task_create(u_int prior, task_ptr func);
void os_start();
void os_config();
void task_idle();
void task_yield();

// Rotinas auxiliares do kernel
u_int who();



#endif	/* KERNEL_H */

