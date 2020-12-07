/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */
#include "types.h"

#ifndef TASKS_H
#define TASKS_H

// Local onde o usu�rio ir� definir as tarefas
//void config_tasks();
//void task_1();
//void task_2();
//void task_3();

void config_tasks();
void task_read_buttons();
void check_wash();
void run_wash(); //clothes_control_t clothes_washing
void check_ironing();
void run_ironing(); //clothes_control_t clothes_ironing

#endif /* TASKS_H */
