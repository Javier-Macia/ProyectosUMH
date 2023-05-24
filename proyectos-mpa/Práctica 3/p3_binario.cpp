//Unai Coves Seva, Alejandro Company García y Javier Maciá Sempere

#include<iostream>
#include <stdlib.h>
using namespace std;
 
void binario(int n){ // La función "binario" se repetirá siempre y cuando n>1
    if(n>1){
        binario(n/2);
    }
    cout<<n%2;  // Se imprime el resto
}

int main(){
    int decim;
    cout<<" INGRESE NUMERO: ";
    cin>>decim;

    if(decim>=0){    // Si el número es menor que 0, se devuelve un error
        cout<<"\n Numero:"<<decim<<endl;
        cout<<"\n Binario:";
        binario(decim); // Se llama a la función "binario"
    }else{
        cout <<"-1";
    }
    
    
    return 0;
}