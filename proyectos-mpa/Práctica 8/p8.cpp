// Javier Maciá Sempere
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

float CalcularSuma(bool *X, float P_V[],int k){
    float sol = 0;
    int i;
    for (i = 0; i < k; i++){
        if (X[i]==1){           // Si hay un 1 en ese array, se sumará el valor de P o de V a la solución.
            sol = sol + P_V[i];
        }
    }
    return sol;
}

void Mochila(int n, float M, float *P,float *V,bool X[], int k, bool Xoptimo[], float &Voptimo){
    int i,j;
    float peso, valor;

    if (k<=n){
        for (i = 0; i <= 1; i++){
            X[k] = i;
            peso = CalcularSuma(X,P,k);
            if (peso <= M){
                if (k == n){
                    valor = CalcularSuma(X,V,k);
                    if (valor>Voptimo){
                        for (i = 0; i < n; i++){
                            Xoptimo[i] = X[i];
                        }
                        Voptimo = valor;
                        
                        cout <<"[ ";
                        for (j = 0; j < n; j++){
                            cout<<X[j]<<" ";
                        }
                        cout <<"]";
                        cout<<"   Valor -> "<<valor<<endl;
                    }
                }else{
                    Mochila(n,M,P,V,X,k+1,Xoptimo,Voptimo);
                }
            } 
        }
    }
}

int main(){
    int n;          // n se pedirá al usuario y es la cantidad de objetos
    double M;       // M se pedirá al usuario y es el peso máximo que soporta la mochila
    int i,j;        // iteradores

    int k = 0;      // Nivel
    float Voptimo = -1 * INFINITY;  //Valor óptimo (es -inf porque queremos maximizar el valor)

    cout<<"Introduzca la cantidad de objetos: "<<endl;
    cin>>n;
    
    cout<<"Introduzca el peso maximo que soporta la mochila: "<<endl;
    cin>>M;

    float V[n];      // Valor del objeto.
    float P[n];      // Peso del objeto.
    int O[n];        // El objeto en si.
    bool X[n];       // Si el objeto entra o no.
    bool Xoptimo[n]; // Objetos que sí son óptimos de introducir.

    for(i=0; i<n; i++){
    	cout<<"Introduzca el PESO del objeto ["<<i+1<<"] : "<<endl;
    	cin>>P[i];
        cout<<"Introduzca el VALOR del objeto["<<i+1<<"] : "<<endl;
    	cin>>V[i];
    }

    for (i = 0; i < n; i++){
        Xoptimo[i] = NULL;
        X[i] = NULL;
    }

    Mochila(n,M,P,V,X,k,Xoptimo,Voptimo);

    cout <<"\n\nObjetos a introducir: ";
    cout <<"[ ";
    for (i = 0; i < n; i++){
        cout<<Xoptimo[i]<<" ";
    }
    cout <<"]"<<endl;

    cout <<"Valor maximo que se puede obtener: "<<Voptimo<<endl;

    cout <<"\n\nFIN...";
    return 0;
}