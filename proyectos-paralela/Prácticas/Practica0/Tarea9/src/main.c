#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define SQ_SIZE 9

void usage(char *program_name);
void remove_extension(char *origin, char *dest);
unsigned char pixel_mediana_qs(const int pixel_row, const int pixel_col, unsigned char** matrix, const int matrix_length);
unsigned char pixel_mediana_bs(const int pixel_row, const int pixel_col, unsigned char** matrix, const int matrix_length);
void quicksort(unsigned char* array, int first, int last);
void bubblesort(unsigned char* array, int first, int last);
void swap(unsigned char* a, unsigned char* b);


int main(int argc, char* argv[]){
    // Control de argumentos
    if(argc != 2){
        printf("ERROR: Debe introducir un argumento.\n");
        usage(argv[0]);
        return -1;
    }

    char* file_name = argv[1];
    FILE* file = fopen(file_name, "rb");
    if(file == NULL){
        printf("ERROR: No se ha podido abrir el fichero %s\n",file_name);
        return -1;   
    }
    // Obtenemos el tamaño del fichero
    fseek(file, 0, SEEK_END);
    long file_tam = ftell(file);
    long file_elements = file_tam/sizeof(unsigned char);
    long file_length = sqrt(file_elements);
    fseek(file, 0, SEEK_SET);

    // Ficheros nuevos
    char* new_file_name_qs = malloc(sizeof(char)*(strlen(file_name)+14));
    remove_extension(file_name, new_file_name_qs);
    strcat(new_file_name_qs, "_media_qs.raw");
    FILE* new_file_qs = fopen(new_file_name_qs, "wb");
    if(new_file_qs == NULL){
        printf("ERROR: No se ha podido crear el fichero %s\n",new_file_name_qs);
        return -1;   
    }

    char* new_file_name_bs = malloc(sizeof(char)*(strlen(file_name)+14));
    remove_extension(file_name, new_file_name_bs);
    strcat(new_file_name_bs, "_media_bs.raw");
    FILE* new_file_bs = fopen(new_file_name_bs, "wb");
    if(new_file_bs == NULL){
        printf("ERROR: No se ha podido crear el fichero %s\n",new_file_name_bs);
        return -1;   
    }

    // Cronometro
    clock_t start, end;
    double cpu_time_used;    

    // Reservamos memoria para la matriz
    unsigned char** matrix = malloc(sizeof(unsigned char*)*file_length);
    unsigned char** filtred_matrix = malloc(sizeof(unsigned char*)*file_length);

    // Reserva de memoria
    for (int i = 0; i < file_length; i++){
        matrix[i] = malloc(sizeof(unsigned char)*file_length);
        filtred_matrix[i] = malloc(sizeof(unsigned char)*file_length);
        fread(matrix[i], sizeof(unsigned char), file_length, file);
    }
    
    printf("Filtrando imagen con QuickSort...\n");
    // Filtro de mediana con quicksort
    start = clock();
    for(int i = 0; i < file_length; i++){
        // Aplicamos el filtro
        for(int j = 0; j < file_length; j++){
            filtred_matrix[i][j] = pixel_mediana_qs(i, j, matrix, file_length);
        }
        fwrite(filtred_matrix[i], sizeof(unsigned char), file_length, new_file_qs);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
    printf("FILTRO APLICADO EN ARCHIVO %s Y GUARDADO EN %s...\n",file_name, new_file_name_qs);  

    printf("\n");

    printf("Filtrando imagen con BubbleSort...\n");
    // Filtro de mediana con bubble sort
    start = clock();
    for(int i = 0; i < file_length; i++){
        // Aplicamos el filtro
        for(int j = 0; j < file_length; j++){
            filtred_matrix[i][j] = pixel_mediana_bs(i, j, matrix, file_length);
        }
        fwrite(filtred_matrix[i], sizeof(unsigned char), file_length, new_file_bs);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
    printf("FILTRO APLICADO EN ARCHIVO %s Y GUARDADO EN %s...\n",file_name, new_file_name_bs);  

    printf("\n-------------\n");

    // Liberamos memoria
    fclose(file);
    fclose(new_file_bs);
    free(new_file_name_bs);
    fclose(new_file_qs);
    free(new_file_name_qs);
    free(matrix);
    free(filtred_matrix);
    return 0;
}

void usage(char* program_name){
    printf("Usage:\n ");
    printf("->\t%s <filename>\n",program_name);
    printf("\t Donde <filename> es el nombre del archivo a abrir.\n");
}

void remove_extension(char* origin, char* dest){
    int i = 0;
    while(origin[i] != '.' && origin[i] != '\0'){
        dest[i] = origin[i];
        i++;
    }
    dest[i] = '\0';
}

unsigned char pixel_mediana_qs(const int pixel_row, const int pixel_col, unsigned char** matrix, const int matrix_length){
    const int square_size = SQ_SIZE;
    const int square_length = sqrt(square_size);
    const double square_half = square_length/2;
    int i, j;
    
    
    if ((pixel_row - square_half) < 0 || 
        (pixel_col - square_half) < 0 || 
        (pixel_row + square_half) >= matrix_length || 
        (pixel_col + square_half) >= matrix_length)
        {
        return matrix[pixel_row][pixel_col];
    }

    unsigned char* square = malloc(sizeof(unsigned char)*square_size);
    int square_index = 0;
    for(i = pixel_row - square_half; i <= pixel_row + square_half; i++){
        for(j = pixel_col - square_half; j <= pixel_col + square_half; j++){
            square[square_index] = matrix[i][j];
            square_index++;
        }
    }
    quicksort(square, 0, square_size-1);
    unsigned char median = square[square_size/2];
    free(square);
    return median;
}

unsigned char pixel_mediana_bs(const int pixel_row, const int pixel_col, unsigned char** matrix, const int matrix_length){
    const int square_size = SQ_SIZE;
    const int square_length = sqrt(square_size);
    const double square_half = square_length/2;
    int i, j;
    
    
    if ((pixel_row - square_half) < 0 || 
        (pixel_col - square_half) < 0 || 
        (pixel_row + square_half) >= matrix_length || 
        (pixel_col + square_half) >= matrix_length)
        {
        return matrix[pixel_row][pixel_col];
    }

    unsigned char* square = malloc(sizeof(unsigned char)*square_size);
    int square_index = 0;
    for(i = pixel_row - square_half; i <= pixel_row + square_half; i++){
        for(j = pixel_col - square_half; j <= pixel_col + square_half; j++){
            square[square_index] = matrix[i][j];
            square_index++;
        }
    }
    bubblesort(square, 0, square_size-1);
    unsigned char median = square[square_size/2];
    free(square);
    return median;
}

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

void bubblesort(unsigned char* array, int first, int last){
    int i, j, aux;
    for(i = first; i < last; i++){
        for(j = first; j < last; j++){
            if(array[j] > array[j+1]){
                aux = array[j];
                array[j] = array[j+1];
                array[j+1] = aux;
            }
        }
    }
    
}

void swap(unsigned char* a, unsigned char* b){
    unsigned char aux = *a;
    *a = *b;
    *b = aux;
}