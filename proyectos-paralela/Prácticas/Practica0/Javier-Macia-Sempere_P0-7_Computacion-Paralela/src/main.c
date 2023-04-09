#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long average(unsigned int* buffer, const int size);

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("No se ha introducido un numero de argumentos valido.\n");
        return -1;
    }
    const char* filename = argv[1];
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        printf("No se ha podido abrir el fichero %s",filename);
        return -1;   
    }
    fseek(file, 0, SEEK_END);
    const int BYTESTOREAD = ftell(file);
    fseek(file, 0, SEEK_SET);
    const int nElements = BYTESTOREAD/sizeof(unsigned int);
    const int matrixSize = sqrt(nElements); 
    unsigned int** buffer = (unsigned int**)malloc(matrixSize*sizeof(unsigned int*));
    long long avgValue = 0;
    int rowIndex,colIndex;
    int changes = 0;
    int quantity = 0;

    for ( rowIndex = 0; rowIndex < matrixSize; rowIndex++){
        quantity = 0;
        buffer[rowIndex] = (unsigned int*)malloc(matrixSize*sizeof(unsigned int));
        fread(buffer[rowIndex], sizeof(unsigned int), matrixSize, file);
        if (rowIndex == 0){
            avgValue = average(buffer[rowIndex], matrixSize);
        }else{
            // Vemos cuántos números son mayores que la media
            for ( colIndex = 0; colIndex < matrixSize; colIndex++)
                if(buffer[rowIndex][colIndex] > avgValue) 
                    quantity++;
            
            // Si tenemos más números mayores que la media que la mitad del tamaño de la matriz
            // Actualizamos la media y cambiamos la fila
            if (quantity > matrixSize/2){
                changes++;
                avgValue = average(buffer[rowIndex], matrixSize);
            }
            
            if (quantity <= matrixSize*0.05){
                printf("Media de la fila %d: %u\n",rowIndex+1, avgValue);
                printf("Esto es menor que el 5%% del tamaño de la matriz (%d*0.05=%f)\n",matrixSize,matrixSize*0.05);
                printf("Dejamos de leer el fichero.\n");
                break;
            }
        }
    }
    
    printf("Cambios realizados: %d\n",changes);
    printf("Media: %u\n",avgValue);

    fclose(file);
    free(buffer);
    return 0;
}

long long average(unsigned int* buffer, const int nElements){
    double avgValue = 0;
    for (int i = 0; i < nElements; i++)
        avgValue += buffer[i];
    avgValue /= nElements;
    return avgValue;
}