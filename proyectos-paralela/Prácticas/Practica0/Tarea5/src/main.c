#include <stdio.h>
#include <stdlib.h>

#define BYTESTOREAD 262144// 256 KB

void usage(char* nombrePrograma);

int main(int argc, char* argv[]){
    if (argc != 2){
        usage(argv[0]);
        printf("Error: Se debe pasar un argumento al programa.\n");
        return 1;
    }
    int num=atoi(argv[1]);
    if (num < 0){
        usage(argv[0]);
        printf("Error: El argumento debe ser un número positivo.\n");
        return 1;
    }
    
    char* filename = "fichero2.bin";
    FILE* file = fopen(filename, "rb");
    
    if(file == NULL){
        printf("Error al abrir el fichero %s",filename);
        return -1;   
    }

    unsigned char* buffer = (unsigned char*) malloc(BYTESTOREAD);
    unsigned char* p = buffer;
    int nUChar = BYTESTOREAD/sizeof(unsigned char);

    fread(buffer, BYTESTOREAD, 1, file);

    for (int i = 0; i < nUChar; i++){
        if (*p <= num){
            printf("Número %d en posición %d.\n", *p, i);
        }
        p++;
    }

    printf("Fin de la ejecución del programa '%s'.\n", argv[0]);
    
    fclose(file);
    free(buffer);
    return 0;
}

void usage(char* nombrePrograma){
    printf("Uso: %s <numero>\n", nombrePrograma);
    printf("Donde <numero> es un entero que se comparará con \n");
    printf("el resto de números del fichero 'fichero2.bin'.\n");
}