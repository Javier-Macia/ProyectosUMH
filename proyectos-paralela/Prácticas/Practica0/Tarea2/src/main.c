#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMDATOS 10

void imprimirInstrucciones(char *argv[]);

int main(int argc, char *argv[]){  
    if (argc==1){
        imprimirInstrucciones(argv);
        printf("No se ha indicado ningún parámetro y por tanto no se ejecutará el programa.\n");
        return 0;
    } 

    int nFilas, nColumnas;

    int i,j;

    const char* filenameDatos="datos10dobles.bin";
    const char* filenameEscritura;

    unsigned long long int direccion;

    if ( strcmp(argv[1],"2x5") == 0 ){
        nFilas = 2;
        nColumnas = 5;
        filenameEscritura="2x5.txt";
    }
    else if( strcmp(argv[1],"5x2") == 0 ){
        nFilas = 5;
        nColumnas = 2;
        filenameEscritura="5x2.txt";
    }
    else{
        imprimirInstrucciones(argv);
        printf("ERROR: El parámetro indicado no es válido y por tanto no se ejecutará el programa.\n");
        return 0;
    }

    double** matrizEE;
    double** matrizFF;

    FILE* fDatos = fopen(filenameDatos,"rb");
    FILE* fEscritura = fopen(filenameEscritura,"w");

    matrizEE = malloc(nFilas*sizeof(double*));
    matrizFF = malloc(nFilas*sizeof(double*));
    for ( i = 0; i < nFilas; i++){
        for ( j = 0; j < nColumnas; j++){
            matrizEE[i] = malloc(nColumnas*sizeof(double));
            matrizFF[i] = malloc(nColumnas*sizeof(double));
        }
    }

    if (fDatos==NULL || fEscritura==NULL){
        printf("Error al abrir uno de los archivos.\n");
        return -1;    
    }
    
    //Leemos el fichero fila por fila
    for ( i = 0; i < nFilas; i++){
        fread(matrizFF[i],sizeof(double),nColumnas,fDatos);
    }
    fseek(fDatos, 0, 0); // Volvemos al inicio del archivo.

    // Leemos el fichero elemento a elemento
    for (i = 0; i < nFilas; i++){
        for ( j = 0; j < nColumnas; j++){
            fread(&matrizEE[i][j], sizeof(matrizEE[i][j]), 1, fDatos);
        }
    }
    
    // Imprimimos la matriz por pantalla
    for ( i = 0; i < nFilas; i++){
        for ( j = 0; j < nColumnas; j++){
            // Mostramos por pantalla
            printf("[%d][%d] = %f\n", i, j, matrizEE[i][j]);
        }
    }

    // Introducimos la información en el fichero
    fprintf(fEscritura, "\tMATRIZ EE %dx%d\n", nFilas, nColumnas);
    fprintf(fEscritura, "------------------------------------------\n");

    for ( i = 0; i < nFilas; i++){
        direccion = &matrizEE[i][0];
        fprintf(fEscritura, "Dirección matrizEE[%d][%d]: %llu\n", i, 0, direccion);
        direccion = &matrizEE[i][nColumnas-1];
        fprintf(fEscritura, "Dirección matrizEE[%d][%d]: %llu\n", i, nColumnas-1, direccion);
    }

    fprintf(fEscritura, "\n");

    fprintf(fEscritura, "\tMATRIZ FF %dx%d\n", nFilas, nColumnas);
    fprintf(fEscritura, "------------------------------------------\n");
    for ( i = 0; i < nFilas; i++){
        direccion = &matrizFF[i][0];
        fprintf(fEscritura, "Dirección matrizFF[%d][%d]: %llu\n", i, 0, direccion);
        direccion = &matrizFF[i][nColumnas-1];
        fprintf(fEscritura, "Dirección matrizFF[%d][%d]: %llu\n", i, nColumnas-1, direccion);
    }

    fclose(fDatos);
    fclose(fEscritura);

    printf("Fin de la ejecución del programa '%s'.\n", argv[0]);
    return 0;
}


void imprimirInstrucciones(char *argv[]){
    printf("Guía de uso.\n");
    printf(" > '%s' imprime esta guía. \n", argv[0]);
    printf(" > '%s 2x5' Almacena los datos en una matriz 2x5. \n", argv[0]);
    printf(" > '%s 5x2' Almacena los datos en una matriz 5x2. \n", argv[0]);
}
