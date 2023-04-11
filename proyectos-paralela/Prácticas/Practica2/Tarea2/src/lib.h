#include <stdio.h>

#ifndef __LIB_H__
#define __LIB_H__

extern const int N;

void imprime_datos_ejecucion(char* file_name, const int nproces, const double cpu_time_used, const double* elementos, const int nElementos);
char* crear_fichero();
void crear_fichero_nombre(char *nombre);
void rellenar_fichero(char *nombre, double** matriz, int tam);
double **crear_matriz(int altura, int anchura, int desp, int semilla);
void algoritmo_iterativo(double** origen, double* vx, int altura, int anchura, int iteraciones);
double *mult_matrices(double **M, double *v, int altura, int anchura);

#endif