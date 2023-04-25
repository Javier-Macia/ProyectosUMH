#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>   
#include <signal.h>   
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>   
#include <sys/ipc.h>   
#include <sys/shm.h>  
#include "../inc/sem.h"
#include "../inc/func.h"

#define SSEED 'a'               //Semilla para los semáforos
#define SHMFILE "~/.profile"    //Fichero único por alumno usado para obtener el id de la memoria compartida
#define SHMSEED 35              //Semilla para obtener el id de la memoria compartida
#define SHMPERMISOS 0644        //Permisos para obtener el id de la memoria compartida


int main( int args, char *argv[] )
{
	int i,j,x,y;  //variables auxiliares
	int pid;      //para contener el pid de los hijos
	int numHijos;  //Numero de hijos (4 o 16)
	int status;    //Status de retorno de los hijos
	char buffer[100];  //buffer auxiliar para mostrar errores
	int shmId;  //Identificador de segmento de memoria compartida
	int shmSize; //Tamaño del segmento de memoria compartida
	int *shmArray; //Puntero donde queremos ubicar el segmento de memoria compartida.
	int *numeros;  //Puntero a un array que contendrá los numeros que genera cada hijo. 
    int numsPorHijo; //Cantidad de números que genera cada hijo.

	//Obtenemos el número de hijos de los paramateros de entrada. 
	//Si no hay exactamente 2 parámetros llamamos a Sintaxis() y terminamos con exit(-1)
	if (args!=2){
		Sintaxis();
		exit(-1);
	}
	
	//printf("Numero de parámetros = %d\n",args);
	//**** INICIO COMPLETAR--------------------------------------------------
	numHijos=atoi(argv[1]);
	//**** FIN COMPLETAR--------------------------------------------------

    //Comprobamos que el parámetro de numHijos sea correcto, es decir un 4 o un 16
	//Si no es correcto se llama a Sintaxis y se termina con exit(-1)
	//printf("Numero de hijos = %d\n",numHijos);
	//**** INICIO COMPLETAR--------------------------------------------------
	if (numHijos!=4 && numHijos!=16){
		Sintaxis();
		exit(-1);
	}
	//**** FIN COMPLETAR--------------------------------------------------

    //Calculamos la cantidad de números que le tocan a cada hijo.
	numsPorHijo=(NFIL*NCOL)/numHijos;

	//Declaramos el identificadordel semáforo que juega el rol de barrera del padre
	int sBarreraPadre;

	//Creamos el semáforo inicializado a cero
	//**** INICIO COMPLETAR--------------------------------------------------
	sBarreraPadre=sCreate(SSEED,0);
	//**** FIN COMPLETAR--------------------------------------------------


	//Declararmos el array sACKS de semáforos para los ACKs de los hijos y los inicializamos a cero
	int sACKS[numHijos];
	//**** INICIO COMPLETAR--------------------------------------------------
	for (i=0;i<numHijos;i++){
		sACKS[i]=0;
	}	
	//**** FIN COMPLETAR--------------------------------------------------

	
	//Obtenemos el tañamo del segmento de memoria a pedir
	shmSize = NFIL * NCOL * sizeof(int);
	
	//Creamos el segmento de memoria compartida y asignamos los punteros.
	//**** INICIO COMPLETAR--------------------------------------------------
	shmId = shmget(SHMSEED, shmSize, SHMPERMISOS);
	shmArray = (int *) shmat(shmId, NULL, 0);
	//printf("Creado segmento de memoria compartida\n");
	//**** FIN COMPLETAR--------------------------------------------------

	//Se inicializa el array de memoria compartida a ceros
	//**** INICIO COMPLETAR--------------------------------------------------
	shmArray = (int*)malloc(shmSize);
	for (i=0;i<NFIL;i++){
		shmArray[i]=0;
	}
	//printf("Inicializado el array de memoria compartida a 0\n");
	//**** FIN COMPLETAR--------------------------------------------------

	//Imprimimos la matriz de memoria compartida, que esta todo a ceros
	PrintMatrix(shmArray);

	//Bucle para crear a los hijos
	//En el bucle los hijos envían su señal, reciben su ACK, operan y terminan.
	for(i=0; i < numHijos; i++)	{ 
		//El padre crea el hijo
		//**** INICIO COMPLETAR--------------------------------------------------
		pid = fork();
		//**** FIN COMPLETAR--------------------------------------------------
		if(pid == -1){   
		   sprintf(buffer,"ERROR. Fork ha fallado al crear al hijo %d\n",i);
		   perror(buffer);
		   exit(-1);
		}
		if(pid == 0){
			//Código hijo

		    //El hijo pide memoria para su array de numeros.
			//**** INICIO COMPLETAR--------------------------------------------------
			numeros = (int*)malloc(sizeof(int)*numsPorHijo);
			//**** FIN COMPLETAR--------------------------------------------------
			
			//El hijo inicializa la semilla de numeros aleatorios con su pid
			//**** INICIO COMPLETAR--------------------------------------------------
			srand(getpid());
			//**** FIN COMPLETAR--------------------------------------------------

			//El hijo Genera sus números 
			//**** INICIO COMPLETAR--------------------------------------------------
			GenerarNumerosHijo(numeros,numsPorHijo);
			//printf("Generados los numeros del hijo %d\n",i);
			//**** FIN COMPLETAR--------------------------------------------------

			//El hijo indica que ha llegado a la barrera
			//**** INICIO COMPLETAR--------------------------------------------------
			//printf("Hijo %d ha llegado a la barrera\n",i);
			sWait(sBarreraPadre);
			//**** FIN COMPLETAR--------------------------------------------------

			//El hijo espera en su semaforo de ACK para seguir.
			//**** INICIO COMPLETAR--------------------------------------------------
			sWait(sACKS[i]);
			//printf("Hijo %d ha recibido el ACK\n",i);
			//**** FIN COMPLETAR--------------------------------------------------
		   
		    //El hijo asigna en la memoria compartida el numero de aciertos
			//**** INICIO COMPLETAR--------------------------------------------------
			AsignarAciertos(shmArray,numeros,i,numHijos);
			//printf("Hijo %d ha asignado los aciertos\n",i);
			//**** FIN COMPLETAR--------------------------------------------------
			
			//El hijo imprime su fichero de aciertos
			//**** INICIO COMPLETAR--------------------------------------------------
			ImprimirAciertos(shmArray,i,numHijos);
			//printf("Hijo %d ha imprimido los aciertos\n",i);
			//**** FIN COMPLETAR--------------------------------------------------
			
			//El hijo se desvincula del array.
			//**** INICIO COMPLETAR--------------------------------------------------
			shmdt(numeros);
			//printf("Hijo %d se ha desvinculado del array y termina.\n",i);
			//**** FIN COMPLETAR--------------------------------------------------
		   
			//El hijo termina, no itera.
			//**** INICIO COMPLETAR--------------------------------------------------
			exit(0);
			//**** FIN COMPLETAR--------------------------------------------------
		}
	}
    //Aquí sólo llega el padre
    
    //El padre espera a que todos los hijos lleguen a la barrera
	//**** INICIO COMPLETAR--------------------------------------------------
	for ( i = 0; i < numHijos; i++){
		printf("Padre ha llegado a la barrera (%d)\n",i);
		sSignal(sBarreraPadre);
		printf("Padre ha recibido el ACK de un hijo (%d)\n", i);
	}
	//**** FIN COMPLETAR--------------------------------------------------
	printf("Padre ha SALIDO de la barrera\n");


	//El padre inicializa la semilla de numeros aleatorios con su pid
	//**** INICIO COMPLETAR--------------------------------------------------
	srand(getpid());
	//**** FIN COMPLETAR--------------------------------------------------

	//El padre genera en la memoria compartida los valores ganadores 
	//**** INICIO COMPLETAR--------------------------------------------------
	GenerarNumerosPadre(shmArray);
	//**** FIN COMPLETAR--------------------------------------------------

	PrintMatrix(shmArray);
	
	//El padre envía los ACKs
	//**** INICIO COMPLETAR--------------------------------------------------
	for (i=0;i<numHijos;i++){
		sSignal(sACKS[i]);
	}
	//**** FIN COMPLETAR--------------------------------------------------
   
	//Bucle para esperar la terminación de los hijos y notificarlo
	//**** INICIO COMPLETAR--------------------------------------------------
	for (i=0;i<numHijos;i++){
		wait(NULL);
		printf("Padre ha recibido la terminación de un hijo (%d)\n",i);
	}
	//**** FIN COMPLETAR--------------------------------------------------

	PrintMatrix(shmArray);
	
	//El padre se desvincula del array.
	//**** INICIO COMPLETAR--------------------------------------------------
	shmdt(shmArray);
	//**** FIN COMPLETAR--------------------------------------------------
	
	//El padre borra el array
	//**** INICIO COMPLETAR--------------------------------------------------
	shmctl(shmId,IPC_RMID,NULL);
	//**** FIN COMPLETAR--------------------------------------------------
	
	return 0;
}
