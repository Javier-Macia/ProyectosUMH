#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "lib.h"

const int N = 150;

void imprime_datos_ejecucion(char* file_name, const int iteraciones, const int nproc, const double cpu_time_used, const double* elementos, const int nElementos){
    FILE *f;
    f = fopen("out.txt", "a");
    fprintf(f, "EJECUCION FECHA: %s HORA: %s\n" , __DATE__, __TIME__);
    fprintf(f, "Nombre del fichero de entrada: %s\n", file_name);
    fprintf(f, "Nombre del fichero de salida: %s\n", file_name);
    fprintf(f, "Iteraciones: %d\n", iteraciones);
    fprintf(f, "Numero de procesos: %d\n", nproc);
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

void mult_vectores(double **M, double *v, double resultado[], int altura, int anchura){
    double num;
    double res[altura];
    for (int i = 0; i < altura; i++){
        res[i] = 0;
        for (int j = 0; j < anchura; j++){
            res[i] += M[i][j] * v[i];
        }
        resultado[i] = res[i];
    }
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
    /**
    for (int i = 0; i < altura; i++){
        destino[i] = (double*)malloc(anchura*sizeof(double));
        for (int j = 0; j < anchura; j++){
            destino[i][j] = rand() % 50;
            if (i < j) destino[i][j] *= -1;
        }
        // Desplaza la diagonal principal un número de posiciones indicado por desp
        destino[i][desp+i] = 1;

    }
    /**/
    int cont=0;
    destino[0] = (double*)malloc(anchura*altura*sizeof(double));
    for (int i = 0; i < altura; i++){
        for (int j = 0; j < anchura; j++){
            destino [0][cont] = rand() % 50;
            if (i < j) destino[0][cont] *= -1;
            if (i==j) destino[0][cont] = 1;
            cont++;
        }
        // Desplaza la diagonal principal un número de posiciones indicado por desp
        if (i>0)
            destino[i] = destino[i-1] + anchura;
    }

    return destino;
}

void rellenar_fichero(char* nombre, double** matriz, int tam){
    FILE* f = fopen(nombre, "wb");
    if (f == NULL){
        printf("Error abriendo el archivo.\n");
        exit(1);
    }
    for (int i = 0; i < tam; i++)
        fwrite(matriz[i], sizeof(double), tam, f);
    fclose(f);
}

double valor_absoluto(double x) {
    if (x < 0) return -x;
    return x;
}

void algoritmo_iterativo(double** origen, double* vx, int altura, int anchura, int iteraciones){
    double *aux = (double*)malloc(anchura*sizeof(double));
    double *destino = (double*)malloc(anchura*sizeof(double));
    double max_vabs=0;
    double vabs=0;
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

            if (valor_absoluto(destino[j]) > max_vabs){
                max_vabs = valor_absoluto(destino[j]);
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