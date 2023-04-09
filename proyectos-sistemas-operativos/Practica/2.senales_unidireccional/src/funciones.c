#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>

#include "../inc/funciones.h"

extern unsigned int signal_count;
extern int pidsHijos[];

union sigval v;

// MANEJADORES -------------------------------
// PADRE - MANEJADOR PARA LAS SEÑALES EN TIEMPO REAL

/* Esta funcion es el manejador de señales normales
* cAqui el padre comunica que ya se ha creado la combinacion ganadora
*/
void sorteoRealizado(int pid){
    kill(pid, SIGUSR1);
    printf("Se ha mandado ACK al hijo %d\n", pid);
}

// HIJOS - MANEJADOR DE SEÑALES NORMALES
/* Esta funcion es el manejador de señales en tiempo real
* cada hijo enviara al padre una señal diciendo que ha apostado
* aqui contaremos cuantos hijos han contestado en una variable global 
* que habra que definir en el main */
void hijoApuesta(){
    v.sival_int = getpid();
    sigqueue(getppid(), SIGRTMIN, v);
    printf("Señal de %d enviada al padre (%d)\n",getpid(), getppid());
}

// FUUNCIONES -----------------------------------
// FUNCIONES GENERICAS PADRES E HIJOS
bool esNumero(char* cadena) {
    int i = 0;
    while(cadena[i] != '\0') {
        if(cadena[i] < '0' || cadena[i] > '9') {
            return false;
        }
        i++;
    }
    return true;
}

bool argumentosValidos(int argc, char *argv[]) {
    if(argc != 3) {
        printf("ERROR: Numero de argumentos incorrecto\n");
        return false;
    }
    if(!esNumero(argv[1]) || !esNumero(argv[2])) {
        printf("ERROR: Argumentos no numericos\n");
        return false;
    }
    return true;
}

bool estaDisponible(int num, int* array, int size) {
    for(int i = 0; i < size; i++) {
        if(array[i] == num) {
            return false;
        }
    }
    return true;
}

/* Genera una combinacion de 6 numeros aleatorios entre 0 y 9
* y los guarda en el array combinacion */
void generarCombinacion(int combinacion[], int seed) {
    int temp;    
    srand(seed);
    int i= 0;
    while (i<NUM_COMBINACIONES){
        temp = rand()%10;
        if(!estaDisponible(temp, combinacion, i)){
            continue;
        }
        combinacion[i] = temp;
        i++;
    }
}

/* Muestra la combinacion generada */
void mostrarCombinacion(int combinacion[]){
    for(int i = 0; i < NUM_COMBINACIONES; i++) {
        printf("%d ", combinacion[i]);
    }
    printf("\n");
}

// FUNCIONES SOLO PADRE
void manejadorSenal(int sig, siginfo_t *info, void *context){
    if (sig == SIGUSR1) {  // Señal recibida por el hijo (enviada por el padre)
        printf("Hijo %d recibe ACK del padre\n", getpid());
    }else if (sig == SIGRTMIN) { // Señal recibida por el padre (enviada por el hijo)
        pidsHijos[signal_count] = info->si_pid;
        signal_count++;
        printf("Padre recibe señal de %d (SC=%d)\n", info->si_pid, signal_count);
    }
}

void mostrarPIDS(){
    printf("PIDS: ");
    for(int i = 0; i < signal_count; i++) {
        printf(" %d ", pidsHijos[i]);
    }
    printf("\n");
}

bool crearFichero(HIJO* hijos,int numProcesos,int numSorteo){
    char  nombre[5];
    FILE* f;
    sprintf(nombre, "%s%d%s", "S", numSorteo, "R");
    f = fopen(nombre, "w");
    if(f == NULL){
        printf("Error al crear el fichero\n");
        return false;
    }
    for(int i = 0; i < numProcesos; i++){
        fprintf(f, "JUGADOR %d (PID: %d) ha ganado %d€\n", i, hijos[i].pid, hijos[i].premio);
    }
    fclose(f);

    return true;
}

/*Esta función sirve para que el padre, antes de crear los
* hijos y sabiendo cuántos son, cree todas las tuberías. */
void crearTuberias(int numProcesos, HIJO* hijos) {
    int i;
    for(i = 0; i < numProcesos; i++) {
        if(pipe(hijos[i].pipeHijo) == -1) {
            printf("ERROR: No se pudo crear la tuberia\n");
            exit(1);
        }
    }
} 

/* Esta función servirá al padre para enviar la combinación ganadora
* por la tubería correspondiente al hijo, siendo numProcesos el indice del
* hijo en el array de tuberias */
void comunicarApuesta(int nHijos, HIJO* hijo, unsigned int *ganadora){
    int i=0,j=0;
    for ( i = 0; i < nHijos; i++){
        close(hijo[i].pipeHijo[0]); // Cerramos el descriptor de lectura

        for (j = 0; j < NUM_COMBINACIONES; j++){
            write(hijo[i].pipeHijo[1], &ganadora[j], sizeof(unsigned int));
        }
        
        close(hijo[i].pipeHijo[1]); // Cerramos el descriptor de escritura
    }
}

/* Esta función servirá al padre para saber que un hijo ha realizado
* la apuesta */
void apuestaRealizada(){

}

/* FUNCIONES HIJOS */

/* Esta función la usaran los hijos para comprobar el numero
* de aciertos de su combinación */
int comprobarCombinacion(unsigned int* ganadora, unsigned int *apuesta){
    int nAciertos = 0;
    int j = 0;
    for(int i = 0; i < NUM_COMBINACIONES; i++) {
        for ( j = 0; j < NUM_COMBINACIONES; j++){
            if(ganadora[j] == apuesta[i]) {
                nAciertos++;
            }
        }
    }
    return nAciertos;
}

/* Esta función la usaran los hijos para calcular el premio
* de su apuesta */
int calcularPremio(int numAciertos){
    int premio = 0;
    switch(numAciertos) {
        case 3:
            premio = 10;
            break;
        case 4:
            premio = 50;
            break;
        case 5:
            premio = 500;
            break;
        case 6:
            premio = 10000;
            break;
        default:
            premio = 0;
            break;
    }
    return premio;
}

/* Esta función la usaran los hijos para leer la combinación ganadora
* de su apuesta */
void leerGanadora(HIJO hijo, unsigned int ganadora[]){
    close(hijo.pipeHijo[1]); // Cerramos el descriptor de lectura

    for (int i=0; i<NUM_COMBINACIONES; i++){
        read(hijo.pipeHijo[0], &ganadora[i], sizeof(int)); // Leemos la combinación de la apuesta
    }

    close(hijo.pipeHijo[0]); // Cerramos el descriptor de lectura

    return ganadora;
}