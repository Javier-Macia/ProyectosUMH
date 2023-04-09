//*** NO MODIFICAR ESTE CONTENIDO
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/ipc.h>   
#include <sys/sem.h>   
#include <sys/shm.h>  

#define SPERMISOS 0600

int sGet(char seed)
{
	int semID;
	key_t semKey = ftok("/bin/ls", seed);
	if ((semID = semget(semKey,1,IPC_CREAT | SPERMISOS))==-1){
		perror("IPC error: ftok"); 
		exit(-1);
	}
	return semID;
}

void sSet(int semID, int value)
{
	semctl(semID, 0, SETVAL, value);
}

int sCreate(int seed, int value)
{
	int semID = sGet(seed);
	sSet(semID, value);
	return semID;
}

void sWait(int semID)
{
	struct sembuf op_Wait [] = 	{ 0, -1, 0 }; // Decrementa en 1 el semáforo 
	semop ( semID, op_Wait, 1 );
}

void sSignal(int semID)
{
	struct sembuf op_Signal [] = { 0, 1, 0 };	// Incrementa en 1 el semáforo 
	semop ( semID, op_Signal, 1 );
}

//*** A PARTIR DE AQUI PUEDES PONER LO QUE QUIERAS, SI LO NECESITAS
