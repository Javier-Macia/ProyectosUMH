#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void usage(char *program_name);
void remove_extension(char *origin, char *dest);
unsigned char pixel_media(const int pixel_row, const int pixel_col, unsigned char** matrix, const int matrix_length);

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

    // Fichero nuevo
    char* new_file_name = malloc(sizeof(char)*strlen(file_name));
    remove_extension(file_name, new_file_name);
    strcat(new_file_name, "_media.raw");
    FILE* new_file = fopen(new_file_name, "wb");

    // Reservamos memoria para la matriz
    unsigned char** matrix = malloc(sizeof(unsigned char*)*file_length);
    unsigned char** filtred_matrix = malloc(sizeof(unsigned char*)*file_length);

    // Reserva de memoria
    for (int i = 0; i < file_length; i++){
        matrix[i] = malloc(sizeof(unsigned char)*file_length);
        filtred_matrix[i] = malloc(sizeof(unsigned char)*file_length);
        fread(matrix[i], sizeof(unsigned char), file_length, file);
    }
    
    for(int i = 0; i < file_length; i++){
        // Aplicamos el filtro
        for(int j = 0; j < file_length; j++){
            filtred_matrix[i][j] = pixel_media(i, j, matrix, file_length);
        }
        fwrite(filtred_matrix[i], sizeof(unsigned char), file_length, new_file);
    }

    printf("FILTRO APLICADO EN ARCHIVO %s Y GUARDADO EN %s...\n",file_name, new_file_name);  

    fclose(file);
    fclose(new_file);
    free(new_file_name);
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

unsigned char pixel_media(const int pixel_row, const int pixel_col, unsigned char** matrix, const int matrix_length){
    long long avg_value = 0;
    const int square_size = 9;
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

    for(i = pixel_row-square_half; i <= pixel_row+square_half; i++){
        for(j = pixel_col-square_half; j <= pixel_col+square_half; j++){
            avg_value += matrix[i][j];
        }
    }

    return (avg_value/square_size);
}