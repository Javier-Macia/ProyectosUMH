#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <errno.h>
#include <stdlib.h>

#include "../inc/sem.h"

#define PERMISOS 0644

typedef union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
} semun;

int sGet(int seed){
    key_t semKey = ftok("/bin/ls", seed);
    if(semKey == -1){
        perror("Error al crear el token ");
        exit(-1);
    }
    int semid = semget(semKey, 1, IPC_CREAT | PERMISOS);
    if(semid == -1){
        perror("Error al obtener el semID ");
        exit(-1);
    }
    return semid;
}

void sSet(int semid, int value){
    semctl(semid, 0, SETVAL, value);
}

int sCreate(int seed, int value){
    int semID = sGet(seed);
    sSet(semID, value);
    return semID;
}

void sWait(int semid){
    struct sembuf op_Wait [] = { 0, -1, 0 }; // Decrementa en 1 el semaforo
    semop(semid, op_Wait, 1);
}

void sSignal(int semid){
    struct sembuf op_Signal [] = { 0, 1, 0 }; // Incrementa en 1 el semaforo
    semop(semid, op_Signal, 1);
}

void sDestroy(int semid){
    semun dummy;
    int ret;
    ret = semctl(semid, 0, IPC_RMID, dummy);
    if(ret == -1){
        perror("Error al destruir el semaforo ");
        exit(-1);
    }
}