#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../inc/sem.h"
#include "../inc/funciones.h"

#define SSEED 99
#define SHMSEED 35
#define SHMPERMISOS 0644

int pidsHijos[20]; // Array con los PIDs de los hijos


int main (int argc, char *argv[]) {
    /* Control de argumentos */
    if (!argumentosValidos(argc, argv)){
        return -1;
    }

    /* Inicializacion de variables */
    int numSorteo = atoi(argv[1]);
    int numHijos = atoi(argv[2]);

    /* Creacion de los semaforos */
    int sBarreraPadre = sCreate(SSEED, 0);
    int sACKS[numHijos];

    time_t t;
    int pid;
    int status; // Status de los hijos

    /* Creamos la sección de memoria compartida */
    int shmId, shmSize;
    int *shmArray;
    shmSize = sizeof(int) * NUM_COMBINACIONES;
    shmId = shmget(ftok("/bin/ls",SHMSEED), shmSize, IPC_CREAT | SHMPERMISOS);
    shmArray = (int *) shmat(shmId, 0, 0);
    memset(shmArray, 0, shmSize);
    int *ganadora = shmArray;

    HIJO hijos[atoi(argv[2])];

    /*  Inicializamos las estructuras de los hijos */
    for (int i=0; i<numHijos; i++){
        hijos[i].pid=0;
        hijos[i].num=0;
        hijos[i].premio=0;
        pidsHijos[i]=0;
        sACKS[i]=sCreate(SSEED+i+1,0);
    }


    crearTuberias(numHijos, hijos);

    /* Creamos el bucle principal donde se llama a fork en cada iteracion del padre */
    for (int i = 0; i < numHijos; i++){
        pid = fork();
        srand(getpid()%time(&t));

        // Codigo del padre
        switch (pid){
        case 0: // Codigo del hijo
            /* Generamos una combinación para este proceso */
            int miCombinacionGanadora[NUM_COMBINACIONES];
            hijos[i].pid = getpid();
            printf("Se ha creado el hijo %d\n", getpid());
            printf("\t(%d)>Hijo con PID: %d va a generar una combinacion\n",i, hijos[i].pid);
            generarCombinacion(hijos[i].combinacion,rand());
            printf("\t(%d)>La combinacion generada es: ",i);
            mostrarCombinacion(hijos[i].combinacion);

            /* Avisamos al padre de que tenemos nuestra combinacion */
            printf("\t(%d)>Hijo con PID: %d va a avisar de que ha hecho una combinacion\n",i, hijos[i].pid); 
            sSignal(sBarreraPadre);
            printf("sBarreraPadre despues de señal: %d\n", sBarreraPadre);

            /* Leemos del pipe la combinación ganadora */
            // Hijo espera a que el padre le envie la combinacion ganadora
            printf("\t(%d)>Hijo con PID: %d ESPERA a leer\n",i, hijos[i].pid);
            printf("------->Hijo %i semaforo %d\n", i, sACKS[i]);
            sWait(sACKS[i]);
            printf("------->Hijo %i WAIT HECHO %d\n",i, sACKS[i]);

            for (int j=0; j<NUM_COMBINACIONES; j++){
                miCombinacionGanadora[j] = ganadora[j];
            } 
            printf("\t(%d)>Hijo con PID: %d va a leer la combinacion ganadora\n",i, hijos[i].pid);
            printf("\t(%d)>Hijo con PID: %d HA LEIDO la combinacion ganadora\n",i, getpid());
            printf("\t(%d)>La combinacion ganadora es: ",i);
            mostrarCombinacion(miCombinacionGanadora);
            printf("Su combinacion era: ");
            mostrarCombinacion(hijos[i].combinacion);

            printf ("\t(%d)>Hijo se desvincula de la memoria compartida\n",i, i);
            shmdt(shmArray);

            hijos[i].num_aciertos = comprobarCombinacion(miCombinacionGanadora, hijos[i].combinacion);
            printf("\t(%d)>Hijo con PID: %d ha acertado %d numeros\n",i, hijos[i].pid, hijos[i].num_aciertos);
            hijos[i].premio = calcularPremio(hijos[i].num_aciertos);
            printf("\t(%d)>Hijo con PID: %d ha ganado %d euros\n",i, hijos[i].pid, hijos[i].premio);
            
            printf("\t>Hijo con PID: %d termina. \n",i, hijos[i].pid);
            return(hijos[i].num_aciertos);

        case -1: // Error al crear el hijo
            perror("Error al crear uno de los hijos");
            exit(-1);
        
        default: // Codigo del padre
            pidsHijos[i] = pid;
            break;
        }
    }

    /* A esta parte del código solo llega el padre */

    /* Esperamos a que todos los hijos lleguen a la barrera */
    printf("El padre espera a que todos los hijos lleguen a la barrera\n");
    for (int i = 0; i < numHijos; i++){
        printf("sBarreraPadre %d\n", sBarreraPadre);
        sWait(sBarreraPadre);
        printf("Un hijo ha llegado a la barrera (%d)\n", i);
        printf("sBarreraPadre v2 %d\n", sBarreraPadre);
    }

    printf("El padre genera una combinacion\n");
    generarCombinacion(shmArray, rand());
    printf("La combinacion ganadora es: ");
    mostrarCombinacion(ganadora);

    //comunicarApuesta(numHijos, hijos, shmArray);
    //printf("El padre ha escrito la combinacion ganadora en los pipes\n");

    /* Desbloqueamos a los hijos */
    for (int i = 0; i < numHijos; i++){
        sSignal(sACKS[i]);
    }

    for (int i = 0; i < numHijos; i++){
        printf("Padre espera a que los hijos terminen(#%d)\n", i);
        hijos[i].pid = waitpid(-1, &status, 0);
        printf("El hijo con PID: %d ha terminado\n", hijos[i].pid);
        hijos[i].premio = calcularPremio(WEXITSTATUS(status));
        // El limite del retorno de un hijo es 255, por tanto
        // se devuelve unicamente el numero de aciertos y
        // fuera de este se calcula el premio.
    }
    
    crearFichero(hijos, numHijos, numSorteo);
    // Liberamos la memoria compartida
    printf("El padre se desvincula de la memoria compartida.\n");
    shmdt(shmArray);
    printf("El padre libera la memoria compartida.\n");
    shmctl(shmId, IPC_RMID, NULL);
    
    
    printf("EJECUCION DEL SORTEO %d TERMINADA\n", numSorteo);
    return 0;
}