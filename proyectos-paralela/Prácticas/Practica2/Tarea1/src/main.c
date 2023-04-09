#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <omp.h>

#define MODO_ME 0
#define MODO_MN 1
#define MODO_SO 2
#define SQ_SIZE 9

#define TAG_FSIZE 0
#define TAG_MATRIZ 1
#define TAG_RESULTADO 2

#define N 6 // Numero de hilos

void usage(char *program_name);
int validar_modo(char *modo);
void filtrado_media(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post);
void filtrado_mediana(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post);
void filtrado_sobel(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int desp_prev, const int desp_post);
unsigned char pixel_mediana(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
unsigned char pixel_media(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
unsigned char pixel_sobel(unsigned char** matrix, const int matrix_width, const int matrix_height, const int pixel_row, const int pixel_col, const int square_size);
void swap(unsigned char* a, unsigned char* b);
void quicksort(unsigned char* array, int first, int last);


int main(int argc, char* argv[]){
    /*************************************/
    //      CONTROL DE LOS ARGUMENTOS
    /*************************************/
    if (argc != 4){
		printf("Error: Numero de argumentos incorrecto.\n");
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

    int tam = atoi(argv[3]);
    if (tam <= 0){
        printf("Error: Dimensiones de la matriz incorrectas.\n");
        usage(argv[0]);
        return -1;
    }
    
    double fsize = 0;
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

	int number_pixels = fsize/sizeof(unsigned char);
    if (tam*tam > number_pixels){
        printf("Error: Dimensiones de la matriz demasiado grandes.\n");
        usage(argv[0]);
        return -1;
    }

    /*************************************/
    //  FIN DEL CONTROL DE LOS ARGUMENTOS
    /*************************************/
    // Todo correcto, iniciamos el programa.
    // Variables del programa
    int i;

    unsigned char** matrix = (unsigned char**)malloc(tam*sizeof(unsigned char*));
    unsigned char** filtred_matrix = (unsigned char**)malloc(tam*sizeof(unsigned char*));

    matrix[0] = (unsigned char*)malloc(tam*tam*sizeof(unsigned char));
    filtred_matrix[0] = (unsigned char*)malloc(tam*tam*sizeof(unsigned char));

    for (i=1; i < tam; i++)
        matrix[i] = matrix[i-1] + tam;
    for (i=1; i < tam; i++)
        filtred_matrix[i] = filtred_matrix[i-1] + tam;

    struct timespec start, end;
    double cpu_time_used = 0.0;

    // COMIENZA EL CRONOMETRO
    clock_gettime(CLOCK_REALTIME, &start);

    fread(matrix[0], sizeof(unsigned char), tam*tam, f); // Se lee toda la matriz

    #pragma omp parallel num_threads(N) shared(matrix,filtred_matrix, tam, modo)
    {
        int tam_proc = tam/N;
        int resto = tam%N;
        int iam = omp_get_thread_num();
        int posicion_inicio[N];
        int posicion_fin[N];

        posicion_inicio[0] = 0;
        posicion_fin[0] = tam_proc;
        for (int i=1; i < N; i++){
            posicion_inicio[i] = posicion_fin[i-1];
            posicion_fin[i] = posicion_inicio[i] + tam_proc;
            if (i==N-1)
                posicion_fin[i] += resto;
        }

        switch(modo){
        case MODO_ME:
            filtrado_media(filtred_matrix, tam_proc, tam, matrix, tam, tam, SQ_SIZE, posicion_inicio[iam], posicion_fin[iam]);
            break;
        case MODO_MN:
            filtrado_mediana(filtred_matrix, tam_proc, tam, matrix, tam, tam, SQ_SIZE, posicion_inicio[iam], posicion_fin[iam]);
            break;
        case MODO_SO:
            filtrado_sobel(filtred_matrix, tam_proc, tam, matrix, tam, tam, SQ_SIZE, posicion_inicio[iam], posicion_fin[iam]);
            break;
        }
    }  

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Tiempo de ejecución: %f ms.\n", cpu_time_used);

    // Escritura del archivo
    char* new_file_name = "imagen_filtrada.raw";
    FILE* new_f = fopen(new_file_name, "wb");
    fwrite(filtred_matrix[0], sizeof(unsigned char), tam*tam, new_f);
    fclose(new_f);

    char* data_file_name = "out.txt";
    FILE* data_f = fopen(data_file_name, "a");
    fprintf(data_f, "EJECUCIÓN DEL PROGRAMA '%s' (FECHA %s | HORA %s)\n", argv[0], __DATE__, __TIME__);
    fprintf(data_f, "Con el archivo de entrada '%s'\n", file_name);
    fprintf(data_f, "Fichero de salida resultado: '%s'\n", new_file_name);
    fprintf(data_f, "Modo de ejecución: '%s'\n", argv[2]);
    fprintf(data_f, "Dimension de la matriz: %s x %s\n", argv[3], argv[3]);
    fprintf(data_f, "Tiempo de ejecución: %f ms\n", cpu_time_used);
    fprintf(data_f, "Número de hilos: %d\n", N);
    fprintf(data_f, "\n--------------------------------------------------------------------\n");

    printf("Archivo filtrado guardado en %s.\n", new_file_name);
    printf("Datos de ejecución guardados en %s.\n", data_file_name);
    
    fclose(data_f);
    fclose(f);

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


/**
 * @brief Se realiza un filtrado de la imagen por media
 * 
 * @param dest Matriz de destino de la imagen
 * @param orig Matriz de origen de la imagen
 * @param filas Tamaño de filas de la imagen
 * @param columnas Tamaño de columnas de la imagen
 * @param radio Número de pixeles a considerar en el filtrado
 */
void filtrado_media(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int inicio, const int fin){
    int i,j;
    for ( i = inicio; i < fin; i++) 
        for ( j = 0; j < columnas_d; j++) 
            dest[i][j] = pixel_media(orig, columnas_o, filas_o, i, j, radio);
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
void filtrado_mediana(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int inicio, const int fin ){
    int i,j;
    // Aplicamos el filtro
    for ( i = inicio; i < fin; i++) 
        for ( j = 0; j < columnas_d; j++) 
            dest[i][j] = pixel_mediana(orig, columnas_o, filas_o, i, j, radio);
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
void filtrado_sobel(unsigned char** dest, const int filas_d, const int columnas_d, unsigned char** orig, const int filas_o, const int columnas_o, const int radio, const int inicio, const int fin ){
    int i,j;
    for ( i = inicio; i < fin; i++) 
        for ( j = 0; j < columnas_d; j++)
            dest[i][j] = pixel_sobel(orig, columnas_o, filas_o, i, j, radio);
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