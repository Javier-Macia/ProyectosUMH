#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#include "lib.h"

#define MODO_ME 0
#define MODO_MN 1
#define MODO_SO 2
#define SQ_SIZE 9

#define TAG_FSIZE 0
#define TAG_MATRIZ 1
#define TAG_RESULTADO 2

void usage(char *program_name);
int validar_modo(char *modo);

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    
    int nproces, myrank;
    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    /*************************************/
    //      CONTROL DE LOS ARGUMENTOS
    /*************************************/
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

    int tam = atoi(argv[3]);
    if (tam <= 0){
        printf("Error: Dimensiones de la matriz incorrectas.\n");
        usage(argv[0]);
        return -1;
    }
    
    double fsize = 0;
    if ( myrank == 0){
        // Root lee el archivo
	    fseek(f, 0, SEEK_END);
        fsize = ftell(f);
    	fseek(f, 0, SEEK_SET);
    }
    // Root envía el tamaño del fichero a todos los procesos
    MPI_Bcast(&fsize, 1, MPI_DOUBLE, TAG_FSIZE, MPI_COMM_WORLD);

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


	// Variables únicas de root.
    unsigned char** filtred_matrix_resultado = NULL;
    unsigned char** matrix_real = NULL;
    // Variables del programa
    int desp_prev = 1;
    int desp_post = 0;

    if (myrank == nproces-1){
        desp_post = 0;
    } else {
        desp_post = 1;
    }

    int aux_resto = 0;
    const int resto = tam%nproces;
    if (myrank == nproces-1){
        aux_resto = resto;
    } else aux_resto = 0;

    int tam_proc = (int)(tam/nproces) + aux_resto;

    unsigned char** matrix = (unsigned char**)malloc(sizeof(unsigned char*)*(tam_proc+desp_prev+desp_post));
    unsigned char** filtred_matrix = (unsigned char**)malloc((tam_proc)*sizeof(unsigned char*));

    clock_t start, end;
    double cpu_time_used;

    int i,j;


    // Todos los procesos almacenan espacio para sus matrices (tam_proc x tam).
    for (i=0; i < (tam_proc+desp_post+desp_prev) ; i++)
        matrix[i] = (unsigned char*)malloc(tam*sizeof(unsigned char));
    for (i=0; i < tam_proc; i++)
        filtred_matrix[i] = (unsigned char*)malloc(tam*sizeof(unsigned char));
    
    if (myrank == 0){ // Si somos el proceso 0:
        // Reservamos para almacenar la matriz filtrada  y la matriz real.
        filtred_matrix_resultado = (unsigned char**)malloc(tam*sizeof(unsigned char*));
        matrix_real = (unsigned char**)malloc(tam*sizeof(unsigned char*));
        // Solo 0 lee
    	for (i=0; i<tam; i++){
            filtred_matrix_resultado[i] = (unsigned char*)malloc(tam*sizeof(unsigned char));
            matrix_real[i] = (unsigned char*)malloc(tam*sizeof(unsigned char));
    		fread(matrix_real[i], sizeof(unsigned char), tam, f);
        }

        // Copiamos la parte de la matriz que nos corresponde.
        desp_post = 1;
        desp_prev = 0;
        if (nproces == 1) desp_post = 0;
        
        for (i=0; i<tam_proc+desp_post; i++){
            memcpy(matrix[i], matrix_real[i], tam*sizeof(unsigned char));
        }

        // Y ahora enviamos a los demás procesos su parte de la matriz. Si el proceso es el último, no se le da fila extra posterior.
        for (i=1; i<nproces; i++){
            // Todas reciben fila previa y posterior, menos el último que solo recibe fila previa.
            if (i == nproces-1){
                desp_post = 0;
                desp_prev = 1;
                aux_resto = resto;
            } 
            else {
                desp_prev = 1;
                desp_post = 1;
                aux_resto = 0;
            }

            for (j=0; j<tam_proc+desp_prev+desp_post+aux_resto; j++){
                // Cada fila de la matriz se envía a un proceso.
                MPI_Send(matrix_real[(i*tam_proc)+j-desp_prev], tam, MPI_UNSIGNED_CHAR, i, TAG_MATRIZ, MPI_COMM_WORLD);
                //printf("ENVÍO de la fila %d a proceso %d \n", (i*tam_proc)+j-desp_prev, i);
            }
            //printf("\nSE HA ENVIADO TODO A %d\n", i);
        }
        desp_prev = 0;
        desp_post = 1;
        if (nproces == 1) desp_post = 0;

    } else { // Si somos otro proceso:
        // Recibimos nuestra parte de la matriz.
        // Todos reciben fila previa y posterior, menos el último que solo recibe fila previa.
        for (i=0; i<tam_proc+desp_post+desp_prev; i++){
            MPI_Recv(matrix[i], tam, MPI_UNSIGNED_CHAR, 0, TAG_MATRIZ, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("RECIBO de la fila %d al proceso %d \n", i, myrank);
        }
        //printf("\n %d HA RECIBIDO TODO\n", myrank);
    }

    if (myrank == 0)
        start = clock(); // Empieza el cronómetro el root.

    // Aplicamos el filtro
    switch(modo){
    case MODO_ME:
        filtrado_media(filtred_matrix, tam_proc, tam, matrix, tam_proc, tam, SQ_SIZE, desp_prev, desp_post);
        break;
    case MODO_MN:
        filtrado_mediana(filtred_matrix, tam_proc, tam, matrix, tam_proc, tam, SQ_SIZE, desp_prev, desp_post);
        break;
    case MODO_SO:
        filtrado_sobel(filtred_matrix, tam_proc, tam, matrix, tam_proc, tam, SQ_SIZE, desp_prev, desp_post);
        break;
    default:
        printf("Error: Modo de ejecucion incorrecto.\n");
        usage(argv[0]);
        return -1;
    }

    if (myrank == 0){
        unsigned char* temp = (unsigned char*)malloc(tam*sizeof(unsigned char));

        // Nos asignamos nuestra parte a la matriz resultado
        for (i=0; i<tam_proc; i++){
            memcpy(filtred_matrix_resultado[i], filtred_matrix[i], tam*sizeof(unsigned char));
        }

        // Recibimos las partes de los demás procesos.
        for (i=1; i<nproces; i++){
            if (i == nproces-1){
                desp_post = 0;
                desp_prev = 1;
                aux_resto = resto;
            }
            else {
                desp_post = 1;
                desp_prev = 1;
                aux_resto = 0;
            }
            
            for (j=0; j<tam_proc+aux_resto; j++){
                MPI_Recv(temp, tam, MPI_UNSIGNED_CHAR, i, TAG_RESULTADO, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                memcpy(filtred_matrix_resultado[(i*tam_proc)+j], temp, tam*sizeof(unsigned char));
            }
        }
        //printf("RECIBO de la fila %d al proceso %d \n", (i*tam_proc)+j, i);


    }else{
        // Los procesos envian a root su filtrado 
        for (i=0; i<tam_proc; i++){
            MPI_Send(filtred_matrix[i], tam, MPI_UNSIGNED_CHAR, 0, TAG_RESULTADO, MPI_COMM_WORLD);
        }
    }
    
    if (myrank == 0){ // Root se encarga de parar el cronómetro y de escribir
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

        // Escritura del archivo
        char* new_file_name = "imagen_filtrada.raw";
        FILE* new_f = fopen(new_file_name, "wb");
        for (i=0; i<tam; i++){
            fwrite(filtred_matrix_resultado[i], sizeof(unsigned char), tam, new_f);
        }
        fclose(new_f);

        char* data_file_name = "out.txt";
        FILE* data_f = fopen(data_file_name, "a");
        fprintf(data_f, "EJECUCIÓN DEL PROGRAMA '%s' (FECHA %s | HORA %s)\n", argv[0], __DATE__, __TIME__);
        fprintf(data_f, "Con el archivo de entrada '%s'\n", file_name);
        fprintf(data_f, "Fichero de salida resultado: '%s'\n", new_file_name);
        fprintf(data_f, "Modo de ejecución: '%s'\n", argv[2]);
        fprintf(data_f, "Dimension de la matriz: %s x %s\n", argv[3], argv[3]);
        fprintf(data_f, "Tiempo de ejecución: %f segundos\n", cpu_time_used);
        fprintf(data_f, "\n--------------------------------------------------------------------\n");

        printf("Archivo filtrado guardado en %s.\n", new_file_name);
        printf("Datos de ejecución guardados en %s.\n", data_file_name);

        free(filtred_matrix_resultado);
        free(matrix_real);
	    fclose(f);
        fclose(data_f);
    }

	free(matrix);
    free(filtred_matrix);
    
    MPI_Finalize();
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