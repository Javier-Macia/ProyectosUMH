#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/types.h>

#include "../inc/funciones.h"
#include "../inc/sem.h"

static unsigned int contador=0;
int pidsHijos[20]; // Array con los PIDs de los hijos


int main (int argc, char *argv[]) {
    /* Control de argumentos */
    if (!argumentosValidos(argc, argv)){
        return -1;
    }


    /* Creacion de los semaforos */
    /* Inicializacion de variables */
    int numSorteo = atoi(argv[1]);
    int numHijos = atoi(argv[2]);

    unsigned int ganadora[NUM_COMBINACIONES]; // Array que guardara la combinacion ganadora
    time_t t;
    int pid;
    int status; // Status de los hijos
    int semid; // ID del semaforo

    /* Creacion del semaforo */
    sCreate(&semid, 4);



    HIJO hijos[atoi(argv[2])];

    /*  Inicializamos las estructuras de los hijos */
    for (int i=0; i<numHijos; i++){
        hijos[i].pid=0;
        hijos[i].num=0;
        hijos[i].premio=0;
        pidsHijos[i]=0;
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
            hijos[i].pid = getpid();
            printf("Se ha creado el hijo %d\n", getpid());
            printf("\t>Hijo con PID: %d va a generar una combinacion\n", hijos[i].pid);
            generarCombinacion(hijos[i].combinacion,rand());
            printf("\t>La combinacion generada es: ");
            mostrarCombinacion(hijos[i].combinacion);

            /* Avisamos al padre de que tenemos nuestra combinacion */
            printf("\t>Hijo con PID: %d va a avisar de que ha hecho una combinacion\n", hijos[i].pid);            
            sWait(semid); // Espera a que el padre ponga la combinacion ganadora
            
            /* Leemos del pipe la combinación ganadora */
            printf("\t>Hijo con PID: %d va a leer la combinacion ganadora\n", hijos[i].pid);
            leerGanadora(hijos[i], ganadora);
            
            printf("\t>Hijo %d con PID: %d HA LEIDO la combinacion ganadora\n",i, getpid());
            printf("\t>La combinacion ganadora es: ");
            mostrarCombinacion(ganadora);
            printf("Su combinacion era: ");
            mostrarCombinacion(hijos[i].combinacion);

            hijos[i].num_aciertos = comprobarCombinacion(ganadora, hijos[i].combinacion);
            printf("Hijo con PID: %d ha acertado %d numeros\n", hijos[i].pid, hijos[i].num_aciertos);
            hijos[i].premio = calcularPremio(hijos[i].num_aciertos);
            printf("Hijo con PID: %d ha ganado %d euros\n", hijos[i].pid, hijos[i].premio);
            
            printf("\t>Hijo con PID: %d termina. \n", hijos[i].pid);
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
    printf("El padre genera una combinacion\n");
    generarCombinacion(ganadora, rand());
    printf("La combinacion ganadora es: ");
    mostrarCombinacion(ganadora);

    printf("El padre ha escrito la combinacion ganadora en los pipes\n");
    comunicarApuesta(numHijos, hijos, ganadora);

    /* Enviamos ACK's a los hijos */
    sSignal(semid);

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
    
    // pkill -f sorteo

    printf("EJECUCION DEL SORTEO %d TERMINADA\n", numSorteo);

    return 0;
}