#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>

#ifndef __SEM_HPP__
#define __SEM_HPP__

// Creacion del semaforo
int sCreate(int seed, int value);

// Incremento y decremento del semaforo
void sWait(int semid);
void sSignal(int semid);
void sDestroy(int semid);
#endif // __SEM_HPP__