#include <iostream>
#include "ColaPasajeros.hpp"

#ifndef __MENU_HPP__
#define __MENU_HPP__

#define MAX_OPCIONES 6
#define MAX_COLAS 4
#define KEY_INTRODUCE_PASAJERO 1
#define KEY_SACA_PASAJERO 2
#define KEY_IMPRIMIR_COLA 3
#define KEY_CONSULTAR_PASAJERO 4
#define KEY_SIMULACION 5
#define KEY_SALIR 6

void muestraMenuPrincipal();
void muestraColasGraficamente(pColaPasajeros* co);
int pideOpcionMenuPrincipal();
Pasajero pideDatosPasajero();
int pideCola();
bool seRepitePasaporte(Pasaporte p, pColaPasajeros *co);

#endif // __MENU_HPP__
