#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "lib.h"
#define BUFFER 15

void usage(char *program_name);
bool parametros_validos(int argc, char* argv[]);

int main(int argc, char* argv[]){
    if (parametros_validos(argc, argv) == false){
        usage(argv[0]);
        return -1;
    }
    FILE* f = NULL;
    FILE* fout = NULL;
    double** M = NULL;
    double* out = NULL;
	double fbytes = 0;
    const int nproc = 1;
	int numero_elementos = 0;
    int tam = 0;
    int iteraciones = atoi(argv[1]);
    double* vx = (double*)malloc(N*sizeof(double));
    char* new_file_name;
    clock_t start, end;
    double cpu_time_used;
    
    //Inicializamos vx al vector unidad
    for (int i = 0; i < N; i++)
        vx[i] = 1;
        

    if (argc == 2){ // Si solo se pasa el numero de iteraciones
        printf("No se han ingresado archivos de entrada, se generaran aleatoriamente.\n");
        M = crear_matriz(N,time(NULL));
        printf("Matriz creada.\n");
        new_file_name = crear_fichero();
        printf("Fichero creado con el nombre %s.\n", new_file_name);
        rellenar_fichero(new_file_name, M, N);
        printf("Fichero rellenado.\n");


        /***************** COMPROBACION DEL PROFESOR *****************
        for (int j=0;j<N;j++){
            for (int k=0;k<N;k++){
                if (j == k) M[j][k] = 1.0;
                else if (j > k) M[j][k]=(double)50*(j+1)*(k+1)/((double)N*N*10000); //Triangular inferior
                else M[j][k]=(double)-50*(j+1)*(k+1)/((double)N*N*10000); //Triangular superior
            }
        }
        /***************** COMPROBACION DEL PROFESOR *****************/


        printf("Se va a realizar el algoritmo iterativo con la matriz %s.\n", new_file_name);
        start = clock();
        algoritmo_iterativo(M, vx, N, iteraciones);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Algoritmo iterativo realizado.\n");
        printf("Tiempo empleado: %f segundos.\n", cpu_time_used);
        
        imprime_datos_ejecucion(new_file_name, iteraciones, nproc, cpu_time_used, out, sizeof(out)/sizeof(double));
    }

    // Resto de casos
    for (int i = 2; i < argc; i++){
        f = fopen(argv[i], "r");
        if (f == NULL){
            printf("El archivo %s no existe. Se generarán elementos aleatorios en este caso.\n", argv[i]);
            M = crear_matriz(N, time(NULL));
            printf("Matriz creada.\n");
            crear_fichero_nombre(argv[i]);
            new_file_name = argv[i];
            printf("Fichero creado con el nombre %s.\n", argv[i]);
            rellenar_fichero(argv[i], M, N);
            printf("Fichero rellenado.\n");
        }else{
            fseek(f, 0, SEEK_END);
            fbytes = ftell(f);
            fseek(f, 0, SEEK_SET);
            numero_elementos = fbytes/sizeof(double);
            tam = sqrt(numero_elementos);
            if (tam < N){
                printf("El archivo %s no las dimensiones correctas para realizar el algoritmo iterativo (tiene %dx%d y se necesitan %dx%d).\n", argv[i], tam, tam, N,N );
                printf("Se generarán elementos aleatorios en este caso y se guardará esta matriz con otro nombre.\n");
                M = crear_matriz(N, time(NULL));
                printf("Matriz creada.\n");
                new_file_name = crear_fichero();
                printf("Fichero creado con el nombre %s.\n", new_file_name);
                rellenar_fichero(new_file_name, M, N);
                printf("Fichero rellenado.\n");
            }else{
                new_file_name = argv[i];
                printf("El archivo %s tiene las dimensiones correctas para realizar el algoritmo iterativo.\n", new_file_name);
                M = (double**)malloc(N*sizeof(double));
                for (int j = 0; j < N; j++){
                    M[j] = (double*)malloc(N*sizeof(double));
                    fread(M[j], sizeof(double), N, f);
                }
            }    
            fclose(f);
        }

        printf("Se va a realizar el algoritmo iterativo con la matriz %s.\n", new_file_name);
        
        /***************** COMPROBACION DEL PROFESOR *****************
        for (int j=0;j<N;j++){
            for (int k=0;k<N;k++){
                if (j == k) M[j][k] = 1.0;
                else if (j > k) M[j][k]=(double)50*(j+1)*(k+1)/((double)N*N*10000); //Triangular inferior
                else M[j][k]=(double)-50*(j+1)*(k+1)/((double)N*N*10000); //Triangular superior
            }
        }
        /***************** COMPROBACION DEL PROFESOR *****************/

        start = clock();
        algoritmo_iterativo(M, vx, N, iteraciones);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("Algoritmo iterativo realizado.\n");
        printf("Tiempo empleado: %f segundos.\n", cpu_time_used);
        
        imprime_datos_ejecucion(new_file_name, iteraciones, nproc, cpu_time_used, out, sizeof(out)/sizeof(double));        
        
        free(out);
    }
    free(M);
    free(vx);


	return 0;
}

void usage(char* program_name){
	printf("Uso: %s <iteraciones> (nombre_archivo.raw) (nombre_archivo2.raw) (...) (nombre_archivoN.raw) \n", program_name);
	printf("Donde:\n");
	printf("<iteraciones> es el numero de veces que realiza el esquema iterativo.\n");
    printf("(nombre_archivo) es el nombre del archivo a procesar. Es un parámetro opcional y pueden ser varios archivos.\n");
}

bool parametros_validos(int argc, char* argv[]){
    if (argc < 2){
        usage(argv[0]);
        printf("ERROR: No se han introducido argumentos suficientes.\n");
        return false;
    }
    if (argc-2 > BUFFER){
        usage(argv[0]);
        printf("ERROR: Se han introducido demasiados argumentos.\n");
        return false;
    }
    if (atoi(argv[1]) < 0){
        usage(argv[0]);
        printf("ERROR: El numero de iteraciones debe ser mayor o igual que 0.\n");
        return false;
    }
    return true;
}