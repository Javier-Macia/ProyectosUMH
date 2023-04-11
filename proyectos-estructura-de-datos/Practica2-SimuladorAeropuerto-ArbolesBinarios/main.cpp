#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Pasajero.hpp"
#include "Menu.hpp"

using namespace std;

/**
* Programa hecho por Javier Macia Sempere
*
* Practica 2 de la asignatura de Estructuras de Datos de Ingenieria Informatica
* en Tecnologias de la Informacion de la UMH.
*
* Mayo de 2022
*/
int main(){
    srand(time(NULL));
    int opcion = 0;
    int nArbol = 0;
    int evento = 0;

    Pasajero p;
    Pasaporte pas;
    ArbolPasajeros nodoAux = CrearArbol();
    ArbolPasajeros a[MAX_ARBOLES];
    string mensaje = "";
    bool clean = true;

    // Creamos los arboles.
    for (int i = 0; i < MAX_ARBOLES; i++){
        a[i] = CrearArbol();
    }


    while (opcion != KEY_SALIR){
        if(clean == true){
            system("cls");
            muestraMenuPrincipal();
            muestraArbolesGraficamente(a);
        }

        if (mensaje != ""){
            cout << "[!] " << mensaje << endl;
            mensaje = "";
        }

        clean = true;
        opcion = pideOpcionMenuPrincipal();

        switch (opcion){
        case KEY_INTRODUCE_PASAJERO:
            p = pideDatosPasajero();
            nArbol = getTipo(p);
            if (InsertarValorArbol(a[nArbol], p)){
                mensaje = "El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                + "' ha sido introducido en el arbol numero " + to_string(nArbol + 1) + " correctamente.";
            }else{
                mensaje = "El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p)) + "' NO ha podido ser introducido en el arbol numero "
                + to_string(nArbol + 1) + " porque su pasaporte se repite.";
            }

            break;

        case KEY_SACA_PASAJERO:
            opcion = pideModoRetiradaPasajero();
            switch (opcion){
            case 1: // Aleatoriamente
                nArbol = pideArbol()-1;
                nodoAux = NULL;
                if (nArbol == -2){
                    mensaje = "El arbol no existe.";
                    break;
                }
                if (ArbolVacio(a[nArbol])){
                    mensaje = "El arbol numero " + to_string(nArbol + 1) + " esta vacio.";
                    break;
                }
                nodoAux = ApuntaPasajeroAleatorio(a[nArbol], rand());

                break;

            case 2: // Por numero de pasaporte
                pas = pidePasaporte();
                for (nArbol = 0; nArbol < MAX_ARBOLES; nArbol++){
                    if (ArbolVacio(a[nArbol])){
                        nodoAux = NULL;
                        continue;
                    }
                    nodoAux = ApuntarValorArbol(a[nArbol], pas);
                    if (nodoAux != NULL){
                        break;
                    }
                }
                if ( (nodoAux == NULL)){
                    mensaje = "El pasajero de pasaporte '" + muestraPasaporte(pas) + "' no existe en ninguno de los arboles.";
                    break;
                }
                break;

            case 3: // Retirando el nodo raiz
                nArbol = pideArbol() - 1;
                if (nArbol == -2){
                    mensaje = "El arbol no existe.";
                    break;
                }
                nodoAux = a[nArbol];
                if (nodoAux == NULL){
                    mensaje = "El arbol esta vacio.";
                    break;
                }

                break;
            default:
                mensaje = "Opcion no valida.";
                break;
            }
            if (opcion == -1) break;
            if (nodoAux == NULL) break;

            p = nodoAux->dato;

            if (getEstadoPasaporte(p) == false){
                p.estadoPasaporte = true;
                BorrarValorArbol(a[nArbol], p);
                InsertarValorArbol(a[nArbol], p);
                mensaje =  "El pasaporte '" + muestraPasaporte(getPasaporte(p)) +
                        "' del pasajero no esta vigente. Por ello, se ha reinsertado en el arbol " + to_string(nArbol) + ".";
                break;
            }else{
                BorrarValorArbol(a[nArbol], p);
                mensaje = "El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                        + "' ha sido sacado del arbol numero " + to_string(nArbol + 1) + " correctamente.";
                break;
            }

        case KEY_IMPRIMIR_ARBOL:
            nArbol = (pideArbol()) - 1;
            opcion = pideModoImprimirArbol();

            clean = false;
            system("cls");
            muestraMenuPrincipal();
            muestraArbolesGraficamente(a);

            if(nArbol == -1){
                mensaje = "El arbol no existe.";
                break;
            }

            if (ArbolVacio(a[nArbol])){
                mensaje = "El arbol esta vacio.";
                break;
            }

            cout << "[!] Arbol " << nArbol + 1 << ": "<< endl;
            recorreArbol(a[nArbol], opcion);
            cout << endl;
            break;

        case KEY_CONSULTAR_PASAJERO:
            nArbol = (pideArbol());
            pas = pidePasaporte();
            clean = false;
            system("cls");
            muestraMenuPrincipal();
            muestraArbolesGraficamente(a);

            if(nArbol == -1){
                mensaje = "El arbol no existe.";
                break;
            }

            if (ArbolVacio(a[nArbol-1])){
                mensaje = "El arbol esta vacio.";
                break;
            }

            nodoAux = ApuntarValorArbol(a[nArbol-1], pas);
            if (nodoAux == NULL){
                mensaje = "El pasaporte '" + muestraPasaporte(pas) + "' no existe en el arbol.";
            }else{
                cout << "[!] El pasajero del arbol "<< nArbol << " es: " << endl;
                muestraPasajero(nodoAux->dato);
            }

            cout << endl;
            break;
        case KEY_GENERAR_PASAJEROS:
            nArbol = pideArbol() - 1;
            if (nArbol == -2){
                mensaje = "El arbol no existe.";
                break;
            }
            opcion = pideNumeroPasajeros();
            if (opcion == -1){
                mensaje = "Numero de pasajeros no valido.";
                break;
            }

            for (int i = 0; i < opcion; i++){
                p = crearPasajeroAleatorio(rand(),nArbol);
                while (seRepitePasaporte(getPasaporte(p), a)){
                    p = crearPasajeroAleatorio(rand(),nArbol);
                }
                InsertarValorArbol(a[nArbol], p);
            }
            
            mensaje = "Pasajeros generados correctamente.";

            break;
        case KEY_BORRAR_PASAJEROS:
            nArbol = pideArbol() - 1;
            if (nArbol+1 == -1){
                mensaje = "El arbol no existe.";
                break;
            }
            opcion = pideNumeroPasajeros();
            if (opcion == -1){
                mensaje = "Numero de pasajeros no valido.";
                break;
            }

            for ( int i = 0; i < opcion; i++){
                if (ArbolVacio(a[nArbol])){
                    if (i == 0){
                        mensaje = "El arbol esta vacio.";
                    }
                    break;
                }
                p = a[nArbol]->dato;

                BorrarValorArbol(a[nArbol], p);
                mensaje = "Pasajero(s) eliminados correctamente.";
            }

            break;

        case KEY_SIMULACION:

            for (int i = 0; i < MAX_ARBOLES; i++){
                if(ArbolVacio(a[i])){
                    InsertarPasajeroAleatorioArbol(a[i], rand(), i);
                }
            }

            mensaje = "Cada arbol tiene minimo un pasajero para efectuar la simulacion. De no haberlo, se ha creado uno aleatorio.";

            for (int i = 0; i < 12; i++){
                system("cls");
                muestraMenuPrincipal();
                muestraArbolesGraficamente(a);
                if (mensaje != ""){
                    cout << "[!] " << mensaje << endl;
                    mensaje = "";
                }
                cout << endl;
                for (int j = 1; j <= 5; j++){
                    cout << ".";
                    Sleep(1000);
                }


                nArbol = rand() % MAX_ARBOLES;
                switch (evento){
                case 0: // Llega pasajero a una cola
                    p = crearPasajeroAleatorio(rand(),nArbol);
                    while (seRepitePasaporte(getPasaporte(p), a)){
                        p = crearPasajeroAleatorio(rand(),nArbol);
                    }

                    InsertarValorArbol(a[nArbol], p);
                    mensaje = "EVENTO: El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                        + "' ha sido introducido en el arbol numero " + to_string(nArbol + 1) + ".";

                    evento = 1;
                    break;

                case 1: // Sale pasajero de una cola
                    while (ArbolVacio(a[nArbol])){ // Si esta vacia, se generan numeros aleatorios hasta dar con una que tenga gente.
                        nArbol = rand() % MAX_ARBOLES;
                    }

                    if (a[nArbol]->dato.estadoPasaporte == false){ // Si el pasaporte no esta vigente, se sale y vuelve a entrar.
                        p = a[nArbol]->dato;
                        BorrarValorArbol(a[nArbol], p);
                        p.estadoPasaporte = true;
                        InsertarValorArbol(a[nArbol], p);
                        mensaje = "EVENTO: El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                            + "' se ha reintroducido en el arbol " + to_string(nArbol + 1) + " con el pasaporte vigente.";
                    }

                    else{
                        p = a[nArbol]->dato;
                        BorrarValorArbol(a[nArbol], p);
                        mensaje = "EVENTO: El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                            + "' ha sido sacado del arbol " + to_string(nArbol + 1) + ".";
                    }
                    evento = 0;
                    break;

                default:
                    break;
                }
            }


            break;
        case KEY_SALIR:
            cout << "Gracias por usar la aplicacion. Vuelva pronto!" << endl;
            break;

        default:
            mensaje = "No se ha entendido la orden. Intentelo de nuevo.";
            break;
        }
    }
    return 0;
}
