#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib.h"

/**
 * @brief Se realiza un filtrado de la imagen por media
 * 
 * @param dest Matriz de destino de la imagen
 * @param orig Matriz de origen de la imagen
 * @param filas Tamaño de filas de la imagen
 * @param columnas Tamaño de columnas de la imagen
 * @param radio Número de pixeles a considerar en el filtrado
 */
void filtrado_media(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post){
    int i,j;
    for ( i = 0; i < filas_d; i++) 
        for ( j = 0; j < columnas_d; j++) 
            dest[i][j] = pixel_media(orig, filas_o+desp_prev+desp_post, columnas_o, i+desp_prev, j, radio);
}

/**
 * @brief Se realiza un filtrado de la imagen por mediana
 * 
 * @param dest Matriz de destino de la imagen
 * @param orig Matriz de origen de la imagen
 * @param filas Tamaño de filas de la imagen
 * @param columnas Tamaño de columnas de la imagen
 * @param radio Número de pixeles a considerar en el filtrado
 */
void filtrado_mediana(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post){
    int i,j;
    // Aplicamos el filtro
    for ( i = 0; i < filas_d; i++) 
        for ( j = 0; j < columnas_d; j++) 
            dest[i][j] = pixel_mediana(orig, filas_o+desp_prev+desp_post, columnas_o, i+desp_prev, j, radio);
}

/**
 * @brief Se realiza un filtrado de la imagen por SOBEL
 * 
 * @param dest Matriz de destino de la imagen
 * @param orig Matriz de origen de la imagen
 * @param filas Tamaño de filas de la imagen
 * @param columnas Tamaño de columnas de la imagen
 * @param radio Número de pixeles a considerar en el filtrado
 */
void filtrado_sobel(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post){
    int i,j;
    for ( i = 0; i < filas_d; i++) 
        for ( j = 0; j < columnas_d; j++)
            dest[i][j] = pixel_sobel(orig, columnas_o, filas_o+desp_post+desp_prev, i+desp_prev, j, radio);
}

/**
 * @brief Dada una matriz de pixeles,
 * se calcula el valor del pixel en la posición (i,j) 
 * aplicando el filtro de media
 * 
 * @param matrix Matriz de pixeles
 * @param matrix_width Anchura de la matriz
 * @param matrix_height Altura de la matriz
 * @param pixel_row Fila del pixel a calcular
 * @param pixel_col Columna del pixel a calcular
 * @param square_size Numero de pixeles a considerar en el cálculo de la media
 * @return unsigned char 
 */
unsigned char pixel_media(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size){
    long long avg_value = 0;
    const int square_length = sqrt(square_size);
    const double square_half = square_length/2;
    int i, j;
    
    if ((pixel_row - square_half) < 0 || 
        (pixel_col - square_half) < 0 || 
        (pixel_row + square_half) >= matrix_width || 
        (pixel_col + square_half) >= matrix_height) {
        return matrix[pixel_row][pixel_col];
    }

    for(i = pixel_row-square_half; i <= pixel_row+square_half; i++){
        for(j = pixel_col-square_half; j <= pixel_col+square_half; j++)
            avg_value += matrix[i][j];
    }
    return (avg_value/square_size);
}

/**
 * @brief Dada una matriz de pixeles,
 * se calcula el valor del pixel en la posición (i,j) 
 * aplicando el filtro de mediana
 * 
 * @param matrix Matriz de pixeles
 * @param matrix_width Anchura de la matriz
 * @param matrix_height Altura de la matriz
 * @param pixel_row Fila del pixel a calcular
 * @param pixel_col Columna del pixel a calcular
 * @param square_size Numero de pixeles a considerar en el cálculo de la mediana
 * @return unsigned char 
 */
