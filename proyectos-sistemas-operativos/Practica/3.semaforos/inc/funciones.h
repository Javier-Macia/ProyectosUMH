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

/* FUNCIONES SOLO PADRE */
void crearTuberias(int numHijos, HIJO* hijos);
void comunicarApuesta(int nHijos, HIJO* hijo, unsigned int *ganadora);
void apuestaRealizada();
bool crearFichero(HIJO* hijos,int numHijos,int numSorteo);

/* FUNCIONES HIJOS*/
int comprobarCombinacion(unsigned int ganadora[], unsigned int *apuesta);
int calcularPremio(int numAciertos);
void leerGanadora(HIJO hijo, unsigned int ganadora[]);

#endif