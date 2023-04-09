#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMDATOS 10

int main(int argc, char *argv[]){   
    char* filenameDatos="datos10dobles.bin";
    char* filenameDiezDecimales="10decimales.txt";
    char* filenameCincoDecimales="5decimales.txt";

    FILE* fDatos = fopen(filenameDatos,"rb");
    FILE* fDiezDecimales = fopen(filenameDiezDecimales,"w");
    FILE* fCincoDecimales = fopen(filenameCincoDecimales,"w");

    double buffer;

    if (fDatos==NULL || fDiezDecimales==NULL || fCincoDecimales==NULL){
        printf("Error al abrir uno de los archivos.\n");
        return -1;    
    }
    
    for (int i = 0; i < NUMDATOS; i++){
        //Leemos el fichero
        fread(&buffer, sizeof(double), 1, fDatos);

        //Mostramos por pantalla
        printf("Numero 5 decimales->%.5f\n", buffer);
        printf("Numero 10 decimales->%.10f\n", buffer);
        printf("\n");

        //Imprimimos resultado en ambos archivos
        fprintf(fDiezDecimales, "%.10f\n", buffer );
        fprintf(fCincoDecimales, "%.5f\n", buffer );
    }

    fclose(fDatos);
    fclose(fDiezDecimales);
    fclose(fCincoDecimales);

    printf("Fin de la ejecución del programa %s.\n", argv[0]);
    return 0;
}
	
