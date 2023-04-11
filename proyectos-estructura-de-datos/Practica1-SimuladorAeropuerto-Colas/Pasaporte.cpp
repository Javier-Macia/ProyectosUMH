#include <iostream>
#include <string>
#include <time.h>
#include <random>
#include <cstring>

#include "Pasaporte.hpp"

using namespace std;

//Getters

char* getPrefijo( Pasaporte pas){
    char* p = pas.prefijo;
    return p;
}

int* getDigitos( Pasaporte pas){
    int* p = pas.digitos;
    return p;
}

char* getSufijo( Pasaporte pas){
    char* p = pas.sufijo;
    return p;
}



// Métodos
string muestraPasaporte( Pasaporte pas){
    string s_pre = pas.prefijo;
    string s_suf = pas.sufijo;
    string s_digitos = "";
    for (int i = 0; i < NDIGITOS; i++){
        s_digitos += to_string(getDigitos(pas)[i]);
    }
    return s_pre + s_digitos + s_suf;
}

// Valida pasaporte
bool esPasaporteValido( Pasaporte pas){
    bool valido = true;
    char* prefijo = getPrefijo(pas);
    char* sufijo = getSufijo(pas);
    int* digitos = getDigitos(pas);
    if (strlen(prefijo) != 1){
        valido = false;
    }
    if (strlen(sufijo) != 1){
        valido = false;
    }
    for (int i = 0; i < NDIGITOS; i++){
        if (digitos[i] < 0 || digitos[i] > 9){
            valido = false;
        }
    }
    return valido;
}

bool esPasaporteValido(string s_pasaporte){
    char alfabeto[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char digitos[] = "0123456789";
    if(s_pasaporte.length() != NPREFIJO+NDIGITOS+NSUFIJO)
        return false;

    unsigned int i = 0, j = 0;

    int correcto = 0;
    for ( i = 0; i < NPREFIJO; i++){
        for ( j = 0; j < sizeof(alfabeto)/sizeof(alfabeto[0]); j++){
            if (alfabeto[j] == s_pasaporte[i]){
                correcto++;
            }
        }
    }
    if (correcto != NPREFIJO){
        return false;
    }

    correcto = 0;
    for ( i = 0; i < NDIGITOS; i++){
        for ( j = 0; j < sizeof(digitos)/sizeof(digitos[0]); j++){
            if (digitos[j] == s_pasaporte[i+NPREFIJO]){
                correcto++;
            }
        }
    }

    if (correcto != NDIGITOS){
        return false;
    }

    correcto = 0;
    for ( i = 0; i < NSUFIJO; i++){
        for ( j = 0; j < sizeof(alfabeto)/sizeof(alfabeto[0]); j++){
            if (alfabeto[j] == s_pasaporte[i+NPREFIJO+NDIGITOS]){
                correcto++;
            }
        }
    }

    if (correcto != NSUFIJO){
        return false;
    }
    return true;
}

Pasaporte crearPasaporteAleatorio(const int seed){
    char alfabeto[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* p = (char*)malloc(sizeof(char)*3);
    int dig[NDIGITOS];
    char* s = (char*)malloc(sizeof(char)*3);

    int i = 0;
    srand(seed);

    for ( i = 0; i < NDIGITOS; i++){
        dig[i] = (rand()%10);
    }
    for ( i = 0; i < 2; i++){
        p[i] = alfabeto[(rand()%26)];
        s[i] = alfabeto[rand()%26];
    }
    p[2] += '\0';
    s[2] += '\0';


    return crearPasaporte(p,dig,s);
}

//Operadores
bool operator == (const Pasaporte& p1, const Pasaporte& p2){
    if (strcmp(p1.prefijo,p2.prefijo) == 0 && strcmp(p1.sufijo,p2.sufijo) == 0){
        for (int i = 0; i < NDIGITOS; i++){
            if (p1.digitos[i] != p2.digitos[i]){
                return false;
            }
        }
        return true;
    }
    return false;
}

// Constructores
Pasaporte crearPasaporte(){
    Pasaporte p;
    int i;
    for ( i = 0; i < NPREFIJO; i++){
        p.prefijo[i] = 'A';
    }
    p.prefijo[i] = '\0';

    for ( i = 0; i < NDIGITOS; i++){
        p.digitos[i] = i+1;
    }

    for ( i = 0; i < NSUFIJO; i++){
        p.sufijo[i] = 'Z';
    }
    p.sufijo[i] = '\0';

    return p;
}

Pasaporte crearPasaporte( char* pre, int* dig, char* suf ){
    int i = 0;
    Pasaporte p;
    for ( i = 0; i < NPREFIJO; i++){
        p.prefijo[i] = pre[i];
    }
    p.prefijo[i] = '\0';

    for ( int i = 0; i < NDIGITOS; i++){
        p.digitos[i] = dig[i];
    }

    for ( i = 0; i < NSUFIJO; i++){
        p.sufijo[i] = suf[i];
    }
    p.sufijo[i] = '\0';
    return p;
}

Pasaporte crearPasaporte(string s_pasaporte){
    if (!esPasaporteValido(s_pasaporte)){
        cout << "Error: el pasaporte no tiene el formato correcto" << endl;
        return crearPasaporte();
    }
    Pasaporte p;
    int i;
    char c;

    for ( i = 0; i < NPREFIJO; i++){
        p.prefijo[i] = s_pasaporte[i];
    }
    p.prefijo[i] = '\0';

    for ( i = 0; i < NDIGITOS; i++){
        c = s_pasaporte[i+NPREFIJO];
        p.digitos[i] = c - '0';
    }

    for ( i = 0; i < NSUFIJO; i++){
        p.sufijo[i] = s_pasaporte[i+NPREFIJO+NDIGITOS];
    }
    p.sufijo[i] = '\0';

    return p;
}

