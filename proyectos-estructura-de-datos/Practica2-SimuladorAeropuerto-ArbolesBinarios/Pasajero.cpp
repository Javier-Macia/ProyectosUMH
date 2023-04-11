#include <iostream>
#include <time.h>
#include <cstring>
#include <string>
#include <algorithm>

#include "Pasajero.hpp"

using namespace std;

// Cabecera funciones privadas
string nombreAleatorio(int seed);
string apellidoAleatorio(int seed);
string nombreCompletoAleatorio(int seed);
string paisUEAleatorio(int seed);
string paisAcuerdoUEAleatorio(int seed);
string paisReinoUnidoAleatorio(int seed);
string paisExtracomunitarioAleatorio(int seed);
int tipoAleatorio(int seed);
string paisAleatorio(int seed, int tipo);
bool estadoPasaporteAleatorio(int seed);
int buscarTipo(string pais);
string tipoAString(int tipo);
string estadoAString(bool estado);

// Constantes globales
string paisesUnionEuropea[]{
    "Alemania",
    "Austria",
    "Belgica",
    "Bulgaria",
    "Chipre",
    "Croacia",
    "Dinamarca",
    "Eslovaquia",
    "Eslovenia",
    "Espana",
    "Estonia",
    "Finlandia",
    "Francia",
    "Grecia",
    "Hungria",
    "Irlanda",
    "Italia",
    "Letonia",
    "Lituania",
    "Luxemburgo",
    "Malta",
    "Paises Bajos",
    "Pa�ses Bajos",
    "Polonia",
    "Portugal",
    "Republica Checa",
    "Rumania",
    "Suecia"
};

string paisesAsociadosUE[]{
    "Noruega",
    "Islandia",
    "Suiza" ,
    "Liechtenstein"
};

string paisesReinoUnido[]{
    "Inglaterra",
    "Escocia",
    "Gales" ,
    "Irlanda del Norte"
};

string paisesEC[]{
    "Afganistan",
    "Albania",
    "Andorra",
    "Angola",
    "Arabia Saudita",
    "Argelia",
    "Argentina",
    "Armenia",
    "Australia",
    "Bahamas",
    "Bielorrusia",
    "Bolivia",
    "Brasil",
    "Bulgaria",
    "Burundi",
    "Chile",
    "China",
    "Colombia",
    "Cuba",
    "Ecuador",
    "Egipto",
    "Etiopia",
    "Filipinas",
    "Japon",
    "Estados Unidos de America",
    "Madagascar",
    "Marruecos",
    "Moldavia",
    "Mongolia",
    "Nepal",
    "Nicaragua",
    "Nigeria",
    "Panama",
};


// Getters
Pasaporte getPasaporte(Pasajero p){
    return p.pasaporte;
}

string getNombre(Pasajero p){
    return p.nombre;
}

string getNacionalidad(Pasajero p){
    return p.nacionalidad;
}

int getTipo(Pasajero p){
    return p.tipo;
}

bool getEstadoPasaporte(Pasajero p){
    return p.estadoPasaporte;
}


// Funciones publicas
void muestraPasajero(Pasajero p){
    cout << "Pasajero: " << getNombre(p) << endl;
    cout << "Pasaporte: " << muestraPasaporte(getPasaporte(p)) << endl;
    cout << "Nacionalidad: " << getNacionalidad(p) << endl;
    cout << "Tipo: " << tipoAString(getTipo(p)) << endl;
    cout << "Estado pasaporte: " << estadoAString(getEstadoPasaporte(p)) << endl;
}

// Constructores
Pasajero crearPasajero(){
    Pasajero JMS;
    JMS.pasaporte = crearPasaporte();
    JMS.nombre = "Javier Macia Sempere";
    JMS.nacionalidad = "Espa\xA4\x0061";
    JMS.tipo = UE;
    JMS.estadoPasaporte = true;
    return JMS;
}

Pasajero crearPasajeroAleatorio(int seed){
    srand(seed);
    Pasajero pas;
    pas.pasaporte = crearPasaporteAleatorio(rand());
    pas.nombre = nombreCompletoAleatorio(rand());
    pas.tipo = tipoAleatorio(rand());
    pas.nacionalidad = paisAleatorio(rand(), pas.tipo);
    pas.estadoPasaporte = estadoPasaporteAleatorio(rand());
    return pas;
}

Pasajero crearPasajeroAleatorio(int seed, int tipo){
    srand(seed);
    Pasajero pas;
    pas.pasaporte = crearPasaporteAleatorio(rand());
    pas.nombre = nombreCompletoAleatorio(rand());
    pas.tipo = tipo;
    pas.nacionalidad = paisAleatorio(rand(), pas.tipo);
    pas.estadoPasaporte = estadoPasaporteAleatorio(rand());
    return pas;
}

Pasajero crearPasajero(Pasaporte pasaporte, string nombre, string nacionalidad, bool estadoPasaporte){
    Pasajero pas;
    pas.pasaporte = pasaporte;
    pas.nombre = nombre;
    if (nacionalidad == "Espa\xA4\x0061" || nacionalidad == "espa\xA4\x0061"){
        nacionalidad = "Espana";
    }
    pas.tipo = buscarTipo(nacionalidad);
    pas.nacionalidad = nacionalidad;
    pas.estadoPasaporte = estadoPasaporte;
    return pas;
}