unsigned char pixel_mediana(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size){
    const int square_length = sqrt(square_size);
    const int square_half = square_length/2;
    int i, j;
    if ((pixel_row - square_half) < 0 || 
        (pixel_col - square_half) < 0 || 
        (pixel_row + square_half) >= matrix_width || 
        (pixel_col + square_half) >= matrix_height)
        {
        return matrix[pixel_row][pixel_col];
    }

    unsigned char* square = (unsigned char*) malloc(square_size*sizeof(unsigned char));
    int k = 0;
    for(i = pixel_row-square_half; i <= pixel_row+square_half; i++){
        for(j = pixel_col-square_half; j <= pixel_col+square_half; j++){
            square[k] = matrix[i][j];
            k++;
        }
    }
    quicksort(square, 0, square_size-1);
    unsigned char median = square[square_size/2];
    free(square);
    return median;
}

/**
 * @brief Dada una matriz de pixeles,
 * se calcula el valor del pixel en la posición (i,j) 
 * aplicando el filtro de SOBEL
 * 
 * @param matrix Matriz de pixeles
 * @param matrix_width Anchura de la matriz
 * @param matrix_height Altura de la matriz
 * @param pixel_row Fila del pixel a calcular
 * @param pixel_col Columna del pixel a calcular
 * @param square_size Numero de pixeles a considerar en el cálculo del SOBEL
 * @return unsigned char 
 */
unsigned char pixel_sobel(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size){
    int C[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int F[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    const int tamAux = sqrt(square_size);
    unsigned char pixel_filtrado;
    int i,j, indice=0;

    unsigned long sumF = 0, sumC = 0;
    int nueva_fila = 0, nueva_columna = 0;

    unsigned char *square = (unsigned char*) malloc(square_size*sizeof(unsigned char));

    
    if (pixel_row == 0 || pixel_row == matrix_height-1 || pixel_col == 0 || pixel_col == matrix_width-1){ // Es un borde o una esquina
        for (i = pixel_row-1; i <= pixel_row+1; i++){ // Recorremos los 3x3 pixeles alrededor del pixel
            if (i < 0) // Corrección de índices por arriba
                nueva_fila = i +2;
            else if (i > matrix_height - 1) // Corrección de índices por abajo
                nueva_fila = i -2;
            else
                nueva_fila = i;
            for (j = pixel_col-1; j <= pixel_col+1; j++){ 
                if (j < 0) // Corrección de índices por la izquierda
                    nueva_columna = j +2;
                else if (j > matrix_width -1) // Corrección de índices por la derecha
                    nueva_columna = j -2;
                else
                    nueva_columna = j; 

                // Imprimimos square
                square[indice] = matrix[nueva_fila][nueva_columna];  
                sumC += square[indice] * C[indice];
                sumF += square[indice] * F[indice];
                indice++;
            }

        }
    }
    else { // Es un pixel interno
        for(i=pixel_row-1; i<=pixel_row+1; i++){
            for(j=pixel_col-1; j<=pixel_col+1; j++){
                square[indice] = matrix[i][j];
                sumC += square[indice] * C[indice];
                sumF += square[indice] * F[indice];
                indice++;
            }
        }
    }

    // Aplicamos SOBEL al pixel
    pixel_filtrado = sqrt(sumF*sumF + sumC*sumC);
    return pixel_filtrado;
}

/**
 * @brief Función de ordenación rápida
 * 
 * @param array 
 * @param first 
 * @param last 
 */
void quicksort(unsigned char* array, int first, int last){
    int i, j, pivot, aux;
    if(first < last){
        pivot = first;
        i = first;
        j = last;
        while(i < j){
            while(array[i] <= array[pivot] && i < last)
                i++;
            while(array[j] > array[pivot])
                j--;
            if(i < j){
                aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
        swap(&array[pivot], &array[j]);
        quicksort(array, first, j-1);
        quicksort(array, j+1, last);
    }
}

/**
 * @brief Función de intercambio de valores
 * 
 * @param a 
 * @param b 
 */
void swap(unsigned char* a, unsigned char* b){
	unsigned char aux = *a;
	*a = *b;
	*b = aux;
}