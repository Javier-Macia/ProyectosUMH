#include <string>

#ifndef __PASAPORTE_HPP__
#define __PASAPORTE_HPP__

#define NDIGITOS 5
#define NPREFIJO 2
#define NSUFIJO 2

using namespace std;

//Declaracion del TAD
typedef struct Pasaporte{
    char prefijo[NPREFIJO+1];
    int digitos[NDIGITOS];
    char sufijo[NSUFIJO+1];
}Pasaporte;

//Getters
char* getPrefijo( Pasaporte pas);
int* getDigitos( Pasaporte pas);
char* getSufijo( Pasaporte pas);

//Metodos
string muestraPasaporte(const Pasaporte pas);
bool esPasaporteValido(Pasaporte pas);
bool esPasaporteValido(string s_pasaporte);

//Operadores
bool operator > (const Pasaporte& p1, const Pasaporte& p2);
bool operator < (const Pasaporte& p1, const Pasaporte& p2);
bool operator == (const Pasaporte& p1,const Pasaporte& p2);

//Constructores
Pasaporte crearPasaporte();
Pasaporte crearPasaporte( char pre[], int* dig, char suf[] );
Pasaporte crearPasaporte( string s_pasaporte);
Pasaporte crearPasaporteAleatorio(const int seed);



#endif