// Operadores
bool operator > (Pasajero& p1, Pasajero& p2){
    return p1.pasaporte > p2.pasaporte; 
}

bool operator < (Pasajero& p1, Pasajero& p2){
    return p1.pasaporte < p2.pasaporte; 
}

bool operator == (Pasajero& p1, Pasajero& p2){
    return p1.pasaporte == p2.pasaporte; 
}




// Funciones privadas

/**
 * @brief Convierte a lowercase una string.
 *
 * @param s
 * @return string
 */
string toLower(std::string s) {
    transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

string nombreAleatorio(int seed){
    srand(seed);
    string nombresPosibles[] = {
        "Cronos",
        "Gea",
        "Zeus",
        "Hera",
        "Atenea",
        "Artemisa",
        "Apolo",
        "Hermes",
        "Ares",
        "Hefesto",
        "Afrodita",
        "Eros",
        "Poseidon",
        "Hestia",
        "Demeter",
        "Dionisio",
        "Asclepios",
        "Hades",
        "Persefone",
        "Heracles",
        "Odiseo"
    };
    int size = sizeof(nombresPosibles)/sizeof(nombresPosibles[0]);
    return nombresPosibles[rand()%size];
}

string apellidoAleatorio(int seed){
    srand(seed);
    string apellidosPosibles[] = {
        "Garcia",
        "Rodriguez",
        "Gonzalez",
        "Fernandez",
        "Lopez",
        "Martinez",
        "Sanchez",
        "Perez",
        "Gomez",
        "Martin",
        "Jimenez",
        "Hernandez",
        "Ruiz",
        "Diaz",
        "Moreno",
        "Mendez",
        "Alvarez",
        "Cruz",
        "Ramirez",
        "Macia",
        "Sempere"
    };
    int size = sizeof(apellidosPosibles)/sizeof(apellidosPosibles[0]);
    return apellidosPosibles[rand()%size];
}

string nombreCompletoAleatorio(int seed){
    string s = nombreAleatorio(seed) + " " + apellidoAleatorio(seed + 5);
    return s;
}

string paisUEAleatorio(int seed){
    srand(seed);
    int size = sizeof(paisesUnionEuropea)/sizeof(paisesUnionEuropea[0]);
    return paisesUnionEuropea[rand()%size];
}
string paisAsociadoUEAleatorio(int seed){
    srand(seed);
    int size = sizeof(paisesAsociadosUE)/sizeof(paisesAsociadosUE[0]);
    return paisesAsociadosUE[rand()%size];
}

string paisReinoUnidoAleatorio(int seed){
    srand(seed);
    int size = sizeof(paisesReinoUnido)/sizeof(paisesReinoUnido[0]);
    return paisesReinoUnido[rand()%size];
}

string paisExtracomunitarioAleatorio(int seed){
    srand(seed);
    int size = sizeof(paisesEC)/sizeof(paisesEC[0]);
    return paisesEC[rand()%size];
}

int tipoAleatorio(int seed){
    srand(seed);
    return rand()%4;
}

string paisAleatorio(int seed, int tipo){
    srand(seed);
    switch (tipo){
    case UE:
        return paisUEAleatorio(seed);
        break;
    case BR:
        return paisReinoUnidoAleatorio(seed);
        break;
    case AUE:
        return paisAsociadoUEAleatorio(seed);
        break;
    case EC:
        return paisExtracomunitarioAleatorio(seed);
        break;
    default:
        return paisExtracomunitarioAleatorio(seed);
        break;
    }
}

bool estadoPasaporteAleatorio(int seed){
    srand(seed);
    return rand()%2;
}

string tipoAString(int tipo){
    switch (tipo){
    case UE:
        return "Union Europea";
        break;
    case BR:
        return "Reino Unido";
        break;
    case AUE:
        return "Asociado UE";
        break;
    case EC:
        return "Extracomunitario";
        break;
    default:
        return "Extracomunitario";
        break;
    }
}

string estadoAString(bool estado){
    if (estado) return "Vigente";
    return "No vigente";
}

int buscarTipo(string pais){
    unsigned int i;

    for (i = 0; i < sizeof(paisesUnionEuropea)/sizeof(paisesUnionEuropea[0]); i++){
        if ( (toLower(pais) == toLower(paisesUnionEuropea[i])) ){
            return UE;
        }
    }
    for (i = 0; i < sizeof(paisesAsociadosUE)/sizeof(paisesAsociadosUE[0]); i++){
        if ( (toLower(pais) == toLower(paisesAsociadosUE[i])) ){
            return AUE;
        }
    }
    for (i = 0; i < sizeof(paisesReinoUnido)/sizeof(paisesReinoUnido[0]); i++){
        if ( (toLower(pais) == toLower(paisesReinoUnido[i])) ){
            return BR;
        }
    }
    return EC;
}

