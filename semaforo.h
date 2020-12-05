/**
 * Projeto Sistema Operacional Embarcado @2020 
 *  
 */

#ifndef SEMAFORO_H
#define	SEMAFORO_H

#include "config.h"
#include "types.h"

// Fila de espera do semáforo
typedef struct fila_sem {
    u_int tasks[MAX_TASKS];
    u_int queue_size;
    u_int next_to_free;
} sem_queue_t;

// Tipo Abstrato de Dado do Semáforo
typedef struct semaforo {
    int contadorSem;
    sem_queue_t fila; 
} sem_t;

// Rotinas para manipulação do semáforo
void sem_init(sem_t *sem, u_int valor);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);
int sem_get_value(sem_t sem);


#endif	/* SEMAFORO_H */

