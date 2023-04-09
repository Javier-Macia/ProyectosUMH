#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "lib.h"

#define MODO_ME 0
#define MODO_MN 1
#define MODO_SO 2
#define SQ_SIZE 9

void usage(char *program_name);
int validar_modo(char *modo);

int main(int argc, char* argv[]){
	if (argc != 4){
		printf("Error: Numero de argumentos incorrecto.\n", argc);
		usage(argv[0]);
		return -1;
	}
	char* file_name=argv[1];
	FILE* f = fopen(file_name, "rw");
	if (f == NULL){
		printf("Error: No se pudo abrir el archivo %s.\n", file_name);
		return -1;
	}

    int modo = validar_modo(argv[2]);
    if (modo < 0){
        printf("Error: Modo de ejecucion incorrecto.\n");
        usage(argv[0]);
        return -1;
    }

	fseek(f, 0, SEEK_END);
	double fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	int number_pixels = fsize/sizeof(unsigned char);

    int tam = atoi(argv[3]);
    if (tam <= 0){
        printf("Error: Dimensiones de la matriz incorrectas.\n");
        usage(argv[0]);
        return -1;
    }
    if (tam*tam > number_pixels){
        printf("Error: Dimensiones de la matriz demasiado grandes.\n");
        usage(argv[0]);
        return -1;
    }

    
	unsigned char** matrix = (unsigned char**)malloc(tam*sizeof(unsigned char*));
    unsigned char** filtred_matrix = (unsigned char**)malloc(tam*sizeof(unsigned char*));
    clock_t start, end;
    double cpu_time_used;   

    int i,j;

	for (i=0; i<tam; i++){
		matrix[i] = (unsigned char*)malloc(tam*sizeof(unsigned char));
        filtred_matrix[i] = (unsigned char*)malloc(tam*sizeof(unsigned char));
		fread(matrix[i], sizeof(unsigned char), tam, f);
	}
    
    start = clock();
    // Aplicamos el filtro
    /**
     * Aquí soy consciente de que repito líneas de código. 
     * Lo hago con la intención de que sea más fácil de leer y 
     * de que, al hacer los bucles, me ahorre tener que comprobar
     * el modo en el que estoy cada vez.
     */
    switch(modo){
    case MODO_ME:
        filtrado_media(filtred_matrix, matrix, tam, tam, SQ_SIZE);
        break;
    case MODO_MN:
        filtrado_mediana(filtred_matrix, matrix, tam, tam, SQ_SIZE);
        break;
    case MODO_SO:
        filtrado_sobel(filtred_matrix, matrix, tam, tam, SQ_SIZE);
        break;
    default:
        printf("Error: Modo de ejecucion incorrecto.\n");
        usage(argv[0]);
        return -1;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    // Escritura del archivo
    char* new_file_name = "imagen_filtrada.raw";
    FILE* new_f = fopen(new_file_name, "wb");
    for (i=0; i<tam; i++){
        fwrite(filtred_matrix[i], sizeof(unsigned char), tam, new_f);
    }

    char* data_file_name = "out.txt";
    FILE* data_f = fopen(data_file_name, "w");
    fprintf(data_f, "EJECUCIÓN DEL PROGRAMA '%s' (FECHA %s | HORA %s)\n", argv[0], __DATE__, __TIME__);
    fprintf(data_f, "Con el archivo de entrada '%s'\n", file_name);
    fprintf(data_f, "Fichero de salida resultado: '%s'\n", new_file_name);
    fprintf(data_f, "Modo de ejecución: '%s'\n", argv[2]);
    fprintf(data_f, "Dimension de la matriz: %s x %s\n", argv[3], argv[3]);
    fprintf(data_f, "Tiempo de ejecución: %f segundos\n", cpu_time_used);

    printf("Archivo filtrado guardado en %s.\n", new_file_name);
    printf("Datos de ejecución guardados en %s.\n", data_file_name);

	fclose(f);
    fclose(new_f);
	free(matrix);
    free(filtred_matrix);
	return 0;
}

void usage(char* program_name){
	printf("Uso: %s <nombre_archivo.raw> <modo> <tam>\n", program_name);
	printf("Donde:\n");
    printf("<nombre_archivo> es el nombre del archivo a procesar.\n");
	printf("<modo> es media(me)(0), mediana(mn)(1) o sobel(so)(2).\n");
	printf("<tam> es el tamaño de la imagen (tam x tam).\n");
}

int validar_modo(char* modo){
    if (strcmp(modo, "me") == 0 || strcmp(modo, "0") == 0){
        return MODO_ME;
    }
    else if (strcmp(modo, "mn") == 0 || strcmp(modo, "1") == 0){
        return MODO_MN;
    }
    else if (strcmp(modo, "so") == 0 || strcmp(modo, "2") == 0){
        return MODO_SO;
    }
    return -1;
}