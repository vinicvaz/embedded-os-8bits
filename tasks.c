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

extern fila_clothes_t f_clothes;
extern machine_t w_machine;
extern machine_t i_machine;
u_int flag = 0;
//u_int flag_wash = 0;

void config_tasks() {
   //TRISDbits.RD0 = 1; // colorido
   //TRISDbits.RD1 = 1; // branco
   TRISDbits.RD2 = 0; // maquina
   TRISDbits.RD3 = 0; // ferro de passar
   TRISDbits.RD4 = 0; // tudo ligado
 
   TRISBbits.TRISB0 = 0; // botão roupa colorida
   TRISBbits.TRISB1 = 0; // botão roupa branca

}

void color_clothes()
{
      clothes_control_t clothes;
      clothes.color = 0; //colorida
      clothes.washing_cycles = 1; //1 ciclo de lavagem
      clothes.state = 0; //roupa está suja

      f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
      f_clothes.fila_size++;

      INTCONbits.INT0IF = 0; //volta botão para 0
      ENABLE_INTERRUPTS();
}

void white_clothes()
{
      clothes_control_t clothes;
      clothes.color = 1; //roupa branca
      clothes.washing_cycles = 2; //2 ciclos de lavagem
      clothes.state = 0; //roupa está suja

      f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
      f_clothes.fila_size++;
      INTCON3bits.INT1IF = 0;
      ENABLE_INTERRUPTS();
}
/*void task_read_buttons() {
   while (1) {
      if (PORTDbits.RD0 == 0 && PORTDbits.RD1 == 0) {
         flag = 0;
      }

      //PORTDbits.RD4 = ~PORTDbits.RD4;
      if (PORTDbits.RD0 == 1 && f_clothes.fila_size < MAX_CLOTHES && flag == 0) {
         // CRIA STRUCT ROUPA COLORIDA
         clothes_control_t clothes;
         clothes.color = 0;
         clothes.washing_cycles = 1;
         clothes.state = 0;

         f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
         f_clothes.fila_size++;
         flag = 1;
      } else if (PORTDbits.RD1 == 1 && f_clothes.fila_size < MAX_CLOTHES && flag == 0) {
         // CRIA STRUCT ROUPA BRANCA
         clothes_control_t clothes;
         clothes.color = 1;
         clothes.washing_cycles = 2;
         clothes.state = 0;

         f_clothes.clothes_waiting[f_clothes.fila_size] = clothes;
         f_clothes.fila_size++;
         flag = 1;
      }
   }
} */

void check_wash() {
   while (1) {
      // tentando botar roupa na maquina
      if (w_machine.flag == 0 && f_clothes.fila_size > 0) {

         clothes_control_t next_clothes = f_clothes.clothes_waiting[0];

         for (int i = 0; i < MAX_CLOTHES - 1; i++) {
            f_clothes.clothes_waiting[i] = f_clothes.clothes_waiting[i + 1];
         }
         // Variaveis da fila de clothes
         f_clothes.fila_size--;
         f_clothes.clothes_washing = next_clothes;
         w_machine.flag = 1;
         PORTDbits.RD4 = ~PORTDbits.RD4;

         // Variaveis da roupa a ser lavada
         next_clothes.state = 0; //roupa suja
         run_wash(); //next_clothes
      }

      if (w_machine.flag == 1 && f_clothes.clothes_washing.state == 0) {
         run_wash(); //f_clothes.clothes_washing
      }
   }
}

void run_wash() //clothes_control_t clothes_washing
{
   if (w_machine.counter < CYCLES_LIFE) { //Se contador < 2000 a roupa ainda ta suja
      PORTCbits.RC4 = 1;
      PORTCbits.RC5 = 1;
      PORTDbits.RD2 = 1;
      w_machine.counter++;
   }
   if (w_machine.counter >= CYCLES_LIFE && f_clothes.clothes_washing.washing_cycles == 1)
      //if (w_machine.counter >= CYCLES_LIFE)
   { //desligar led indicador da maquina sendo usada
      PORTCbits.RC4 = 0;
      PORTCbits.RC5 = 0;

      PORTDbits.RD2 = 0;
      f_clothes.clothes_washing.state = 1; //roupa limpa
      w_machine.counter = 0;
      w_machine.flag = 1;
      i_machine.flag = 0;
   }
   if (w_machine.counter >= CYCLES_LIFE && f_clothes.clothes_washing.washing_cycles > 1) {
      PORTDbits.RD2 = 0;
      f_clothes.clothes_washing.washing_cycles--;
      w_machine.counter = 0;
   }
}

void check_ironing() {
   while (1) {
      if (w_machine.flag == 1 && f_clothes.clothes_washing.state == 1 && i_machine.flag == 0) {
         //PORTDbits.RD3 = 1;
         // Se roupa na maquina ja limpa e passar livre -> passar
         f_clothes.clothes_ironing = f_clothes.clothes_washing;
         w_machine.flag = 0;
         i_machine.flag = 1;
         run_ironing(f_clothes.clothes_ironing);
      }
      //else
      if (f_clothes.clothes_washing.state == 1 && i_machine.flag == 1) {
         //PORTDbits.RD3 = 1;
         // se roupa na maquina ja limpa e passar nao livre -> espera
         run_ironing(f_clothes.clothes_ironing);
      }
   }
}

void run_ironing(clothes_control_t clothes_ironing) {
   if (i_machine.counter < CYCLES_LIFE) {
      PORTDbits.RD3 = 1;
      i_machine.counter++;
   }

   if (i_machine.counter >= CYCLES_LIFE) {
      PORTDbits.RD3 = 0;
      i_machine.flag = 0;
      i_machine.counter = 0;
      f_clothes.clothes_washing.state = 2;
      f_clothes.clothes_finished[f_clothes.clothes_finished_size] = clothes_ironing;
      f_clothes.clothes_finished_size++;
   }
}
