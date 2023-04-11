#include <iostream>
#include "ArbolPasajeros.hpp"

#ifndef __MENU_HPP__
#define __MENU_HPP__

#define MAX_OPCIONES 8
#define MAX_ARBOLES 4

#define KEY_INTRODUCE_PASAJERO 1
#define KEY_SACA_PASAJERO 2
#define KEY_IMPRIMIR_ARBOL 3
#define KEY_CONSULTAR_PASAJERO 4
#define KEY_GENERAR_PASAJEROS 5
#define KEY_BORRAR_PASAJEROS 6
#define KEY_SIMULACION 7
#define KEY_SALIR 8


void muestraMenuPrincipal();
void muestraArbolesGraficamente(ArbolPasajeros* a);
void recorreArbol(ArbolPasajeros a, int modo);
int pideOpcionMenuPrincipal();
Pasaporte pidePasaporte();
Pasajero pideDatosPasajero();
int pideModoRetiradaPasajero();
int pideModoImprimirArbol();
int pideArbol();
int pideNumeroPasajeros();
bool seRepitePasaporte(Pasaporte p, ArbolPasajeros *a);

#endif // __MENU_HPP__
