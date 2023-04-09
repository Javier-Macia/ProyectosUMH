#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TAM 512 // Tamaño de la matriz cuadrada

/**
 * @author Javier Maciá Sempere
 * 
 * @brief Programa que lee una matriz cuadrada 512x512 de unsigned char.
 * Calcula y almacena en un vector la media de cada una de las 512 filas y
 * en otro vector la media de cada una de las 512 columnas.
 * Escribir en un fichero texto el máximo y mínimo de la media de las 512
 * filas y lo mismo para la media de las columnas.
 * 
 * @param argc Número de argumentos
 * @param argv Nombre del fichero a abrir (fichero1.bin o fichero2.bin)
 * @return int 0 si todo ha ido bien
 */
int main(int argc, char *argv[]){
    char* filename = "fichero2.bin";
    char* outname = "resultado.txt";
    FILE *f = fopen(filename, "rb");
    FILE *out = fopen(outname, "w");
    if (out == NULL || f == NULL){
        printf("Error al abrir un fichero");
        return -1;
    }
    unsigned char **matriz;
    unsigned char *vFilas, *vColumnas;
    int maximoF, minimoF, mediaF; 
    int maximoC, minimoC, mediaC; 
    int i,j;

    vFilas = (unsigned char*)malloc(TAM * sizeof(unsigned char));
    vColumnas = (unsigned char*)malloc(TAM * sizeof(unsigned char));
    matriz = (unsigned char**)malloc(TAM * sizeof(unsigned char*));

    mediaF = 0;
    mediaC = 0;
    maximoC = maximoF = 0;
    minimoC = minimoF = UCHAR_MAX;

    for ( i = 0; i < TAM; i++){
        matriz[i] = (unsigned char*)malloc(TAM * sizeof(unsigned char));
        fread(matriz[i], sizeof(unsigned char), TAM, f );
    }
    fclose(f);

    for (i = 0; i < TAM; i++){
        mediaF = 0;
        mediaC = 0;
        for ( j = 0; j < TAM; j++){
            mediaF += matriz[i][j];
            mediaC += matriz[j][i];
        }
        mediaF = mediaF/TAM;
        mediaC = mediaC/TAM;
        
        vFilas[i] = mediaF;
        vColumnas[i] = mediaC;

        if (vFilas[i] < minimoF)
            minimoF = vFilas[i];
        else if (vFilas[i] > maximoF)
            maximoF = vFilas[i];
        
        if (vColumnas[i] < minimoC) 
            minimoC = vColumnas[i];
        else if (vColumnas[i] > maximoC)
            maximoC = vColumnas[i];
    }

    printf("Maximo de la media de todas las filas-> %d\n", maximoF);
    printf("Minimo de la media de todas las filas-> %d \n", minimoF);
    printf("Maximo de la media de todas las columnas-> %d \n", maximoC);
    printf("Mínimo de la media de todas las columnas-> %d \n", minimoC);

    fprintf(out, "Maximo de la media de todas las filas-> %d\n", maximoF);
    fprintf(out, "Minimo de la media de todas las filas-> %d \n", minimoF);
    fprintf(out, "Maximo de la media de todas las columnas-> %d \n", maximoC);
    fprintf(out, "Mínimo de la media de todas las columnas-> %d \n", minimoC);
    fclose(out);

    free(vFilas);
    free(vColumnas);
    for (i = 0; i < TAM; i++){
        free(matriz[i]);
    }
    free(matriz);
    
    printf("FIN...\n");
    return 0;
}