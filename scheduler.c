/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include "scheduler.h"
#include "config.h"
#include "dispatcher.h"
#include "kernel.h"

u_int scheduler()
{
#if RR_SCHED == ON
   return rr_scheduler();
#else
   return prior_scheduler();
#endif
}

u_int rr_scheduler()
{
   u_int next_task = f_aptos.task_running;

   do
   {
      next_task = (next_task + 1) % (f_aptos.ready_queue_size + 1);
   } while (f_aptos.ready_queue[next_task].task_state != READY);

   return next_task;
}

u_int prior_scheduler() // inversamente proporcional
{
   u_int next_task = f_aptos.task_running;
   u_int maybe;

   for (u_int try_to_choice = 1; try_to_choice < f_aptos.ready_queue_size; try_to_choice++)
   {
      maybe = (next_task + 1) % (f_aptos.ready_queue_size);
      if (f_aptos.ready_queue[maybe].task_state == READY)
      {
         if (f_aptos.ready_queue[maybe].task_prior < f_aptos.ready_queue[next_task].task_prior)
         {
            next_task = maybe;
         }
         else if (f_aptos.ready_queue[maybe].task_prior == f_aptos.ready_queue[next_task].task_prior && next_task == f_aptos.task_running)
            next_task = maybe;
      }
   }

   return next_task;
}

/*
u_int prior_scheduler()
{
   u_int next_task = 0;
   
   // Todo: falta implementar o escalonador baseado em prioridades
   // � necess�rio definir a metodologia da prioridade (inversamente ou diretamente)
   // proporcional
   
   return next_task;
}
 */
