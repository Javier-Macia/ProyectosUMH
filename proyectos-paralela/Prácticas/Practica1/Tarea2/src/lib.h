#include <stdio.h>

#ifndef __LIB_H__
#define __LIB_H__

extern const int N;

void imprime_datos_ejecucion(char* file_name, const int iteraciones, const int nproc, const double cpu_time_used, const double* elementos, const int nElementos);
char* crear_fichero();
void crear_fichero_nombre(char *nombre);
void rellenar_fichero(char *nombre, double** matriz, int tam);
double **crear_matriz(int altura, int anchura, int desp, int semilla);
double valor_absoluto(double x);
void algoritmo_iterativo(double** origen, double* vx, int altura, int anchura, int iteraciones);
void mult_vectores(double **M, double *v, double resultado[], int altura, int anchura);

#endif