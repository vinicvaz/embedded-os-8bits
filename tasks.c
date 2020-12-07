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

extern fila_clothes_t f_clothes;
u_int counter_wash; // fazer recurso maquina?
u_int counter_ironing;
u_int flag_wash = 0; // 0 desocupada, 1 ocupada //fazer recurso maquina?
u_int flag_ironing = 0; // 0 desocupado, 1 ocupado

void config_tasks() {
   TRISDbits.RD0 = 1; // colorido
   TRISDbits.RD1 = 1; // branco
   TRISDbits.RD2 = 0; // maquina
   TRISDbits.RD3 = 0; // ferro de passar
   TRISDbits.RD4 = 0; // debug
}

void task_read_buttons() {

   if (PORTDbits.RD0 == 1 && f_clothes.fila_size < MAX_CLOTHES) {
      // CRIA STRUCT ROUPA COLORIDA
      clothes_control_t clothes;
      clothes.color = 0;
      clothes.washing_cicles = 1;
      clothes.state = 0;

      //PORTDbits.RD2 = 1;

      //if ((f_clothes.fila_size) < MAX_CLOTHES)
      //{
      f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
      f_clothes.fila_size++;
      //}
   } else if (PORTDbits.RD1 == 1 && f_clothes.fila_size < MAX_CLOTHES) {
      // CRIA STRUCT ROUPA BRANCA
      clothes_control_t clothes;
      clothes.color = 1;
      clothes.washing_cicles = 2;
      clothes.state = 0;

      //PORTDbits.RD2 = 1;

      //if ((f_clothes.fila_size) < MAX_CLOTHES)
      //{
      f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
      f_clothes.fila_size++;
      //}
   }
   //else
   //{
   //   PORTDbits.RD2 = 0;
   //}
}

void check_wash() {
   // tentando botar roupa na maquina
   if (flag_wash == 0) {
      clothes_control_t next_clothes = f_clothes.clothes_waiting[0];

      for (int i = 0; i < MAX_CLOTHES - 1; i++) {
         f_clothes.clothes_waiting[i] = f_clothes.clothes_waiting[i + 1];
      }
      // Variaveis da fila de clothes
      f_clothes.fila_size--;
      f_clothes.clothes_washing = next_clothes;
      flag_wash = 1;

      // Variaveis da roupa a ser lavada
      next_clothes.state = 0;
      run_wash(next_clothes);
   }

   if (flag_wash == 1 && f_clothes.clothes_washing.state == 0) {
      run_wash(f_clothes.clothes_washing);
   }

   //else if (flag_wash == 1 && f_clothes.clothes_washing.state == 1)
   //{
   // ta limpa
   // check_passar()
   // }
}

void run_wash(clothes_control_t clothes_washing) {
   
    if (counter_wash < 200000) { // Se contador < 2000 a roupa ainda ta suja
      PORTDbits.RD2 = 1;
      counter_wash++;
   } else{
      clothes_washing.state = 1; //roupa limpa
      counter_wash = 0;
      //desligar led indicador da maquina sendo usada
      PORTDbits.RD2 = 0;
   }
}

void check_ironing() {
      PORTDbits.RD4 = 1;
   if (flag_wash == 1 && f_clothes.clothes_washing.state == 1 && flag_ironing == 0) {
      // Se roupa na maquina ja limpa e passar livre -> passar
      f_clothes.clothes_ironing = f_clothes.clothes_washing;
      flag_wash = 0;
      flag_ironing = 1;
      run_ironing(f_clothes.clothes_ironing);
   } else if (flag_wash == 1 && f_clothes.clothes_washing.state == 1 && flag_ironing == 1) {
      // se roupa na maquina ja limpa e passar nao livre -> espera
      run_ironing(f_clothes.clothes_ironing);
   }
}

void run_ironing(clothes_control_t clothes_ironing) {
   if (counter_ironing < 200000) {
      //PORTDbits.RD3 = 1;
      counter_ironing++;
   } else {
      flag_ironing = 0;
      counter_ironing = 0;
      f_clothes.clothes_washing.state = 2;
      f_clothes.clothes_finished[f_clothes.clothes_finished_size] = clothes_ironing;
      f_clothes.clothes_finished_size++;
         //PORTDbits.RD3 = 0;
   }
}