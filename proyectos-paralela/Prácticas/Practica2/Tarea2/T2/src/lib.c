#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "lib.h"

#include <omp.h>

const int N = 15000;

void imprime_datos_ejecucion(char* file_name, const int nproces, const double cpu_time_used, const double* elementos, const int nElementos){
    FILE *f;
    f = fopen("out.txt", "a");
    fprintf(f, "EJECUCION FECHA: %s HORA: %s\n" , __DATE__, __TIME__);
    fprintf(f, "Nombre del fichero de entrada: %s\n", file_name);
    fprintf(f, "Nombre del fichero de salida: %s\n", file_name);
    fprintf(f, "Elementos:\n");
    for (int i=0; i<nElementos; i++){
        fprintf(f, "\t > %f\n",elementos[i]);
    }
    fprintf(f, "Numero de procesos utilizado: %d\n", nproces);
    fprintf(f, "Tiempo de ejecución %f\n", cpu_time_used);
    fprintf(f, "--------------------------------------------\n\n");
    fclose(f);
}

void crear_fichero_nombre(char* nombre){
    FILE* f = fopen(nombre, "w");
    if (f == NULL){
        printf("Error creando el archivo.\n");
        exit(1);
    }
    fclose(f);
}

double *mult_matrices(double **M, double *v, int altura, int anchura){
    double *destino = (double*)malloc(anchura*sizeof(double));
    int i,j;
    double sum;
    for (i = 0; i < altura; i++){
        sum = 0;
        for (j = 0; j < anchura; j++){
            sum += M[i][j] * v[j];
        }
        destino[i] = sum;
    }
    return destino;
}

char* crear_fichero(){
    FILE* f = NULL;
    char* prefijo = "matriz(";
    char* sufijo = ").bin";
    int i = 0;
    char c[3];
    sprintf(c, "%d", i);
    char* nombre = (char*)malloc((strlen(prefijo) + strlen(c) + strlen(sufijo) + 1) * sizeof(char));
    nombre = strcpy(nombre, prefijo);
    nombre = strcat(nombre, c);
    nombre = strcat(nombre, sufijo);
    nombre = strcat(nombre, "\0");
    f = fopen(nombre, "r");
    // Si está este archivo, intentamos crear uno con otro numero
    while (f != NULL){
        fclose(f);
        free(nombre);
        i++;
        sprintf(c, "%d", i);
        nombre = (char*)malloc((strlen(prefijo) + strlen(c) + strlen(sufijo) + 1) * sizeof(char));
        nombre = strcpy(nombre, prefijo);
        nombre = strcat(nombre, c);
        nombre = strcat(nombre, sufijo);
        nombre = strcat(nombre, "\0");
        f = fopen(nombre, "r");
    }
    
    f = fopen(nombre, "w");
    if (f == NULL){
        printf("Error creando el archivo.\n");
        exit(1);
    }
    fclose(f);

    return nombre;
}

double **crear_matriz(int altura, int anchura, int desp, int semilla) {
    double **destino;
    srand(semilla);
    destino = (double**)malloc(altura*sizeof(double));
    int i,j;
    destino[0] = (double*)malloc(anchura*altura*sizeof(double));
    for (i = 1; i < altura; i++){
        destino[i] = destino[i-1] + anchura;
    }

    for ( i = 0; i < altura; i++){
        for ( j = 0; j < anchura; j++){
            destino [i][j] = rand() % 50;
            if (i < j) destino[i][j] *= -1;
            else if (i==j) destino[i][j] = 1;
        }
    }

    return destino;
}

void rellenar_fichero(char* nombre, double** matriz, int tam){
    FILE* f = fopen(nombre, "wb");
    if (f == NULL){
        printf("Error abriendo el archivo.\n");
        exit(1);
    }
    fwrite(matriz[0], sizeof(double), tam*tam, f);
    fclose(f);
}

void algoritmo_iterativo(double** origen, double* vx, int altura, int anchura, int iteraciones){
    double *aux = (double*)malloc(anchura*sizeof(double));
    double *destino = (double*)malloc(anchura*sizeof(double));
    double max_vabs=0;
    int pos = -1;
    double sum;
    double *temp;

    int i,j,k;
    aux = vx;

    for ( i=0; i <= iteraciones; i++){
        max_vabs = 0;
        for ( j = 0; j < altura; j++){
            sum = 0;
            for ( k = 0; k < anchura; k++){
                sum += origen[j][k] * aux[k];
            }
            destino[j] = sum;
            if (i==0) continue;

            if (fabs(destino[j]) > max_vabs){
                max_vabs = fabs(destino[j]);
                pos = j;
            }
        }
        temp = aux;
        aux = destino;
        destino = temp;

        if (max_vabs > 0) printf("Iteracion %d, valor absoluto maximo: %f, posicion: %d\n", i, max_vabs, pos);


        // Si el valor absoluto maximo es mayor a 25.0, divido todo el vector dicho valor
        if (max_vabs > 25.0) {
            for (int j = 0; j < anchura; j++){
                aux[j] /= max_vabs;
            }
        }
    }
}