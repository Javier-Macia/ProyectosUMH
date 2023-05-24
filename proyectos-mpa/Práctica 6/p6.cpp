#include <iostream>
#include <stdlib.h>

using namespace std;
void Intercambiar(int &x,int &y){
    int temp;
    temp = x;
    x = y;
    y = temp;
}


void OrdenarDecreciente_VP(int *O,double *V,double *P){
    int i,j;
    double aux;
    double Cociente[sizeof(O)];
    for (i = 0; i < sizeof (O); i++){
        Cociente[i] = V[i]/P[i];
        O[i] = Cociente[i];
    }
    
    for (i=0; i<sizeof (O)-1; i++){
        for (j=i+1; j < sizeof (O); j++){
            if(O[i]>O[j]){
                Intercambiar(O[i],O[j]);
            }
        }
    }
}

double *Mochila(int n, double M, int *O, double *P, double *V){
    double X[1][n] = {0,1};  // Variable de decisión. ¿Se coge o no el objeto n?
    double peso;
    int i;

    for (i = 1; i <= n; i++){
        X[0][i] = 0;    //Declaramos la primera fila como todo ceros.
    }
    
    OrdenarDecreciente_VP (O,V,P);

    i = 1;
    peso = 0;

    while ((peso<M)&&(i<=n)){
        if ((peso + P[i]) <= M){
            X[i][n] = 1;
            peso = peso + P[i];
        }else{
            X[i][n] = (M-peso)/P[i];
            peso = M;
        }
        i++;
    }
    
    return *X;
}

int main(){
    int n;    //n se pedirá al usuario y es la cantidad de objetos
    double M;  // M se pedirá al usuario y es el peso máximo que soporta la mochila
    int i,j;     //iteradores

    double *Matriz;

    cout<<"Introduzca la cantidad de objetos: "<<endl;
    cin>>n;
    
    cout<<"Introduzca el peso máximo que soporta la mochila: "<<endl;
    cin>>M;

    double V[n+1]; // Valor del objeto
    double P[n+1]; // Peso del objeto
    int O[n];   // El objeto en si

    P[0]=0;
    V[0]=0;	

    for(i=1; i<=n; i++){
    	cout<<"Introduzca el PESO del objeto ["<<i<<"] : "<<endl;
    	cin>>P[i];
        cout<<"Introduzca el VALOR del objeto["<<i<<"] : "<<endl;
    	cin>>V[i];
    }

    Matriz = Mochila(n,M,O,P,V);
    
    for (i = 0; i < n; i++){
        Matriz[i];
    }
    
    cout <<"\n\nFIN...";
    return 0;
}