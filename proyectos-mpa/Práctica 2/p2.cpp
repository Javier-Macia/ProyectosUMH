//Javier Maciá Sempere, Alejandro Company y Unai Coves Seva

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void Intercambiar(int &x,int &y){
    int temp;
    temp = x;
    x = y;
    y = temp;
}

void Seleccion(int *V, int sizeV){
    int i,j,posmin;
    for (i = 1; i < (sizeV)-1; i++){
        posmin = i;
        for (j = i+1; j < sizeV; j++){
            if (V[j] < V[posmin]){
                posmin = j;
            }
        }
        Intercambiar(V[i], V[posmin]);
    }
}

int main(){
    int seed,sizeV,posIni, posFin;  // Variables introducidas por el usuario
    int i,j,posmin;                 // Variables de apoyo
    clock_t tinicio, tfin;          // Variables para calcular el tiempo
    double tiempo;
  
    

    cout <<"Semilla para generar aleatorios: ";
    cin >> seed;

    cout <<"Introduce el tamanyo: ";
    cin >> sizeV;
    int V[sizeV];   //Declaramos un vector con dicho tamaño

    srand(seed);

    cout <<"Posiciones INICIAL y FINAL del vector para mostrar"<<endl;
    cout <<"INCIAL:  ";
    cin >> posIni;

    cout <<"FINAL:  ";
    cin >> posFin;

     //Almacenamos el momento actual
    for (i = 0; i < sizeV; i++){
        V[i] = rand();
    }

    cout <<"\nALGORITMO DE SELECCION"<<endl;

    //////// CASO CUALQUIERA ////////
    tinicio = clock();
    cout <<"\nCASO CUALQUIERA:"<<endl;
    cout <<"-------------------------"<<endl;
    cout <<"Vector a ordenar: ";
    for (i = posIni-1; i <= posFin-1; i++){
        cout <<V[i]<<" ";
    }
    cout<<"\n";

    Seleccion(V,sizeV); // ALGORITMO DE SELECCIÓN

    //Imprimimos los números dentro del rango solicitado
    cout <<"Vector ordenado: ";
    for (i = posIni; i <= posFin; i++){
        cout <<V[i]<<" ";
    }
    cout<<"\n";
    tfin = clock(); // Almacenamos el momento actual

    tiempo = (double)(tfin-tinicio) / CLOCKS_PER_SEC *1000;    // La diferencia de tiempos en milisegundos
    cout << "\n\nEl tiempo de ejecucion en ms es: "<<tiempo<<endl;

    //////// CASO CUALQUIERA ////////
    
    //////// CASO MEJOR ////////
    tinicio = clock();
    cout <<"\nCASO MEJOR:"<<endl;
    cout <<"-------------------------"<<endl;
    cout <<"Vector a ordenar:";
    
    for (i = posIni; i <= posFin; i++){
        cout <<V[i]<<" ";
    }
    cout<<"\n";


    Seleccion(V,sizeV);

    //Imprimimos los números dentro del rango solicitado
    cout <<"Vector ordenado:";
    for (i = posIni; i <= posFin; i++){
        cout <<V[i]<<" ";
    }
    cout<<"\n";

    tfin = clock(); // Almacenamos el momento actual

    tiempo = (double)(tfin-tinicio) / CLOCKS_PER_SEC *1000;    // La diferencia de tiempos en milisegundos
    cout << "\n\nEl tiempo de ejecucion en ms es: "<<tiempo<<endl;

    //////// CASO MEJOR ////////

    
    //////// CASO PEOR ////////
    tinicio = clock();
    cout <<"\nCASO PEOR:"<<endl;
    cout <<"-------------------------"<<endl;
    cout <<"Vector a ordenar: ";
    
    for (i = 1; i < (sizeV)-1; i++){
        posmin = i;
        for (j = i+1; j < sizeV; j++){
            if (V[j] < V[posmin]){
                posmin = j;
            }
        }
        while (i<j){
        Intercambiar(V[i], V[j]);
            i++;
            j--;
        }
    }
    for (i = posIni+1; i <= posFin+1; i++){
        cout <<V[i]<<" ";
    }

    
    cout<<"\n";

    Seleccion(V,sizeV); // ALGORITMO DE SELECCIÓN

    //Imprimimos los números dentro del rango solicitado
    cout <<"Vector ordenado: ";
    for (i = posIni; i <= posFin; i++){
        cout <<V[i]<<" ";
    }
    cout<<"\n";

    //////// CASO PEOR ////////
    


    tfin = clock(); // Almacenamos el momento actual

    tiempo = (double)(tfin-tinicio) / CLOCKS_PER_SEC *1000;    // La diferencia de tiempos en milisegundos
    cout << "\n\nEl tiempo de ejecucion en ms es: "<<tiempo<<endl;
    
    return 0;
}