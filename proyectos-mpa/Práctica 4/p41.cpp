//Unai Coves Seva, Alejandro Company García y Javier Maciá Sempere
#include <iostream>
#include <stdlib.h>
#include <stack>

using namespace std;

//ACTIVIDAD 1 PILA
float p41(float x , unsigned int n){
    float r;
    float x1 = x;
    unsigned int n1 = n;

    stack <double> pila;

    while(n1>0){
        pila.push(n1);

        if(!n1%2==0){
            n1=n1-1;
        }

        else{
            n1=n1/2;
        }
    }

    r=1;
    while(! pila.empty()){
        n1= pila.top();
        pila.pop();

        if(!n1%2==0){
            r=x1*r;
        }
        else{
            r=n1;
            r=r*r;
        }
    }
    return r;
}


int main(){
    float x;
    int n;

    cout << "Introduce un base a : ";
    cin >> x;
    cout << "Introduce una exp b : ";
    cin >> n;

    if (n<0){
        cout <<"ERROR";
    }else{
        cout << "RESULTADO: "<<  p41(x,n)  << endl;
    }
}

