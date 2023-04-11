#include <iostream>
#include "Pasajero.hpp"

#ifndef __COLAPASAJEROS_HPP__

using namespace std;

typedef struct nodoPasajero{
    Pasajero pasajero;
    struct nodoPasajero *sig;
} *pNodoPasajero;

typedef struct colaPasajeros{
    struct nodoPasajero *primero;
    struct nodoPasajero *ultimo;
}*pColaPasajeros;

pColaPasajeros CreateQueue();
bool EmptyQueue(pColaPasajeros co);
void ViewQueue(pColaPasajeros co);
void InsertQueue(pColaPasajeros co, Pasajero p);
bool OutQueue(pColaPasajeros co);
Pasajero FirstQueue(pColaPasajeros co);
Pasajero LastQueue(pColaPasajeros co);
int LengthQueue(pColaPasajeros co);
void InsertRandQueue(pColaPasajeros co, int n);
void InsertRandQueue(pColaPasajeros co, int n, int tipo);


#endif // __COLAPASAJEROS_HPP__
