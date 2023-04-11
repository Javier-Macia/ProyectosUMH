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
* Practica 1 de la asignatura de Estructuras de Datos de Ingenieria Informatica
* en Tecnologias de la Informacion de la UMH.
*
* Abril de 2022
*/
int main(){
    srand(time(NULL));
    int opcion = 0;
    int nCola = 0;
    int evento = 0;

    Pasajero p;
    pColaPasajeros co[MAX_COLAS];
    string mensaje = "";
    bool clean = true;

    // Creamos las colas.
    for (int i = 0; i < MAX_COLAS; i++){
        co[i] = CreateQueue();
    }

    while (opcion != KEY_SALIR){
        if(clean == true){
            system("cls");
            muestraMenuPrincipal();
            muestraColasGraficamente(co);
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
            nCola = getTipo(p);
            if (seRepitePasaporte(getPasaporte(p), co)){
                mensaje = "El pasajero con el pasaporte " + muestraPasaporte(getPasaporte(p)) + " ya existe en una cola. Por ello, no se ha introducido.";
            }else{
                InsertQueue(co[nCola], p);
                mensaje = "El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                    + "' ha sido introducido en la cola numero " + to_string(nCola + 1) + " correctamente.";
            }

            break;

        case KEY_SACA_PASAJERO:
            nCola = (pideCola());

            if(nCola == -1){
                mensaje =  "La cola no existe.";
                break;
            }

            if (EmptyQueue(co[nCola-1])){
                mensaje =  "La cola esta vacia.";
                break;
            }

            p = FirstQueue(co[nCola-1]);

            if (getEstadoPasaporte(p) == false){
                p.estadoPasaporte = true;
                OutQueue(co[nCola-1]);
                InsertQueue(co[nCola-1], p);
                mensaje =  "El pasaporte '" + muestraPasaporte(getPasaporte(p)) +
                        "' del pasajero no esta vigente. Por ello, se ha reinsertado en la cola " + to_string(nCola) + ".";
                break;
            }else{
                OutQueue(co[nCola-1]);
                mensaje = "El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                        + "' ha sido sacado de la cola numero " + to_string(nCola + 1) + " correctamente.";
                break;
            }

        case KEY_IMPRIMIR_COLA:
            nCola = (pideCola());
            clean = false;
            system("cls");
            muestraMenuPrincipal();
            muestraColasGraficamente(co);

            if(nCola == -1){
                mensaje = "La cola no existe.";
                break;
            }

            if (EmptyQueue(co[nCola-1])){
                mensaje = "La cola esta vacia.";
                break;
            }

            cout << "[!] Cola " << nCola << ": "<< endl;
            ViewQueue(co[nCola-1]);
            cout << endl;
            break;

        case KEY_CONSULTAR_PASAJERO:
            nCola = (pideCola());
            clean = false;
            system("cls");
            muestraMenuPrincipal();
            muestraColasGraficamente(co);

            if(nCola == -1){
                mensaje = "La cola no existe.";
                break;
            }

            if (EmptyQueue(co[nCola-1])){
                mensaje = "La cola esta vacia.";
                break;
            }

            cout << "[!] El pasajero de la cola "<< nCola << " es: " << endl;
            muestraPasajero(FirstQueue(co[nCola-1]));
            cout << endl;
            break;

        case KEY_SIMULACION:
            for (int i = 0; i < MAX_COLAS; i++){
                if(EmptyQueue(co[i])){
                    p = crearPasajeroAleatorio(rand(), i);
                    InsertQueue(co[i], p);
                }
            }
            
            mensaje = "Cada cola tiene minimo un pasajero para efectuar la simulacion. De no haberlo, se ha creado uno aleatorio.";

            for (int i = 0; i < 12; i++){
                system("cls");
                muestraMenuPrincipal();
                muestraColasGraficamente(co);
                if (mensaje != ""){
                    cout << "[!] " << mensaje << endl;
                    mensaje = "";
                }
                cout << endl;
                for (int j = 1; j <= 5; j++){
                    cout << ".";
                    Sleep(1000);
                }
                
                
                nCola = rand() % MAX_COLAS;
                switch (evento){
                case 0: // Llega pasajero a una cola
                    p = crearPasajeroAleatorio(rand(),nCola);
                    while (seRepitePasaporte(getPasaporte(p), co)){
                        p = crearPasajeroAleatorio(rand(),nCola);
                    }

                    InsertQueue(co[nCola], p);
                    mensaje = "EVENTO: El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                        + "' ha sido introducido en la cola numero " + to_string(nCola + 1) + ".";

                    evento = 1;
                    break;

                case 1: // Sale pasajero de una cola
                    while (EmptyQueue(co[nCola])){ // Si está vacía, se generan numeros aleatorios hasta dar con una que tenga gente.
                        nCola = rand() % MAX_COLAS;
                    }

                    if (co[nCola]->primero->pasajero.estadoPasaporte == false){ // Si el pasaporte no está vigente, se sale y vuelve a entrar.
                        p = FirstQueue(co[nCola]);
                        p.estadoPasaporte = true;
                        OutQueue(co[nCola]);
                        InsertQueue(co[nCola],p);
                        mensaje = "EVENTO: El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                            + "' se ha reintroducido en la cola " + to_string(nCola + 1) + " con el pasaporte vigente.";
                    }

                    else{
                        p = FirstQueue(co[nCola]);
                        OutQueue(co[nCola]);
                        mensaje = "EVENTO: El pasajero de pasaporte '" + muestraPasaporte(getPasaporte(p))
                            + "' ha sido sacado de la cola " + to_string(nCola + 1) + ".";
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
