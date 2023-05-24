#include <iostream>

using namespace std;

int main(){
    int i, n, nF, nC;
    float *V;
    int **M;
    int A[3][3];

    ///////////////RESERVA DE MEMORIA DINÁMICA///////////////////

    //n toma un valor entero positivo
    n = 20;
    V = new float [n+1];
    if (V == NULL){
        cout << "Error al reservar memoria del vector" <<endl;
        return -1; // exit (-1); 
        //Con exit acabas el programa, y con return acabas la función. Por eso, en el main, da igual cuál poner.
    }else{
        cout << "Se ha reservado la memoria del vector correctamente" <<endl;
    }

    ///////////////RESERVA DE MEMORIA DINÁMICA///////////////////



    ///////////////MATRICES DINÁMICAS///////////////////

    M = new int* [nF+1];
    if (M == NULL){
        cout << "ERROR reservando la memoria de las filas de la matriz dinámica";
        return -1;
    }
    for (i = 0; i <= nF; i++){
        M[i] = new int [nC + 1];
        if (M[i] == NULL){
            cout << "ERROR reservando la memoria de las columnas de la matriz dinámica";
            return -1;
        }
        
    }
    
    ///////////////MATRICES DINÁMICAS///////////////////
    
    ///////////////ENTRADA Y SALIDA////////////////////

    free(M);
    free(V);


    return 0;
}