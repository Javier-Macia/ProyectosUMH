#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "mpi.h"

#include "lib.h"
#define BUFFER 15
#define TAG_MATRIZ 0
#define TAG_VABS 1
#define TAG_POSVABS 2
#define TAG_VABSFINAL 3

void usage(char *program_name);
bool parametros_validos(int argc, char* argv[]);

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    // Variables para MPI
    int myrank, nproc;
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (parametros_validos(argc, argv) == false){
        if (myrank == 0){
            usage(argv[0]);
        }
        return -1;
    }
    
    // Resto de variables
    FILE* f = NULL;
    FILE* fout = NULL;
    double** M = NULL;
    double* aux = NULL;
    double* out = NULL;
	double fbytes = 0;
	int numero_elementos = 0;
    int tam = 0;
    int iteraciones = atoi(argv[1]);
    double* vx = NULL;
    char* new_file_name;
    clock_t start, end;
    double cpu_time_used;

    int desp_diag_ppal = N-(N-myrank*(N/nproc));
    int displs[nproc]; // Desplazamientos
    int sendcounts[nproc]; // Cantidad de elementos a enviar a cada proceso
    int tamproc [nproc]; // Tamaño de cada proceso
    int filasproc [nproc]; // Filas de cada proceso
    double arr_max_vabs[nproc]; // Valor absoluto máximo de cada proceso
    
    int recvcounts_vabs[nproc]; // Cantidad de elementos a enviar a cada proceso para el valor absoluto máximo (Serán todo 1s)
    int displs_vabs [nproc]; // Tamaño de cada proceso para el valor absoluto máximo (Serán todo 1s)
    
    int displs_col[nproc]; // Desplazamientos
    int sendcounts_col[nproc]; // Cantidad de elementos a enviar a cada proceso


    const int sizeproc = N/nproc;
    int resto = 0;

    int i,j,k,cont;
    
    // Inicializamos el vector de desplazamientos, envíos y tamaño de cada proceso
    for (int i=0; i<nproc; i++){
        if (i==nproc-1){
            resto = N%nproc;
        }else{
            resto = 0;
        } 
        
        filasproc[i] = sizeproc+resto;
        tamproc[i] = N*filasproc[i];
        sendcounts[i] = tamproc[i];
        sendcounts_col[i] = filasproc[i];
        displs_col[i] = sendcounts_col[i]*i;
        displs[i] = sendcounts[i]*i;
        recvcounts_vabs[i] = 1;
        displs_vabs[i] = i;
    }

    // Reservamos memoria para el resto de procesos que no sean 0
    if (myrank != 0){
        M = (double**)malloc( (filasproc[myrank])*sizeof(double*));
        M[0] = (double*)malloc( tamproc[myrank]*sizeof(double));
        // Cada fila apunta al comienzo de la misma.
        for (int i = 1; i < filasproc[myrank]; i++){
            M[i] = M[i-1] + N;
        }
    }

    vx = (double*)malloc(N*sizeof(double));
    for (int i = 0; i < N; i++){
        vx[i] = 1;
    }
    
    if (argc == 2){ // Si solo se pasa el numero de iteraciones
        if (myrank == 0)
            printf("No se han ingresado archivos de entrada, se generaran aleatoriamente.\n");
        
        if (myrank == 0){
            // Se crea la matriz. Aquí se reserva la memoria necesitada automáticamente.
            M = crear_matriz(N,N,desp_diag_ppal,time(NULL)); 

            // Se crea el fichero por si se quiere utilizar en otra ejecución
            new_file_name = crear_fichero();
            printf("Fichero creado con el nombre %s.\n", new_file_name);
            rellenar_fichero(new_file_name, M, N);
            printf("Fichero rellenado.\n");
            printf("Se va a realizar el algoritmo iterativo con la matriz %s.\n", new_file_name);
        }


        // Si queremos ver la matriz del profesor, descomentar las siguientes líneas para asi modificar el valor de M
        /***************** COMPROBACION DEL PROFESOR *****************
        if (myrank == 0){
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
        }
        /***************** COMPROBACION DEL PROFESOR *****************/

        
        /***************** ALGORITMO ITERATIVO *****************/
        // Enviamos el trozo de la matriz correspondiente para cada proceso
        MPI_Scatterv(M[0], sendcounts, displs, MPI_DOUBLE, M[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        double *aux = (double*)malloc(filasproc[myrank]*sizeof(double));
        double* res = (double*)malloc(filasproc[myrank]*sizeof(double));
        double* anterior = (double*)malloc(N*sizeof(double)); // Contiene toda la columna anterior de la matriz
        double max_vabs=0;
        double vabs=0;
        int pos = -1;
        double temp_max=0;

        // Copiamos vx en aux
        for (int i=0; i<N; i++){
            aux[i] = 1;
        }

        //ACTIVAMOS EL CRONÓMETRO
        if (myrank == 0)
            start = clock();

        for ( i=1; i <= iteraciones; i++){
            for (j=0; j < N; j++){
                max_vabs = 0;
                temp_max = 0;
                mult_vectores(M, anterior, res, filasproc[myrank], N);


                if (iteraciones == 0){
                    MPI_Allgatherv(res, filasproc[myrank], MPI_DOUBLE, anterior, sendcounts_col, displs_col, MPI_DOUBLE, MPI_COMM_WORLD);
                    continue; // Si es la primera iteración, no se hace nada
                } 

                // Sacamos maximo valor absoluto
                for (k=0; k<filasproc[myrank]; k++){
                    if (valor_absoluto(res[k]) > valor_absoluto(max_vabs)){
                        max_vabs = res[k];
                        pos = k;
                    }
                }
                // Una vez obtenido el valor absoluto máximo, se envía a todos los procesos
                MPI_Allgatherv(&max_vabs, 1, MPI_DOUBLE, arr_max_vabs, recvcounts_vabs, displs_vabs, MPI_DOUBLE, MPI_COMM_WORLD);

                // Se obtiene el valor absoluto máximo de todos los procesos
                for (k=0; k<nproc; k++){
                    if (valor_absoluto(arr_max_vabs[k]) > valor_absoluto(temp_max)){
                        temp_max = arr_max_vabs[k];
                    }
                }

                // Si es mayor que 25, se divide 
                if (valor_absoluto(temp_max) > 25){
                    for (int k = 0; k < filasproc[myrank]; k++){
                        aux[k] /= temp_max;
                    }
                }
                // Juntamos cada trozo de columna en un vector "anterior"
                MPI_Allgatherv(aux, filasproc[myrank], MPI_DOUBLE, anterior, sendcounts_col, displs_col, MPI_DOUBLE, MPI_COMM_WORLD);
            }
            if (myrank == 0)
                printf("Iteración %d, valor absoluto máximo: %f\n", i, temp_max);
        }
        
        if (myrank == 0){
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            printf("Algoritmo iterativo realizado.\n");
            printf("Tiempo empleado: %f segundos.\n", cpu_time_used);
            imprime_datos_ejecucion(new_file_name, iteraciones, nproc, cpu_time_used, out, sizeof(out)/sizeof(double));
        }
        /***************** ALGORITMO ITERATIVO *****************/

    }
    else {
        // Resto de casos
        for (int i = 2; i < argc; i++){
            f = fopen(argv[i], "r");
            // Caso en el que no exista el fichero.
            if (f == NULL){
                printf("El archivo %s no existe. Se generarán elementos aleatorios en este caso.\n", argv[i]);
                M = crear_matriz(sizeproc, N, desp_diag_ppal, time(NULL));
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
                    M = crear_matriz(sizeproc, N, desp_diag_ppal, time(NULL));
                    printf("Matriz creada.\n");
                    new_file_name = crear_fichero();
                    printf("Fichero creado con el nombre %s.\n", new_file_name);
                    rellenar_fichero(new_file_name, M, N);
                    printf("Fichero rellenado.\n");
                }else{
                    new_file_name = argv[i];
                    printf("El archivo %s tiene las dimensiones correctas para realizar el algoritmo iterativo.\n", new_file_name);
                    M = (double**)malloc(N*sizeof(double));
                    for (int j = 0; j < N*N; j++){
                        M[0] = (double*)malloc(N*N*sizeof(double));
                        fread(M[j], sizeof(double), N*N, f);
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



            /***************** ALGORITMO ITERATIVO *****************/
            // Enviamos el trozo de la matriz correspondiente para cada proceso
            MPI_Scatterv(M[0], sendcounts, displs, MPI_DOUBLE, M[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            double *aux = (double*)malloc(filasproc[myrank]*sizeof(double));
            double* res = (double*)malloc(filasproc[myrank]*sizeof(double));
            double* anterior = (double*)malloc(N*sizeof(double)); // Contiene toda la columna anterior de la matriz
            double max_vabs=0;
            double vabs=0;
            int pos = -1;
            double temp_max=0;

            // Copiamos vx en aux
            for (int i=0; i<N; i++){
                aux[i] = 1;
            }

            //ACTIVAMOS EL CRONÓMETRO
            if (myrank == 0)
                start = clock();


            for ( i=1; i <= iteraciones; i++){
                for (j=0; j < N; j++){
                    max_vabs = 0;
                    temp_max = 0;
                    mult_vectores(M, anterior, res, filasproc[myrank], N);


                    if (iteraciones == 0){
                        MPI_Allgatherv(res, filasproc[myrank], MPI_DOUBLE, anterior, sendcounts_col, displs_col, MPI_DOUBLE, MPI_COMM_WORLD);
                        continue; // Si es la primera iteración, no se hace nada
                    } 

                    // Sacamos maximo valor absoluto
                    for (k=0; k<filasproc[myrank]; k++){
                        if (valor_absoluto(res[k]) > valor_absoluto(max_vabs)){
                            max_vabs = res[k];
                            pos = k;
                        }
                    }
                    // Una vez obtenido el valor absoluto máximo, se envía a todos los procesos
                    MPI_Allgatherv(&max_vabs, 1, MPI_DOUBLE, arr_max_vabs, recvcounts_vabs, displs_vabs, MPI_DOUBLE, MPI_COMM_WORLD);

                    // Se obtiene el valor absoluto máximo de todos los procesos
                    for (k=0; k<nproc; k++){
                        if (valor_absoluto(arr_max_vabs[k]) > valor_absoluto(temp_max)){
                            temp_max = arr_max_vabs[k];
                        }
                    }

                    // Si es mayor que 25, se divide 
                    if (valor_absoluto(temp_max) > 25){
                        for (int k = 0; k < filasproc[myrank]; k++){
                            aux[k] /= temp_max;
                        }
                    }
                    // Juntamos cada trozo de columna en un vector "anterior"
                    MPI_Allgatherv(aux, filasproc[myrank], MPI_DOUBLE, anterior, sendcounts_col, displs_col, MPI_DOUBLE, MPI_COMM_WORLD);
                }
                if (myrank == 0)
                    printf("Iteración %d, valor absoluto máximo: %f\n", i, temp_max);
            }
            
            if (myrank == 0){
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Algoritmo iterativo realizado.\n");
                printf("Tiempo empleado: %f segundos.\n", cpu_time_used);
                imprime_datos_ejecucion(new_file_name, iteraciones, nproc, cpu_time_used, out, sizeof(out)/sizeof(double));
            }
            /***************** ALGORITMO ITERATIVO *****************/


        }
    }

    free(M);
    free(vx);

    MPI_Finalize();
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