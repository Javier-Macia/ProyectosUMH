#include <stdio.h>

#ifndef __LIB_H__
#define __LIB_H__

void filtrado_media(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post);
void filtrado_mediana(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post);
void filtrado_sobel(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post);
unsigned char pixel_mediana(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
unsigned char pixel_media(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
unsigned char pixel_sobel(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
void swap(unsigned char* a, unsigned char* b);
void quicksort(unsigned char* array, int first, int last);

#endif