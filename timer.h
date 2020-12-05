/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */


#ifndef TIMER_H
#define	TIMER_H

#include "types.h"

void setup_timer_0();
void __interrupt() IRQ_Timer0();
u_int controle_delay();

#endif	/* TIMER_H */

