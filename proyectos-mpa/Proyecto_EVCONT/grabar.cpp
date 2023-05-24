//Javier Maciá Sempere
#include <iostream>
#include <stdlib.h>
#include <fstream>  //Librería para los archivos.
#include <vector>
#include <cmath>

using namespace std;

void merge(float arr[], int izq, int med, int der, string progs[]){
    int n1 = med - izq + 1;
    int n2 = der - med;
    float L[n1], R[n2];   // Arrays temporales de ambas mitades
    string Lp[n1], Rp[n2];
    int i = 0;
    int j = 0;
    int k = izq;
 
    // Copia de información
    for (int i = 0; i < n1; i++){
        L[i] = arr[izq + i];
        Lp[i] = progs[izq + i];
    }

    for (int j = 0; j < n2; j++){
        R[j] = arr[med + 1 + j];
        Rp[j] = progs[med + 1 + j];
    }
 
    while ((i < n1) && (j < n2)) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            progs[k] = Lp[i];
            i++;
        }
        else {
            arr[k] = R[j];
            progs[k] = Rp[j];
            j++;
        }
        k++;
    }
 
    // Copia los elementos que puedan faltar en L
    while (i < n1) {
        arr[k] = L[i];
        progs[k] = Lp[i];
        i++;
        k++;
    }
 
    // Copia los elementos que puedan faltar en R
    while (j < n2) {
        arr[k] = R[j];
        progs[k] = Rp[j];
        j++;
        k++;
    }
}
 
void mergeSort(float arr[],int izq,int der,string progs[]){
    int med;
    if(izq>=der){
        return;
    }
    med = izq + (der-izq)/2;
    mergeSort(arr,izq,med, progs);
    mergeSort(arr,med+1,der, progs);
    merge(arr,izq,med,der, progs);
}

void saludo(){
    cout<<"-------------------------"<<endl;
    cout<<"  GRABACION DE PROGRAMAS "<<endl;
    cout<<"-------------------------"<<endl;
}
void opciones(){
    cout<<"1.- Introducir lista de programas"<<endl;
    cout<<"2.- Introducir capacidad de la memoria"<<endl;
    cout<<"3.- Grabar maximo numero de programas"<<endl;
    cout<<"4.- Grabar maximo de capacidad"<<endl;
    cout<<"5.- Salir"<<endl;
    cout<<"-------------------------"<<endl;
}

void redimensionar(float *&vectord, int &longitud){
    longitud = longitud + 5;
    int i;

    float *aux = new float[longitud];

    for(i = 0; i < longitud - 5 ; i++){
        aux[i] = vectord[i];
    }
    delete[] vectord;
    vectord = aux;
}


float CalcularSuma(bool *X, float P[],int k){
    float sol = 0;
    int i;
    for (i = 0; i < k; i++){
        if (X[i]==1){           // Si hay un 1 en ese array, se sumará el valor de P a la solución.
            sol = sol + P[i];
        }
    }
    return sol;
}

void Mochila(int progsAInstalar, float espacioLib, float *numeros,bool X[], int k, bool Xoptimo[], float &Voptimo){
    int i,j;
    float peso, valor;

    if (k<=progsAInstalar){
        for (i = 0; i <= 1; i++){
            X[k] = i;
            peso = CalcularSuma(X,numeros,k);
            if (peso <= espacioLib){
                if (k == progsAInstalar){
                    valor = CalcularSuma(X,numeros,k);
                    if (valor>Voptimo){
                        for (i = 0; i < progsAInstalar; i++){
                            Xoptimo[i] = X[i];
                        }
                        Voptimo = valor;
                        
                        // Parte del código que imprime la traza
                        /** 
                        cout <<"[ ";
                        for (j = 0; j < progsAInstalar; j++){
                            cout<<X[j]<<" ";
                        }
                        cout <<"]";
                        cout<<"   Valor -> "<<valor<<endl;
                        **/
                    }
                }else{
                    Mochila(progsAInstalar,espacioLib,numeros,X,k+1,Xoptimo,Voptimo);
                }
            } 
        }
    }
}


