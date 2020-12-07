/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include "kernel.h"
#include "dispatcher.h"
#include "timer.h"
#include "tasks.h"
//#include "tasks.c"

extern fila_aptos_t f_aptos;
extern fila_clothes_t f_clothes;

// Chamadas de sistema

/**
 * Retorna a identifica��o da tarefa
 */
uint8_t get_task_id()
{
   return f_aptos.ready_queue[f_aptos.task_running].task_id;
}

/**
 * Coloca a tarefa que atualmente est� executando em estado de espera
 */
void task_delay(u_int ms)
{
   // Desabilitar interrup��es globais
   DISABLE_INTERRUPTS();

   // Chamar o despachante para salvar o contexto da terefa que est�
   // executando
   SAVE_CONTEXT(WAITING_DELAY);

   // Define o tempo de espera
   f_aptos.ready_queue[f_aptos.task_running].delay = ms;

   // Restaura o contexto da tarefa que entrar� em execu��o
   RESTORE_CONTEXT();
}

/**
 * Cria uma tarefa no SO. Os par�metros necess�rios s�o a prioridade e o ponteiro para a fun��o da tarefa
 */
void task_create(u_int prior, task_ptr func)
{
   tcb_t new_task;

   new_task.task_id = f_aptos.ready_queue_size + 1;
   new_task.task_prior = prior;
   new_task.task_state = READY;
   new_task.task_func = func;
   new_task.task_stack.size = 0;

   // Incui a tarefa na fila de aptos
   f_aptos.ready_queue[f_aptos.ready_queue_size + 1] = new_task; // [f_aptos.ready_queue_size]
   f_aptos.ready_queue_size++;
}

/**
 * Inicia a execu��o do SO. Inicia o perif�rico do TIMER
 */
void os_start()
{
   // Coloca o SO em execu��o

   // Habilita as interrup��es globais
   ENABLE_INTERRUPTS();

   // Inicia o TIMER0
   T0CONbits.TMR0ON = 1;
}

/**
 * Configura o SO antes de sua execu��o
 */
void os_config()
{
   __asm("GLOBAL _task_idle, _task_read_buttons, _check_wash, _check_ironing");

   // Configura as estruturas de dados do kernel
   f_aptos.task_running = 0;
   f_aptos.ready_queue_size = 0;
   f_clothes.fila_size = 0;
   f_clothes.clothes_finished_size = 0;

   // Configura o timer
   setup_timer_0();

   // Cria a tarefa IDLE
   task_create(0, &task_idle);

   // Chama a rotina para configurar as tarefas
   config_tasks();
}

/**
 * Tarefa IDLE - � executada quando n�o h� nenhuma outra tarefa no estado de pronto na fila de aptos
 */
void task_idle()
{
   while (1)
   {
      //Nop();
      ;
      //PORTDbits.RD3 = ~PORTDbits.RD3;
   }
}

/**
 * Faz com que a tarefa libere o processador voluntariamente
 */
void task_yield()
{
   DISABLE_INTERRUPTS();
   SAVE_CONTEXT(READY);
   RESTORE_CONTEXT();
}

// Rotinas auxiliares do kernel
u_int who()
{
   return f_aptos.task_running;
}
