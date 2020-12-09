/* 
 * File:   resources.h
 * Author: Giulio
 *
 * Created on 8 de Dezembro de 2020, 15:13
 */

#ifndef RESOURCES_H
#define	RESOURCES_H

#include "types.h"


typedef struct wash_machine
{
    u_int cunter_wash;
    u_int flag_wash; //0->desocupada; 1->ocupada;
} wash_machine_t;

typedef struct iron_machine
{
    u_int counter_iron;
    u_int flag_iron; //0->desocupada; 1->ocupada;
} iron_machine_t;



#endif	/* RESOURCES_H */