int main(){
    int opcion;
    int i = 0;
    int j;
    int nNums = 0;
    int nProgs = 0;
    int tam = 2;
    float espacioLib = -1;
    int progsAInstalar;
    float peso = 0;
    float sumador = 0;
    int contador = 0;

    //Variables para los archivos:
    string nombreArchivo;
    string programas[256];  // Matriz que guarda el nombre de los programas. Limitado a 256 programas.
    float *numeros= new float[tam]; // Matriz que guarda los pesos de los programas. Es un array dinámico.
    bool archivoActivo = false;
    ifstream archivo;
    string linea;
    //

    bool X[256];       // Si el objeto entra o no. Limitado a 256 programas
    bool Xoptimo[256]; // Objetos que sí son óptimos de introducir. Limitado a 256 programas
    float Voptimo = -1*INFINITY;

    do{
        if (espacioLib>0){
            cout<<"  >> La capacidad disponible es de: "<<espacioLib<<" Gb"<<endl;
        }
        if (archivoActivo == true){
            cout<<"  >> Archivo abierto: "<<nombreArchivo<<endl;
        }
        
        saludo();
        opciones();
        cout<<">> Introduzca una opcion: ";
        cin>>(opcion); 
        
        switch (opcion){
        case 1: //Obtener lista de programas
            cout <<">> Introduce un archivo que quieras abrir: ";
            cin >>(nombreArchivo);
            archivoActivo = false;
            //Apertura del archivo
            archivo.open(nombreArchivo);
            archivo.clear();
            archivo.seekg(0);
            
            i = 0;
            nNums = 0;
            nProgs = 0;
            progsAInstalar = 0;

            if (!archivo.is_open()){
                cout <<"No se ha podido abrir el archivo."<<endl;
            }else{
                archivoActivo = true;
                cout <<"Este es tu texto: "<<endl;
                cout<<"-------------------------"<<endl;

                // Esta parte del código leerá el texto y seleccionará los programas que se van a instalar, los nobmres de los programas, y el peso de los mismos.
                while (archivo.good()){
                    getline(archivo,linea);
                    cout<<linea<<endl;
                    if (i == 0){
                        progsAInstalar = atoi(linea.c_str());
                    }else{
                        if ((i%2) == 0){
                            if (tam<=nNums){
                                redimensionar(numeros,tam);
                            }
                            numeros[nNums] = atof(linea.c_str());
                            nNums++;
                        }else{
                            programas[nProgs] = linea;
                            nProgs++;
                        }
                    }
                    i++;
                }
                cout<<"-------------------------"<<endl;
                cout<<"Hemos detectado lo siguiente:"<<endl<<endl;
                cout<<"El usuario ha indicado que hay "<<progsAInstalar<<" programas. Los cuales son:"<<endl;
                for (i = 0; i < progsAInstalar-1; i++){
                    cout<<programas[i]<<"("<<numeros[i]<<" Gb) , ";
                }
                cout<<programas[i]<<"("<<numeros[i]<<" Gb)"<<endl<<endl;

                archivo.close();
            }
            
            system("pause");
            system("@cls");
            break;


        case 2: //Capacidad disponible de la memoria.
            cout<<">> Introduzca la capacidad disponible de la memoria (en Gb!): ";
            cin>>espacioLib;
            if (espacioLib<=0){
                cout<<"No se ha introducido una capacidad valida. La cantidad debe ser mayor que 0."<<endl;
            }
            system("pause");
            system("@cls");
            break;


        case 3: // Grabar máximo número de programas.
            if (!archivoActivo){
                cout<<"No ha indicado un archivo. Por favor, seleccionela en la OPCION 1 "<<endl<<endl;
            }
            if (espacioLib<=0){
                cout<<"No ha indicado la capacidad disponible de memoria. Por favor, seleccionela en la OPCION 2 "<<endl<<endl;
            }

            if ((espacioLib>0) && (archivoActivo == true)){
                contador = 0;
                sumador = 0;
                cout<<endl;
                cout <<"A continuacion, se aplicara un algoritmo de ORDENACION: "<<endl;
                cout<<"-------------------------"<<endl;
                cout <<"Peso de los programas que se han solicitado en la primera linea: "<<endl;
                for (i = 0; i < progsAInstalar-1; i++){
                    cout<<programas[i]<<"("<<numeros[i]<<" Gb) , ";
                }
                cout<<programas[i]<<"("<<numeros[i]<<" Gb)"<<endl<<endl;

                mergeSort(numeros, 0, progsAInstalar-1, programas);

                cout <<"Peso de tus programas ORDENADOS de menor a mayor: "<<endl;
                for (i = 0; i < progsAInstalar-1; i++){
                    cout<<programas[i]<<"("<<numeros[i]<<" Gb) , ";
                }
                cout<<programas[i]<<"("<<numeros[i]<<" Gb)"<<endl<<endl;


                cout <<"\n\nProgramas que se instalaran: "<<endl;

                for (i = 0; i < progsAInstalar-1; i++){
                    if ((sumador + numeros[i]) <= espacioLib){
                        sumador = sumador + numeros[i];
                        cout<<programas[i]<<" ";
                        contador++;
                    }
                }

                cout<<endl;
                cout<<"Total programas: "<<contador<<endl;
                cout<<"Espacio ocupado: "<<sumador<<" / "<<espacioLib<<"Gb. "<<endl;
            }

            system("pause");
            system("@cls");
            break;


        case 4: // Grabar máximo de capacidad.
            if (!archivoActivo){
                cout<<"No ha indicado un archivo. Por favor, seleccionela en la OPCION 1. "<<endl<<endl;
            }
            if (espacioLib<=0){
                cout<<"No ha indicado la capacidad disponible de memoria. Por favor, seleccionela en la OPCION 2. "<<endl<<endl;
            }

            if ((espacioLib>0) && (archivoActivo == true)){
                contador = 0;
                cout<<endl;
                cout <<"A continuacion, se aplicara un algoritmo de MOCHILA: "<<endl;
                cout<<"-------------------------"<<endl;
                cout <<"Peso de los programas que se han solicitado en la primera linea: "<<endl;
                for (i = 0; i < progsAInstalar-1; i++){
                    cout<<programas[i]<<"("<<numeros[i]<<" Gb) , ";
                }
                cout<<programas[i]<<"("<<numeros[i]<<" Gb)"<<endl<<endl;

                for (i = 0; i < progsAInstalar-1; i++){
                    Xoptimo[i] = NULL;
                    X[i] = NULL;
                }
                Voptimo = -1*INFINITY;

                mergeSort(numeros, 0, progsAInstalar-1,programas);
                Mochila(progsAInstalar-1, espacioLib, numeros, X, 0, Xoptimo, Voptimo);

                

                cout <<"\n\nProgramas que se instalaran: "<<endl;
                for (i = 0; i < progsAInstalar-1; i++){
                    if (Xoptimo[i] == 1){
                        cout<<programas[i]<<"("<<numeros[i]<<" Gb) ";
                        contador++;
                    }
                }
                cout<<endl;

                cout<<"Se pueden almacenar "<<Voptimo<<" Gb /"<<espacioLib<<" Gb. "<<endl;
                cout<<"Total programas: "<<contador<<endl;

            }
            system("pause");
            system("@cls");
            break;


        case 5: // Salir
            system("@cls");
            cout<<"Se ha cerrado el programa. "<<endl<<endl;
            archivo.close();
            return 0;


        default: //Caso de error
            cout<<"Has introducido una opcion no valida. Pulsa un boton para continuar."<<endl;
            system("pause");
            system("@cls");
            break;
        }
    }while (true);
    
    free(numeros);
    archivo.close();
    return 0;
}