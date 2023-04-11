#include <stdio.h>
#include <string>
#include "Pasaporte.hpp"

#ifndef __PASAJERO_HPP__
#define __PASAJERO_HPP__

// Tipos de pasajeros que hay
typedef enum tipoCiudadano{
    UE, // Pertenencientes a la Union Europea
    BR, // Britanicos, ciudadanos del Reino Unido
    AUE, // Ciudadanos con los que la UE tiene acuerdos de transito.
    EC, // Extracomunitarios, resto de nacionalidades
}tipoCiudadano;

// Declaracion del TAD
typedef struct Pasajero{
    Pasaporte pasaporte;
    string nombre;
    string nacionalidad;
    int tipo;
    bool estadoPasaporte;
}Pasajero;

// Getters
Pasaporte getPasaporte(Pasajero p);
string getNombre(Pasajero p);
string getNacionalidad(Pasajero p);
int getTipo(Pasajero p);
bool getEstadoPasaporte(Pasajero p);

// Metodos
void muestraPasajero(Pasajero p);

// Constructores
Pasajero crearPasajero();
Pasajero crearPasajero(Pasaporte pasaporte, string nombre, string nacionalidad, bool estadoPasaporte);
Pasajero crearPasajeroAleatorio(int seed);
Pasajero crearPasajeroAleatorio(int seed, int tipo);

// Operadores
bool operator > (Pasajero& p1, Pasajero& p2);
bool operator < (Pasajero& p1, Pasajero& p2);
bool operator == (Pasajero& p1, Pasajero& p2);

#endif // __PASAJERO_HPP__

