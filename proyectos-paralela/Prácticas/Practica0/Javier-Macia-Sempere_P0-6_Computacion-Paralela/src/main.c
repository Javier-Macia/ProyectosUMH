#include <stdio.h>
#include <stdlib.h>

#define BYTESTOREAD 262144// 256 KB

int main(int argc, char* argv[]){
    char* filename = "fichero2.bin";
    FILE* file = fopen(filename, "rb");
    
    if(file == NULL){
        printf("Error al abrir el fichero %s",filename);
        return -1;   
    }
    unsigned char* buffer = (unsigned char*)malloc(BYTESTOREAD);
    unsigned char* paresChar = buffer;
    unsigned short int* paresShort =  buffer;
    unsigned int* paresInt = buffer;
    fread(buffer, 1, BYTESTOREAD, file);
    int i=0;
    int numParesChar = 0, numParesShort = 0, numParesInt = 0;
    int numChar = 0, numShort = 0, numInt = 0;

    // Contar con char
    while(i<BYTESTOREAD){
        if( ((*paresChar)%2) == 0)
            numParesChar++;
        paresChar++;
            
        i+=sizeof(unsigned char);
        numChar++;
    }

    // Contar con short
    i=0;
    while(i<BYTESTOREAD){
        if( ((*paresShort)%2) == 0)
            numParesShort++;
        paresShort++;

        i+=sizeof(unsigned short int);
        numShort++;
    }

    // Contar con int
    i=0;
    while(i<BYTESTOREAD){
        if( ((*paresInt)%2) == 0)
            numParesInt++;
        paresInt++;

        i+=sizeof(unsigned int);
        numInt++;
    }
    fclose(file);
    printf("Numero de chars leidos: %d\n", numChar);
    printf("Numero de pares tipo char: %d\n\n", numParesChar);
    
    printf("Numero de shorts leidos: %d\n", numShort);
    printf("Numero de pares tipo short: %d\n\n", numParesShort);
    
    printf("Numero de ints leidos: %d\n", numInt);
    printf("Numero de pares tipo int: %d\n\n", numParesInt);
    printf("FIN...\n");
    return 0;
}