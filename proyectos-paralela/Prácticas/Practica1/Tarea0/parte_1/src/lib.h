#include <stdio.h>

#ifndef __LIB_H__
#define __LIB_H__

void filtrado_media(unsigned char** dest, unsigned char** orig, const int filas, const int columnas, const int radio);
void filtrado_mediana(unsigned char** dest, unsigned char** orig, const int filas, const int columnas, const int radio);
void filtrado_sobel(unsigned char** dest, unsigned char** orig, const int filas, const int columnas, const int radio);
unsigned char pixel_mediana(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
unsigned char pixel_media(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
unsigned char pixel_sobel(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
void quicksort(unsigned char* array, int first, int last);
void swap(unsigned char* a, unsigned char* b);

#endif