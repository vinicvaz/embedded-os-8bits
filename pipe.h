/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef PIPE_H
#define	PIPE_H

#include "types.h"

typedef struct pipe {
    u_char* buffer;
    u_int buffer_size;
    u_int itens;
    u_int pos_read;
    u_int pos_write;    
} pipe_t;


// Rotinas de manipulação do PIPE
void pipe_create(pipe_t* p, u_int size);
u_char pipe_read(pipe_t* p);
void pipe_write(pipe_t* p, u_char data);


#endif	/* PIPE_H */

