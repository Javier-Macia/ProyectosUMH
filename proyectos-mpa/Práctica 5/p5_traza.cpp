//Unai Coves Seva, Alejandro Company García y Javier Maciá Sempere
#include <iostream>
#include <stdlib.h>

using namespace std;

void intercambiar(int *V, int &i, int &d){
    int temporal = V[i];
    V[i] = V[d];
    V[d] = temporal;
}

int seleccionar_pivote(int *V, unsigned int izq, unsigned int der,int estrategia,int tamVector,int &contador){
    int pivote = 0;
    int p;
    int iter=1;
    contador += 1;

    if (estrategia == 1){ // Estrategia Mediana de tres elementos del vector
        
        if (der != izq){    //Si la posición derecha e izquierda son diferentes, se ejecutará el algoritmo. En caso contrario, la función devolverá un pivote 0.
            p = (der+izq)/2;
            
            if (V[izq]<V[p]){
                if (V[p]<V[der]){
                    pivote = p;
                }else{
                    if(V[izq]>V[der]){
                        pivote = izq;
                    }
                    else{
                    pivote = der;
                    }
                } 
            }else{
                if (V[izq]>V[der]){
                    pivote = izq;
                }else{
                    pivote = der;
                }
            }
        }     
             
    }else if (estrategia == 2){ // Estrategia Mayor de los dos primeros números
        if (izq+1<sizeof(V)){
            if (V[izq]<=V[izq+1]){
                pivote = izq+1;
            }else{
                pivote = izq;
            }
        }else{
            pivote = izq; 
        }
    }

    cout<<contador<<".- quicksort(V,"<<izq+1<<","<<der+1<<")"<<endl; //Tener en cuenta que se imprimirán los números a partir del 0 !
    cout<<"p = "<<pivote+1<<"\npivote = "<<V[pivote]<<endl;

    cout <<"["<<V[0]<<" , ";
    for (iter = 1; iter < tamVector-1; iter++){
        cout<<V[iter]<<" , ";
    }
    cout <<V[iter]<<"]"<<endl;
    
    cout<<"----"<<endl;

    return pivote;
}

void quicksort(int *V, unsigned int izq, unsigned int der, unsigned int estrategia, int tamVector,int &contador){
    int i,d,p;
    int pivote;
    estrategia = 1;

    p = seleccionar_pivote(V,izq,der,estrategia,tamVector,contador); //0 si no pivote
    if (p>=0){
        pivote = V[p];
        i = izq;
        d = der;

        while (i<=d){
            while (V[i]<pivote){
                i++;
            }
            while (V[d]>pivote){
                d--;
            }
            if (i<=d){
                intercambiar(V,i,d);
                i++;
                d--;
            }

            if (izq<d){
                quicksort(V,izq,d,estrategia, tamVector,contador);
            }
            if (i<der){
                quicksort(V,i,der,estrategia, tamVector,contador);
            }
        }
    }
    
}

int main(){
    int tamVector;
    int izq, der;
    int estrategia = 1;
    int i;
    int contador = 0;
    cout<<"Introduce un tamanyo a tu vector:  ";
    cin>>tamVector;
    int V[tamVector];

    cout<<"A continuacion, introduce elementos a tu vector: "<<endl;

    for (i = 0; i < tamVector; i++){
        cin>>V[i];
    }

    cout <<"Este es tu vector: ";
    cout <<"[ "<<V[0]<<" , ";
    for (i = 1; i < tamVector-1; i++){
        cout<<V[i]<<" , ";
    }
    cout <<V[i]<<" ]"<<endl;
    
    cout <<"Ordenamos el vector: "<<endl;
    izq = 0;
    der = i;

    quicksort(V,izq,der,estrategia, tamVector,contador);


    cout <<"Este es tu vector ordenado: ";
    cout <<"[ "<<V[0]<<" , ";
    for (i = 1; i < tamVector-1; i++){
        cout<<V[i]<<" , ";
    }
    cout <<V[i]<<" ]"<<endl;
    

    cout <<"FIN...";
}