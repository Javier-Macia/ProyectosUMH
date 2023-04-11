#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>

#include "lib.h"
#define BUFFER 15
#define TAG_MATRIZ 0
#define NUM_PROCESOS 7

void usage(char *program_name);
bool parametros_validos(int argc, char* argv[]);

int main(int argc, char* argv[]){
    if (parametros_validos(argc, argv) == false){
        usage(argv[0]);
        return -1;
    }
    
    // Resto de variables
    FILE* f = NULL;
    double** M = NULL;
	double fbytes = 0;
	int numero_elementos = 0;
    int tam = 0;
    int iteraciones = atoi(argv[1]);
    double* vx = NULL;
    char* new_file_name;
    double start, end, cpu_time_used;

    int i,j,k;

    // Reservamos memoria para el resto de procesos que no sean 0
    M = (double**)malloc(N*sizeof(double*));
    M[0] = (double*)malloc(N*N*sizeof(double));
    // Cada fila apunta al comienzo de la misma.
    for (int i = 1; i < N; i++){
        M[i] = M[i-1] + N;
    }

    vx = (double*)malloc(N*sizeof(double));
    for (i=0; i<N;i++){
        vx[i] = 1;
    }
    
    if (argc == 2){ // Si solo se pasa el numero de iteraciones
        printf("No se han ingresado archivos de entrada, se generaran aleatoriamente.\n");
        printf("Espere, por favor...\n");
        
        // Se crea la matriz. Aquí se reserva la memoria necesitada automáticamente.
        M = crear_matriz(N,N,0,time(NULL)); 

        // Se crea el fichero por si se quiere utilizar en otra ejecución
        new_file_name = crear_fichero();
        printf("Fichero creado con el nombre %s.\n", new_file_name);
        rellenar_fichero(new_file_name, M, N);
        printf("Fichero rellenado.\n");
        printf("Se va a realizar el algoritmo iterativo con la matriz %s.\n", new_file_name);

        // Si queremos ver la matriz del profesor, descomentar las siguientes líneas para asi modificar el valor de M
        /***************** COMPROBACION DEL PROFESOR *****************
        printf("SE HA ACTIVADO LA COMPROBACION DEL PROFESOR.\n");
        printf("Se ha creado la matriz, pero se va a modificar para que sea la del profesor.\n");
        for (int j=0;j<N;j++){
            for (int k=0;k<N;k++){
                if (j == k) 
                    M[j][j] = 1.0;
                else if (j > k) 
                    M[j][k] = ((double)50*(j+1)*(k+1))/((double)N*N*10000); //Triangular inferior
                else 
                    M[j][k] = ((double)-50*(j+1)*(k+1))/((double)N*N*10000); //Triangular superior
            }
        }
        printf("Matriz modificada.\n");
        /***************** COMPROBACION DEL PROFESOR *****************/

        
        /***************** ALGORITMO ITERATIVO *****************/
        // Enviamos el trozo de la matriz correspondiente para cada proceso
        double* res = (double*)malloc(N*sizeof(double));
        double max_vabs = 0, min_vabs = 0;
        int altura = N, anchura=N;
        double sum;
        double arr_absolutos[iteraciones-1];


        //ACTIVAMOS EL CRONÓMETRO
        printf("\n\nACTIVAMOS EL CRONOMETRO.\n");
        int tam_proc;
        double* temp;

        start = omp_get_wtime();

        #pragma omp parallel num_threads(NUM_PROCESOS) shared(M,vx,res,temp,max_vabs,min_vabs,arr_absolutos,iteraciones, altura, anchura) private(sum,tam_proc,j,k) default(none)
        {       
            tam_proc = altura/NUM_PROCESOS;
            #pragma omp master // El 0 hace más trabajo
            {
                tam_proc += altura%omp_get_num_threads();
            }
            // PRIMERA ITERACION
            #pragma omp for schedule(dynamic,tam_proc)
            for (j = 0; j < altura; j++){
                sum = 0;
                for (k = 0; k < anchura; k++){
                    sum += M[j][k] * vx[k];
                }
                res[j] = sum;
            }
            #pragma omp single 
            {
                temp = vx;
                vx = res;
                res = temp;
            }


            // RESTO DE ITERACIONES
            for (int i=0; i < iteraciones-1; i++){
                max_vabs=0, min_vabs=0;
                // Multiplicamos la matriz por el vector
                #pragma omp for schedule(dynamic, tam_proc) reduction(max : max_vabs) reduction(min : min_vabs)
                for (j = 0; j < altura; j++){
                    sum = 0;
                    for (k = 0; k < anchura; k++){
                        sum += M[j][k] * vx[k];
                    }
                    res[j] = sum;

                    if(res[j] < min_vabs){
                        min_vabs = res[j];
                    }
                    if(res[j] > max_vabs){
                        max_vabs = res[j];
                    }
                }

                #pragma omp single
                {
                    if(fabs(max_vabs) > fabs(min_vabs))
                        arr_absolutos[i] = max_vabs;
                    else
                        arr_absolutos[i] = min_vabs;
                }
                    
                // Si es mayor que 25, se divide 
                if (fabs(arr_absolutos[i]) > 25){
                    #pragma omp for schedule(dynamic, tam_proc)
                    for ( j = 0; j < altura; j++){
                        res[j] /= arr_absolutos[i];
                    }
                }

                // Se copia el resultado en la columna anterior
                #pragma omp single
                {
                    temp = vx;
                    vx = res;
                    res = temp;
                }

                #pragma omp barrier
            }
        }
        end = omp_get_wtime();
        printf("CRONOMETRO DETENIDO.\n\n");
        cpu_time_used = end-start;

        for (i=0; i < iteraciones-1; i++){
            printf("Iteracion %d: max_vabs = %f\n", i+1, arr_absolutos[i]);
        }

        printf("Algoritmo iterativo realizado.\n");
        printf("Tiempo empleado: %f segundos.\n", cpu_time_used);
        imprime_datos_ejecucion(new_file_name, NUM_PROCESOS, cpu_time_used, arr_absolutos, iteraciones-1);
        /***************** ALGORITMO ITERATIVO *****************/

    }else {
        // Resto de casos
        for (int i = 2; i < argc; i++){
            f = fopen(argv[i], "r");
            // Caso en el que no exista el fichero.
            if (f == NULL){
                printf("El archivo %s no existe. Se generarán elementos aleatorios en este caso.\n", argv[i]);
                M = crear_matriz(N, N, 0, time(NULL));
                printf("Matriz creada.\n");
                crear_fichero_nombre(argv[i]);
                new_file_name = argv[i];
                printf("Fichero creado con el nombre %s.\n", argv[i]);
                rellenar_fichero(argv[i], M, N);
                printf("Fichero rellenado.\n");
            }
            // Caso en el que el fichero exista.
            else{
                fseek(f, 0, SEEK_END);
                fbytes = ftell(f);
                fseek(f, 0, SEEK_SET);
                numero_elementos = fbytes/sizeof(double);
                tam = sqrt(numero_elementos);
                if (tam < N){
                    printf("El archivo %s no las dimensiones correctas para realizar el algoritmo iterativo (tiene %dx%d y se necesitan %dx%d).\n", argv[i], tam, tam, N,N );
                    printf("Se generarán elementos aleatorios en este caso y se guardará esta matriz con otro nombre.\n");
                    M = crear_matriz(N, N, 0, time(NULL));
                    printf("Matriz creada.\n");
                    new_file_name = crear_fichero();
                    printf("Fichero creado con el nombre %s.\n", new_file_name);
                    rellenar_fichero(new_file_name, M, N);
                    printf("Fichero rellenado.\n");
                }else{
                    new_file_name = argv[i];
                    printf("El archivo %s tiene las dimensiones correctas para realizar el algoritmo iterativo.\n", new_file_name);
                    fread(M[0], sizeof(double), N*N, f);
                }    
                fclose(f);
            }

            printf("Se va a realizar el algoritmo iterativo con la matriz %s.\n", new_file_name);

            /***************** COMPROBACION DEL PROFESOR *****************/
            printf("SE HA ACTIVADO LA COMPROBACION DEL PROFESOR.\n");
            printf("Se ha creado la matriz, pero se va a modificar para que sea la del profesor.\n");
            for (int j=0;j<N;j++){
                for (int k=0;k<N;k++){
                    if (j == k) M[j][k] = 1.0;
                    else if (j > k) M[j][k]=(double)50*(j+1)*(k+1)/((double)N*N*10000); //Triangular inferior
                    else M[j][k]=(double)-50*(j+1)*(k+1)/((double)N*N*10000); //Triangular superior
                }
            }
            /***************** COMPROBACION DEL PROFESOR *****************/

            /***************** ALGORITMO ITERATIVO *****************/
            // Enviamos el trozo de la matriz correspondiente para cada proceso
            double* res = (double*)malloc(N*sizeof(double));
            double max_vabs = 0, min_vabs = 0;
            //int pos[iteraciones];
            int altura = N, anchura=N;
            double sum;
            double arr_absolutos[iteraciones-1];


            //ACTIVAMOS EL CRONÓMETRO
            printf("\n\nACTIVAMOS EL CRONOMETRO.\n");
            int tam_proc;
            double* temp;

            start = omp_get_wtime();

            #pragma omp parallel num_threads(NUM_PROCESOS) shared(M,vx,res,temp,max_vabs,min_vabs,arr_absolutos,iteraciones, altura, anchura) private(sum,tam_proc,j,k) default(none)
            {       
                tam_proc = altura/NUM_PROCESOS;
                #pragma omp master // El 0 hace más trabajo
                {
                    tam_proc += altura%omp_get_num_threads();
                }
                // PRIMERA ITERACION
                #pragma omp for schedule(dynamic,tam_proc)
                for (j = 0; j < altura; j++){
                    sum = 0;
                    for (k = 0; k < anchura; k++){
                        sum += M[j][k] * vx[k];
                    }
                    res[j] = sum;
                }
                #pragma omp single 
                {
                    temp = vx;
                    vx = res;
                    res = temp;
                }


                // RESTO DE ITERACIONES
                for (int i=0; i < iteraciones-1; i++){
                    max_vabs=0, min_vabs=0;
                    // Multiplicamos la matriz por el vector
                    #pragma omp for schedule(dynamic, tam_proc) reduction(max : max_vabs) reduction(min : min_vabs)
                    for (j = 0; j < altura; j++){
                        sum = 0;
                        for (k = 0; k < anchura; k++){
                            sum += M[j][k] * vx[k];
                        }
                        res[j] = sum;

                        if(res[j] < min_vabs){
                            min_vabs = res[j];
                        }
                        if(res[j] > max_vabs){
                            max_vabs = res[j];
                        }
                    }

                    #pragma omp single
                    {
                        if(fabs(max_vabs) > fabs(min_vabs))
                            arr_absolutos[i] = max_vabs;
                        else
                            arr_absolutos[i] = min_vabs;
                    }
                        
                    // Si es mayor que 25, se divide 
                    if (fabs(arr_absolutos[i]) > 25){
                        #pragma omp for schedule(dynamic, tam_proc)
                        for ( j = 0; j < altura; j++){
                            res[j] /= arr_absolutos[i];
                        }
                    }

                    // Se copia el resultado en la columna anterior
                    #pragma omp single
                    {
                        temp = vx;
                        vx = res;
                        res = temp;
                    }

                    #pragma omp barrier
                }
            }
            end = omp_get_wtime();
            printf("CRONOMETRO DETENIDO.\n\n");
            cpu_time_used = end-start;

            for (i=0; i < iteraciones-1; i++){
                printf("Iteracion %d: max_vabs = %f\n", i+1, arr_absolutos[i]);
            }

            printf("Algoritmo iterativo realizado.\n");
            printf("Tiempo empleado: %f segundos.\n", cpu_time_used);
            imprime_datos_ejecucion(new_file_name, NUM_PROCESOS, cpu_time_used, arr_absolutos, iteraciones-1);
            /***************** ALGORITMO ITERATIVO *****************/
        }

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