#include <iostream>
#include <time.h>
#include "ColaPasajeros.hpp"

using namespace std;

pColaPasajeros CreateQueue(){
    pColaPasajeros co = new struct colaPasajeros;
    co->primero = new struct nodoPasajero;
    co->ultimo = new struct nodoPasajero;

    co->ultimo = NULL;
    co->primero->sig = NULL;
    return (co);
}

bool EmptyQueue(pColaPasajeros co){
    return (co->primero->sig == NULL);
}

void ViewQueue(pColaPasajeros co){
    struct nodoPasajero *nodo_aux;
    nodo_aux = new struct nodoPasajero;
    nodo_aux = co->primero->sig;
    cout<<"_______________________"<<endl;
    if (EmptyQueue(co)){
        cout<<"La cola esta vacia"<<endl;
    }
    while (nodo_aux!=NULL){
        muestraPasajero(nodo_aux->pasajero);
        nodo_aux = nodo_aux->sig;
        cout << endl;
    }
    cout<<"_______________________"<<endl;

}

void InsertQueue (pColaPasajeros co, Pasajero p){
    struct nodoPasajero *nodo_aux;
    nodo_aux = new struct nodoPasajero;
    nodo_aux->pasajero = p;

    if (EmptyQueue(co)){
        nodo_aux->sig = NULL;
        co->primero->sig = nodo_aux;
        co->ultimo = nodo_aux;
    }else{
        co->ultimo->sig = nodo_aux;
        co->ultimo = nodo_aux;
        co->ultimo->sig = NULL;
    }
}

bool OutQueue(pColaPasajeros co){
    struct nodoPasajero *nodo_aux;
    nodo_aux = new struct nodoPasajero;
    Pasajero p;

    if(EmptyQueue(co) == true){
        return false;
    }else{
        nodo_aux = co->primero->sig;
        co->primero->sig = nodo_aux->sig; //primero->sig->sig;
        p = nodo_aux->pasajero;
        delete(nodo_aux);
        return true;
    }
}

Pasajero FirstQueue(pColaPasajeros co){
    return co->primero->sig->pasajero;
}

Pasajero LastQueue(pColaPasajeros co){
    return co->ultimo->pasajero;
}

int LengthQueue(pColaPasajeros co){
    int contador = 0;
    struct nodoPasajero *nodo_aux;
    nodo_aux = new struct nodoPasajero;
    nodo_aux =co->primero->sig;

    while(nodo_aux != NULL){
        nodo_aux = nodo_aux->sig;
        contador++;
    }

    return contador;
}

void InsertRandQueue(pColaPasajeros co, int n){
    srand(time(NULL));
    for( int i = 1; i<=n; i++){
        InsertQueue(co, crearPasajeroAleatorio(rand()));
    }
}

void InsertRandQueue(pColaPasajeros co, int n, int tipo){
    srand(time(NULL));
    for( int i = 1; i<=n; i++){
        InsertQueue(co, crearPasajeroAleatorio(rand(), tipo));
    }
}