//Unai Coves Seva, Alejandro Company García y Javier Maciá Sempere
#include <iostream>
#include <stdlib.h>
#include <stack>

using namespace std;

int p42(int x, int y){
    int x1 = x;
    int y1 = y;
    stack <int> pila;
    int s;
    
    while (x1>4){
        y1 = y1/3;
        pila.push(y1);

        x1 = x1 - 4;
        pila.push(x1);
        
    }

    s = x1+y1;
    
    while (!pila.empty()){
        
        x1 = pila.top();
        pila.pop();
        
        y1 = pila.top();
        pila.pop();
        s = s + x1*y1;
        
    }

    return s;
}

int main(){
    int x;
    int y;

    cout <<"Introduce x: ";
    cin >> x;

    cout << "Introduce y: ";
    cin >> y;
  
    cout <<"RESULTADO: "<< p42(x,y)<<endl;   

    return 0;
}