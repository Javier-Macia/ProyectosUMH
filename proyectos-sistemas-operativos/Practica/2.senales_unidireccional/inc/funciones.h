#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>

#ifndef __FUNCIONES_HPP__
#define __FUNCIONES_HPP__

#define NUM_COMBINACIONES 6

typedef struct {
    int pid; /* Pid del hijo */
    int num; /* Numero de orden del hijo */
    int num_aciertos; /* Numero de aciertos */
    int pipeHijo[2]; /* Pipe para comunicar con el hijo */
    unsigned int combinacion[NUM_COMBINACIONES]; /* Combinacion del hijo */
    long premio; /* Premio del hijo */
}HIJO;

/* FUNCIONES ----------------------------------- */
/* FUNCIONES GENERICAS PADRES E HIJOS */
bool esNumero(char* cadena);
bool argumentosValidos(int argc, char *argv[]);
void generarCombinacion(int combinacion[], int seed);
void mostrarCombinacion(int combinacion[]);

/* MANEJADORES -------------------------------  */
/* PADRE - MANEJADOR PARA LAS SEÑALES EN TIEMPO REAL */
void sorteoRealizado(int pid);
void manejadorSenal(int sig, siginfo_t *info, void *context);
void mostrarPIDS();

/* HIJOS - MANEJADOR DE SEÑALES NORMALES */
void hijoApuesta();

/* FUNCIONES SOLO PADRE */
void crearTuberias(int numProcesos, HIJO* hijos);
void comunicarApuesta(int nHijos, HIJO* hijo, unsigned int *ganadora);
void apuestaRealizada();
bool crearFichero(HIJO* hijos,int numProcesos,int numSorteo);

/* FUNCIONES HIJOS*/
int comprobarCombinacion(unsigned int ganadora[], unsigned int *apuesta);
int calcularPremio(int numAciertos);
void leerGanadora(HIJO hijo, unsigned int ganadora[]);

#endif