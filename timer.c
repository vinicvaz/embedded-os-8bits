/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include <pic18f4520.h>

#include "tasks.h"
#include "kernel.h"
#include "mm.h"
#include "types.h"
#include "config.h"
#include "resources.h"

#include <xc.h>
#include "timer.h"
#include "dispatcher.h"

extern fila_aptos_t f_aptos;
extern fila_clothes_t f_clothes;
int quantum = QUANTUM;

void setup_timer_0() {
   // Configuracao do timer 0
   INTCONbits.TMR0IE = 1;
   INTCONbits.PEIE = 1;
   T0CONbits.T0CS = 0;
   T0CONbits.PSA = 0;
   T0CONbits.T0PS = 0b111;
   TMR0L = 0;
}

u_int controle_delay() {
   int i, flag = 0;

   // Verifica tarefas que est�o em delay
   for (i = 0; i < f_aptos.ready_queue_size; i++) {
      if (f_aptos.ready_queue[i].task_state == WAITING_DELAY) {
         f_aptos.ready_queue[i].delay--;
         if (f_aptos.ready_queue[i].delay == 0) {
            f_aptos.ready_queue[i].task_state = READY;
            flag = 1;
         }
      }
   }

   return flag;
}

void __interrupt() IRQ_Timer0() {

   DISABLE_INTERRUPTS();

   if (INTCONbits.TMR0IF) 
   {
      INTCONbits.TMR0IF = 0;
      TMR0L = 0; // Todo: falta definir o valor.

#if RR_SCHED == ON
      // Diminui o tempo
      quantum--;
      if (quantum == 0) 
      {
         // Salvar o contexto da tarefa que ir� deixar a CPU
         SAVE_CONTEXT(READY);
         // Escolher outra tarefa para executar
         RESTORE_CONTEXT();
         quantum = QUANTUM;
      }
#endif

      if (controle_delay() && PRIOR_SCHED == ON) 
      {
         // Salvar o contexto da tarefa que ir� deixar a CPU
         SAVE_CONTEXT(READY);
         // Escolher outra tarefa para executar
         RESTORE_CONTEXT();
      }
   }
   
   if (INTCONbits.INT0IF == 1 && f_clothes.fila_size < MAX_CLOTHES) 
   {
      clothes_control_t clothes;
      clothes.color = 0;
      clothes.washing_cycles = 1;
      clothes.state = 0;

      f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
      f_clothes.fila_size++;

      INTCONbits.INT0IF = 0;
      ENABLE_INTERRUPTS();
   }

   if (INTCON3bits.INT1IF == 1 && f_clothes.fila_size < MAX_CLOTHES) 
   {
      clothes_control_t clothes;
      clothes.color = 1;
      clothes.washing_cycles = 2;
      clothes.state = 0;

      f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
      f_clothes.fila_size++;
      INTCON3bits.INT1IF = 0;
      ENABLE_INTERRUPTS();
   }
}

