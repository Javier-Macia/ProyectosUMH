#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NFyC 5 // Numero de filas y columnas que se quitan

/**
 * @author Javier Maciá Sempere
 * 
 * @brief Programa que lee una matriz de un fichero y reduce su tamaño para
 *       obtener una nueva matriz con el tamaño deseado.
 * 
 * @param argc Número de argumentos
 * @param argv Nombre del fichero a abrir (fichero1.bin o fichero2.bin)
 * @return int 0 si todo ha ido bien
 */
int main(int argc, char *argv[]){  
    if (argc < 2){
        printf("ERROR: Número de argumentos incorrecto.\n");
        printf("El segundo argumento debe ser un fichero a abrir.\n");
        return 1;
    }

    char *filename = argv[1];
    char *filenameResultado;
    char copy[strlen(filename)+1];
    char aux[strlen("resultado_")+strlen(copy)];

    strcpy(copy, filename);
    strcpy(aux, "resultado_");
    strcat(aux, copy);
    filenameResultado = aux;


    FILE *f = fopen(filename, "rb");
    FILE *fResultado = fopen(filenameResultado, "wb");

    if (f == NULL || fResultado == NULL){
        printf("ERROR: No se ha podido abrir el archivo.\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    const int tamArchivo = ftell(f);
    fseek(f, 0, SEEK_SET);
    const int tamElemento = sizeof(int);
    const int numDoubles = tamArchivo / tamElemento;

    int** d;
    int** dAux;
    int** resultado;
    int nFilas, nColumnas;
    int nFilasResultado, nColumnasResultado;
    int i,j;

    nFilas = sqrt(numDoubles);
    nColumnas = nFilas;
    nFilasResultado = nFilas - NFyC*2;
    nColumnasResultado = nFilasResultado;
    

    // Leemos el archivo y lo guardamos en una matriz
    d = (int**)malloc(nFilas*sizeof(int*));
    for ( i = 0; i < nFilas; i++){
        d[i] = (int*)malloc(nColumnas*tamElemento);
        fread(d[i], tamElemento, nColumnas, f);
    }


    // Usando memcpy quitamos las 5 primeras filas y columnas, y las 5 ultimas filas y columnas
    // Metemos esa información en un nuevo fichero.
    resultado = (int**)malloc(((nFilasResultado))*sizeof(int*));
    for ( i = 0; i < nFilasResultado; i++){
        resultado[i] = (int*)malloc(nColumnasResultado*tamElemento);
        d[i+NFyC] += NFyC;
        memcpy(resultado[i], d[i+NFyC], (nColumnasResultado*tamElemento));
        d[i+NFyC] -= NFyC;
        fwrite(resultado[i], tamElemento, nColumnasResultado, fResultado);
    }
    printf("El fichero '%s' se ha creado correctamente.\n", filenameResultado);

    printf("El tamaño del archivo es %d bytes.\n", tamArchivo);
    printf("El tamaño de un double es %d bytes.\n", tamElemento);
    printf("Por tanto, el número de doubles es %d.\n", numDoubles);
    printf("El número de filas es %d.\n", nFilas);
    printf("El número de columnas es %d.\n", nColumnas);


    fclose(f);
    fclose(fResultado);

    printf("FIN...\n");
    return 0;
}