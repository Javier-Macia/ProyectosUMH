//Unai Coves Seva, Alejandro Company García y Javier Maciá Sempere
#include <iostream>
#include <stdlib.h>

using namespace std;

double maximo(double a, double b){
    double auxiliar;
    if (a > b){
        auxiliar = a;
    }else{
        auxiliar = b;
    }
    return auxiliar;
}


double Mochila(int n, int M, int *O, int *P, double *V){
    double T[n+1][M+1];
    int i,j;
    int contador = 1;
    int aux;


    cout<<"\t\t";
    for (i = 0; i <= M; i++){
        T[0][i] = 0;
        cout<<T[0][i]<<"\t";
    }
    cout <<"\n\n";

    for (i = 0; i <= n; i++){
        T[i][0] = 0;
    }
    
    for (i = 1; i <= n; i++){
        cout <<"OBJETO "<<contador<<"\t";
        for (j = 0; j <= M; j++){
            if (P[i]>j){
                T[i][j] = T[i-1][j];
                cout<<T[i][j]<<"\t";
            }else{
                T[i][j] = maximo(T[i-1][j], V[i] + T[i-1][j-P[i]]);
                cout<<T[i][j]<<"\t";
            }
        }
        cout <<"   p = "<<P[i]<<" , v = "<<V[i];
        cout<<"\n\n";
        contador++;
    }

    aux = M;
    for (i = n; i > 0; i--){
        if ((T[i][aux]) != (T[i-1][aux])){
            cout<<"El OBJETO "<<i<<" se ha introducido."<<endl;
            aux = aux-P[i];
        }else{
            cout<<"El OBJETO "<<i<<" NO se ha introducido."<<endl;
        }
    }

    cout<<"Tu valor maximo es: ";
    return T[n][M];
}

int main(){
    unsigned int n;   //n se pedirá al usuario y es la cantidad de objetos
    unsigned int M;   // M se pedirá al usuario y es el peso máximo que soporta la mochila
    int i,j;          //iteradores

    cout<<"Introduzca la cantidad de objetos: "<<endl;
    cin>>n;
    
    cout<<"Introduzca el peso maximo que soporta la mochila: "<<endl;
    cin>>M;
    if ((M>0)&&(n>=0)){
        double V[n+1];  // Valor del objeto
        int P[n+1];     // Peso del objeto
        int O[n];       // El objeto en si

        P[0]=0;
        V[0]=0;	

        for(i=1; i<=n; i++){
            cout<<"Introduzca el PESO del objeto ["<<i<<"] : "<<endl;
            cin>>P[i];
            cout<<"Introduzca el VALOR del objeto["<<i<<"] : "<<endl;
            cin>>V[i];
        }
        cout<<"LA MATRIZ ES: \n"<<Mochila(n,M,O,P,V)<<endl;

    }else{
        cout<<"ERROR, M debe ser mayor que 0"<<endl;
    }

    cout <<"\n\nFIN...";
    return 0;
}