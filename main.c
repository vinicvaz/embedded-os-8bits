/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#include <xc.h>
#include "types.h"
#include "kernel.h"
#include "tasks.h"
#include "config.h"
#include "mcu_config.h"
#include "mm.h"
#include "resources.h"

// Fila de tarefas aptas a serem executadas
fila_aptos_t f_aptos;
fila_clothes_t f_clothes;
machine_t w_machine;
machine_t i_machine;

void main(void)
{
   // Configura o SO antes da execu��o
   os_config();

   // Cria mem�ria heap
   //SRAMInitHeap();

   // Criar as tarefas de usu�rio
   task_create(2, &task_1);
   task_create(2, &task_2);
   task_create(3, &task_3);

   //task_create(3, &task_read_buttons); // rd4 debug
   //task_create(2, &check_ironing);     // rd3 debug
   //task_create(2, &check_wash);        // rd2 debug

   // Ativa o escalonador
   os_start();

   while (1)
   {
      ;
   }

   return;
}
