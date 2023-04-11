#include <iostream>
#include <algorithm>
#include <string>

#include "Menu.hpp"
#include "Pasajero.hpp"
#include "Pasaporte.hpp"


using namespace std;

// Funciones privadas
bool esUnaRespuestaSioNo(string palabra);
bool esSi(string palabra);
bool esOpcionValida(int opcion);
string toLower(string s);
bool esColaValida(int nArbol);
bool esRetiradaPasajeroValida(int nRetirada);
bool esImprimirArbolValido(int nImprimir);
bool esNumeroPasajerosValido(int nPasajeros);

// Funciones publicas
void muestraMenuPrincipal(){
    cout << "----------------------------------------------------------" << endl;
    cout << "                       MENU PRINCIPAL" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "1. Introducir un pasajero en cualquier arbol: 1, 2, 3, 4 " << endl;
    cout << "2. Sacar un pasajero del arbol: 1, 2, 3, 4 " << endl;
    cout << "3. Imprimir cualquier arbol: 1, 2, 3, 4 " << endl;
    cout << "4. Buscar pasajero en un arbol: 1, 2, 3, 4 " << endl;
    cout << "5. Generar 'n' pasajeros aleatorios. " << endl;
    cout << "6. Borrar 'n' pasajeros aleatorios." << endl;
    cout << "7. Iniciar la simulacion." << endl;
    cout << "8. Salir de la aplicacion." << endl;
    cout << "----------------------------------------------------------" << endl;
}

void muestraArbolesGraficamente(ArbolPasajeros* a){
    cout << "----------------------------------------------------------" << endl;
    cout << "                       ARBOLES" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "1. Arbol UE:  |" ;
    if (!ArbolVacio(a[0])){
        RecorridoEntreOrdenArbolSimple(a[0]);
    }
    cout << endl;

    cout << "2. Arbol BR:  |";
    if (!ArbolVacio(a[1])){
        RecorridoEntreOrdenArbolSimple(a[1]);
    }
    cout << endl;

    cout << "3. Arbol AUE: |";
    if (!ArbolVacio(a[2])){
        RecorridoEntreOrdenArbolSimple(a[2]);
    }
    cout << endl;

    cout << "4. Arbol EC:  |";
    if (!ArbolVacio(a[3])){
        RecorridoEntreOrdenArbolSimple(a[3]);
    }
    cout << endl;

    cout << "----------------------------------------------------------" << endl;
}

void recorreArbol(ArbolPasajeros a, int modo){
    if (!ArbolVacio(a)){
        switch (modo){
        case 1:
            RecorridoAnchuraArbol(a);
            break;
        case 2:
            PreOrdenArbol(a);
            break;
        case 3:
            EntreOrdenArbol(a);
            break;
        case 4:
            PostOrdenArbol(a);
            break;

        default:
            cout << "Modo de recorrido no valido." << endl;
            break;
        }
    }else{
        cout << "El arbol esta vacio." << endl;
    }
    
}

/**
 * @brief Lee por teclado una opcion del menu principal. De no ser valida, devuelve -1.
 *
 * @return int
 */
int pideOpcionMenuPrincipal(){
    string opcion;
    int opcionInt;
    cout << "Introduzca una opcion: ";
    cin >> opcion;
    opcionInt = atoi(opcion.c_str());
    if (esOpcionValida(opcionInt)){
        return opcionInt;
    }
    return -1;
}

Pasaporte pidePasaporte(){
    string str_pasaporte = "";

    cout << "Introduzca el pasaporte del pasajero: ";
    cin >> str_pasaporte;

    while (!esPasaporteValido(str_pasaporte)){
        cout << "No ha introducido un pasaporte valido (El formato debe ser: LLDDDDDLL). Intentelo de nuevo: ";
        cin >> str_pasaporte;
    }
    return crearPasaporte(str_pasaporte);
}

/**
 * @brief Pide por teclado los datos de un pasajero.
 *
 * @return Pasajero
 */
