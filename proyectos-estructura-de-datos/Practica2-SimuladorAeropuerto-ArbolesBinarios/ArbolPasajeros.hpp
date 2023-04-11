#include <iostream>

#include "Pasajero.hpp"

#ifndef __ARBOLPASAJEROS_HPP__
#define __ARBOLPASAJEROS_HPP__

using namespace std;

typedef struct nodoArbol {
    Pasajero dato;
    struct nodoArbol *izq;
    struct nodoArbol *der;
    struct nodoArbol *padre;
}*ArbolPasajeros;

ArbolPasajeros CrearArbol();
void BorrarArbol(ArbolPasajeros &a);
bool ArbolVacio(ArbolPasajeros a);
int GradoArbol(ArbolPasajeros a);
int AlturaArbol(ArbolPasajeros a);
bool InsertarValorArbol(ArbolPasajeros &a, Pasajero dato);
ArbolPasajeros ApuntarValorArbol(ArbolPasajeros a, Pasajero dato);
ArbolPasajeros ApuntarValorArbol(ArbolPasajeros a, Pasaporte dato);
bool BusquedaValorArbol(ArbolPasajeros a, Pasajero dato);
void RecorridoAnchuraArbol(ArbolPasajeros a);
void RecorridoEntreOrdenArbolSimple(ArbolPasajeros a);
void PreOrdenArbol(ArbolPasajeros a);
void EntreOrdenArbol(ArbolPasajeros a);
void PostOrdenArbol(ArbolPasajeros a);
bool BorrarValorArbol(ArbolPasajeros& a, Pasajero dato);
void BorrarValorSubArbol(ArbolPasajeros &a, Pasajero dato);
bool ArbolesIguales(ArbolPasajeros a1, ArbolPasajeros a2);
void InsertarPasajeroAleatorioArbol(ArbolPasajeros &a, int seed, int tipo);
void ImprimirArbol(ArbolPasajeros a);
ArbolPasajeros ApuntaPasajeroAleatorio(ArbolPasajeros &a, int seed);

#endif // __ARBOLPASAJEROS_HPP__