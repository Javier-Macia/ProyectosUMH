#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define N 1200 // Tamaño a leer

double valor_absoluto(double x){
    if(x<0)
        return -x;
    return x;
}

int main(int argc, char* argv[]){
    char* nombre_archivo = "mat.bin";
    FILE* archivo = fopen(nombre_archivo, "rb");
    int i,j;
    if(archivo == NULL)
        printf("Error abriendo el archivo %s\n",nombre_archivo);
    
    double** matriz = (double**)malloc(sizeof(double*)*N);
    for( i = 0; i < N; i++){
        matriz[i] = (double*)malloc(sizeof(double)*N);
        fread(matriz[i], sizeof(double), N, archivo);
    }
    double* identidad = (double*)malloc(sizeof(double)*N);
    double* vector_resultado = (double*)malloc(sizeof(double)*N);
    double resultado = 0;
    for( i = 0; i < N; i++){
        identidad[i] = 1;
        vector_resultado[i] = 0;
    }

    // Producto matriz y vector
    for( i = 0; i < N; i++){
        for( j = 0; j < N; j++){
            vector_resultado[i] += matriz[i][j]*identidad[j];
        }
        if (valor_absoluto(vector_resultado[i]) > valor_absoluto(resultado))
            resultado = vector_resultado[i];
    }


    printf("Valor absoluto max: %f \n", resultado);

    // Liberamos memoria
    fclose(archivo);
    free(matriz);
    free(identidad);
    free(vector_resultado);
    return 0;
}