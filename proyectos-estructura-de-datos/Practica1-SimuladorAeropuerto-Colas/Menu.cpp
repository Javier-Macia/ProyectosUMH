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
bool esColaValida(int nCola);


// Funciones públicas
void muestraMenuPrincipal(){
    cout << "----------------------------------------------------------" << endl;
    cout << "                       MENU PRINCIPAL" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "1. Introducir un pasajero en cualquier cola: 1, 2, 3, 4 " << endl;
    cout << "2. Sacar el primer pasajero de la cola: 1, 2, 3, 4 " << endl;
    cout << "3. Imprimir cualquier cola: 1, 2, 3, 4 " << endl;
    cout << "4. Consultar que pasajero esta en la cola: 1, 2, 3, 4 " << endl;
    cout << "5. Iniciar la simulacion." << endl;
    cout << "6. Salir de la aplicacion." << endl;
    cout << "----------------------------------------------------------" << endl;
}

void muestraColasGraficamente(pColaPasajeros *co){
    int i = 0;
    Pasaporte pas;
    struct nodoPasajero *aux;
    aux = new struct nodoPasajero;
    //const char c = 254; // Caracter de un cuadrado
    cout << "----------------------------------------------------------" << endl;
    cout << "                       COLAS" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "1. Cola UE:  |" ;
    if (!EmptyQueue(co[0])){
        aux = co[0]->primero;
        aux = aux->sig;
        for ( i = 0; i < LengthQueue(co[0]); i++){
            pas = getPasaporte(aux->pasajero);
            cout << muestraPasaporte(pas) << " ";
            aux = aux->sig;
        }
    }
    cout << endl;

    cout << "2. Cola BR:  |";
    if (!EmptyQueue(co[1])){
        aux = co[1]->primero;
        aux = aux->sig;
        for ( i = 0; i < LengthQueue(co[1]); i++){
            pas = getPasaporte(aux->pasajero);
            cout << muestraPasaporte(pas) << " ";
            aux = aux->sig;
        }
    }
    cout << endl;

    cout << "3. Cola AUE: |";
    if (!EmptyQueue(co[2])){
        aux = co[2]->primero;
        aux = aux->sig;
        for ( i = 0; i < LengthQueue(co[2]); i++){
            pas = getPasaporte(aux->pasajero);
            cout << muestraPasaporte(pas) << " ";
            aux = aux->sig;
        }
    }
    cout << endl;

    cout << "4. Cola EC:  |";
    if (!EmptyQueue(co[3])){
        aux = co[3]->primero;
        aux = aux->sig;
        for ( i = 0; i < LengthQueue(co[3]); i++){
            pas = getPasaporte(aux->pasajero);
            cout << muestraPasaporte(pas) << " ";
            aux = aux->sig;
        }
    }
    cout << endl;

    cout << "----------------------------------------------------------" << endl;
}

/**
 * @brief Lee por teclado una opcion del menu principal. De no ser válida, devuelve -1.
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
 * @brief Función que, dado un pasaporte y una cola de pasajeros, dice si este pasaporte aparece en ella o no.
 * 
 * @param pas 
 * @param co 
 * @return true Se repite el pasaporte en la cola.
 * @return false 
 */
bool seRepitePasaporte(Pasaporte pas, pColaPasajeros *co){
    struct nodoPasajero* nP = new struct nodoPasajero;
    Pasajero p;
    
    for (int i = 0; i < MAX_COLAS; i++){
        if (!EmptyQueue(co[i])){
            nP = co[i]->primero;
            nP = nP->sig;
            while (nP != NULL){
                p = nP->pasajero;
                if (getPasaporte(p) == pas){
                    return true;
                }
                nP = nP->sig;
            }
        }
    }
    return false;
}

/**
 * @brief Pide una cola por teclado. De no ser válida, devuelve -1.
 * 
 * @return int 
 */
int pideCola(){
    string opcion;
    int opcionInt;
    cout << "Introduzca la cola: ";
    cin >> opcion;
    opcionInt = atoi(opcion.c_str());
    if (esColaValida(opcionInt)){
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

bool esColaValida(int nCola){
    return (nCola >= 1 && nCola <= MAX_COLAS);
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
