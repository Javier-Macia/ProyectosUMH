//Javier Maciá Sempere, Alejandro Company García y Unai Coves Seva

#include<iostream>

using namespace std;
int combinaciones(int n, int k) {
   if(k==0 || k==n) {
    return 1;
   }
   else{
      return (combinaciones(n-1,k-1)+combinaciones(n-1,k));
   }
}

int combinaciones_traza(int n, int k, int &count, int level) {
   count++; //Esta variable servirá para contar el número de combinaciones traza. Aquí sumamos 1 a esa variable
   int i;

   for (i = 0; i < level; i++){  //Este bucle sirve para tabular según el nivel.
      cout << "\t";
   }
   level ++;
   
   cout <<count<<".-combinaciones_traza ("<<n<<","<<k<<")"<<endl;

   if(k==0 || k==n){ //Si el numero de personas y el numero de grupos es el mismo, o no hay grupos, solo hay una combinación posible
      return 1; 
   }
   else{
      return (combinaciones_traza(n-1,k-1,count,level)+combinaciones_traza(n-1,k,count,level));
   }
}

int main(){
   int n, k;
   int count = 0; //Esta variable servirá para contar el número de combinaciones traza.
   int level = 0; //Esta variable servirá para contar el nivel de cada traza.
   cout << "Introduce el numero de personas: ";
   cin >> n;
   cout << "Introduce el numero de grupos de personas: ";
   cin >> k;
   cout <<"\n\n";
   
   cout << "El numero de combinaciones: " << combinaciones(n,k);
   cout << "\n";
   cout << "Combinaciones traza:\n" << combinaciones_traza(n,k,count,level) <<" combinaciones posibles";

   return 0;
}