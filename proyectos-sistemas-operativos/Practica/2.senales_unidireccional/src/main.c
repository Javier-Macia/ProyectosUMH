#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/types.h>

#include "../inc/funciones.h"

unsigned int signal_count=0;
int pidsHijos[20]; // Array con los PIDs de los hijos
unsigned int ganadora[NUM_COMBINACIONES]; // Array que guardara la combinacion ganadora



int main (int argc, char *argv[]) {
    /* Control de argumentos */
    if (!argumentosValidos(argc, argv)){
        return -1;
    }

    /* Inicializacion de variables */
    int numSorteo = atoi(argv[1]);
    int numProcesos = atoi(argv[2]);

    int fd[2]; // File descriptor de la tuberia. fd[0] = read, fd[1] = write
    time_t t;
    int pid;
    int status; // Status de los hijos

    struct sigaction manejador;
    manejador.sa_sigaction = manejadorSenal; // Nombre de la función manejadora */
    manejador.sa_flags = SA_SIGINFO;
    sigaction(SIGRTMIN, &manejador, NULL); // Se asigna la función manejadora a la senal SIGRTMIN

    manejador.sa_sigaction = manejadorSenal;
    manejador.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &manejador, NULL);


    HIJO hijos[atoi(argv[2])];

    /*  Inicializamos las estructuras de los hijos */
    for (int i=0; i<numProcesos; i++){
        hijos[i].pid=0;
        hijos[i].num=0;
        hijos[i].premio=0;
        pidsHijos[i]=0;
    }


    crearTuberias(numProcesos, hijos);

    /* Creamos el bucle principal donde se llama a fork en cada iteracion del padre */
    for (int i = 0; i < numProcesos; i++){
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
            hijoApuesta(); // Envia Señal al padre
            //printf("\t>Hijo con PID: %d espera un ACK. \n", hijos[i].pid);
            //pause();

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

    
    while (signal_count < numProcesos){
        printf("El padre espera a una senal (SC=%d) (numProcesos=%d).\n",signal_count, numProcesos);
        pause();
        mostrarPIDS();
    }
    
    

    printf("El padre ha escrito la combinacion ganadora en los pipes\n");
    comunicarApuesta(numProcesos, hijos, ganadora);


    /* Enviamos ACK's a los hijos */
    //sleep(2);
    for (int i = 0; i < numProcesos; i++){
        printf("Padre dice a hijo %d de que tiene la GANADORA (#%d)\n",pidsHijos[i], i);
        sorteoRealizado(pidsHijos[i]);
    }

    //sleep(1);
    for (int i = 0; i < numProcesos; i++){
        printf("Padre espera a que los hijos terminen(#%d)\n", i);
        hijos[i].pid = waitpid(-1, &status, 0);
        printf("El hijo con PID: %d ha terminado\n", hijos[i].pid);
        hijos[i].premio = calcularPremio(WEXITSTATUS(status));
        // El limite del retorno de un hijo es 255, por tanto
        // se devuelve unicamente el numero de aciertos y
        // fuera de este se calcula el premio.
    }
    
    crearFichero(hijos, numProcesos, numSorteo);
    
    // pkill -f sorteo

    printf("EJECUCION DEL SORTEO %d TERMINADA\n", numSorteo);

    return 0;
}