Pasajero pideDatosPasajero(){
    string nombre = "", str_pasaporte = "", nacionalidad = "", str_estado = "";
    Pasaporte pasaporte;
    cout << "Introduzca el nombre del pasajero: ";
    cin.ignore();
    getline(cin, nombre);

    cout << "Introduzca el pasaporte del pasajero: ";
    cin >> str_pasaporte;

    while (!esPasaporteValido(str_pasaporte)){
        cout << "No ha introducido un pasaporte valido (El formato debe ser: LLDDDDDLL). Intentelo de nuevo: ";
        cin >> str_pasaporte;
    }
    pasaporte = crearPasaporte(str_pasaporte);


    cout << "Introduzca el pais del pasajero: ";
    cin.ignore();
    getline(cin, nacionalidad);

    cout << "El pasaporte esta vigente? (si/no): ";
    cin >> str_estado;

    while (!esUnaRespuestaSioNo(str_estado)){
        cout << "No te hemos entendido, el pasaporte esta vigente? (si/no): ";
        cin >> str_estado;
    }
    pasaporte = crearPasaporte(str_pasaporte);

    return crearPasajero(pasaporte, nombre, nacionalidad, esSi(str_estado));
}


/**
 * @brief Funcion que, dado un pasaporte y un arbol de pasajeros, dice si este pasaporte aparece en ella o no.
 *
 * @param pas
 * @param co
 * @return true Se repite el pasaporte en la cola.
 * @return false
 */
bool seRepitePasaporte(Pasaporte pas, ArbolPasajeros *a){
    Pasajero auxiliar = crearPasajero(pas, "", "", false);

    for (int i = 0; i < MAX_ARBOLES; i++){
        if (!ArbolVacio(a[i])){
            if (BusquedaValorArbol(a[i], auxiliar))
                return true;
        }
    }
    return false;
}

/**
 * @brief Pide una cola por teclado. De no ser v�lida, devuelve -1.
 *
 * @return int
 */
int pideArbol(){
    string opcion;
    int opcionInt;
    cout << "Introduzca el arbol: ";
    cin >> opcion;
    opcionInt = atoi(opcion.c_str());
    if (esColaValida(opcionInt)){
        return opcionInt;
    }
    return -1;
}

int pideModoRetiradaPasajero(){
    string opcion;
    int opcionInt;
    cout << "Como quieres retirar un pasajero?: " << endl;
    cout << "1. Aleatoriamente." << endl;
    cout << "2. Por numero de pasaporte." << endl;
    cout << "3. Retirando el nodo raiz." << endl;
    cin >> opcion;
    opcionInt = atoi(opcion.c_str());
    if (esRetiradaPasajeroValida(opcionInt)){
        return opcionInt;
    }
    return -1;
}

int pideModoImprimirArbol(){
    string opcion;
    int opcionInt;
    cout << "Como quieres imprimir el arbol?: " << endl;
    cout << "1. Recorrido anchura." << endl;
    cout << "2. Pre-orden." << endl;
    cout << "3. Entre-orden." << endl;
    cout << "4. Post-orden." << endl;
    cin >> opcion;
    opcionInt = atoi(opcion.c_str());
    if (esImprimirArbolValido(opcionInt)){
        return opcionInt;
    }
    return -1;
}

int pideNumeroPasajeros(){
    string opcion;
    int opcionInt;
    cout << "Introduzca el numero de pasajeros: ";
    cin >> opcion;
    opcionInt = atoi(opcion.c_str());
    if (esNumeroPasajerosValido(opcionInt)){
        return opcionInt;
    }
    return -1;
}


//Funciones privadas

/**
 * @brief Convierte a lowercase una string.
 *
 * @param s
 * @return string
 */
string toLowerCase(string s) {
    transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}


bool esOpcionValida(int opcion){
    return (opcion >= 1 && opcion <= MAX_OPCIONES);
}

bool esColaValida(int nArbol){
    return (nArbol >= 1 && nArbol <= MAX_ARBOLES);
}

bool esRetiradaPasajeroValida(int nRetirada){
    return (nRetirada >= 1 && nRetirada <= 3);
}

bool esImprimirArbolValido(int nImprimir){
    return (nImprimir >= 1 && nImprimir <= 4);
}

/**
 * @brief Devuelve si una string es una respuesta de si o no (o similares: SI, s, S...).
 *
 * @param palabra
 * @return true
 * @return false
 */
bool esUnaRespuestaSioNo(string palabra){
    string cpy;
    cpy = toLowerCase(palabra);
    return (cpy == "si" || cpy == "no" || cpy == "s" || cpy == "n" || cpy == "1" || cpy == "0");
}

/**
 * @brief Devuelve si la palabra es "si" o algo similar
 *
 * @param palabra
 * @return true
 * @return false
 */
bool esSi(string palabra){
    string cpy;
    cpy = toLower(palabra);
    return (cpy == "si" || cpy == "s" || cpy == "1");
}

bool esNumeroPasajerosValido(int nPasajeros){
    return (nPasajeros >= 1);
}