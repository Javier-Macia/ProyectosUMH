#include <iostream>
#include <time.h>

#include "ArbolPasajeros.hpp"

using namespace std;

/*******************************
 * NO VISIBLE PARA EL USUARIO *
 ******************************************************************************/
/*                               COLA DE NODOS                                */
/******************************************************************************/
typedef struct nodoColaArbol{
    ArbolPasajeros dato;
    struct nodoColaArbol *sig;
}*pnodoColaArbol;

typedef struct ColaArbol{
    struct nodoColaArbol *primero;
    struct nodoColaArbol *ultimo;
}*pColaArbol;


pColaArbol CreateQueue(){
    pColaArbol co = new struct ColaArbol;
    co->primero = new struct nodoColaArbol;
    co->ultimo = new struct nodoColaArbol;

    co->ultimo = NULL;
    co->primero->sig = NULL;
    return (co);
}

bool EmptyQueue(pColaArbol co){
    return (co->primero->sig == NULL);
}

void InQueue (pColaArbol co, ArbolPasajeros n){
    struct nodoColaArbol *nodo_aux;
    nodo_aux = new struct nodoColaArbol;
    nodo_aux->dato = n;

    if (EmptyQueue(co)){
        nodo_aux->sig = NULL;
        co->primero->sig = nodo_aux;
        co->ultimo = nodo_aux;
    }else{
        co->ultimo->sig = nodo_aux;
        co->ultimo = nodo_aux;
        co->ultimo->sig = NULL;
    }
}

ArbolPasajeros OutQueue(pColaArbol co){
    struct nodoColaArbol *nodo_aux;
    nodo_aux = new struct nodoColaArbol;
    ArbolPasajeros r;

    if(EmptyQueue(co) == true){
        return NULL;
    }else{
        nodo_aux = co->primero->sig;
        co->primero->sig = nodo_aux->sig; //primero->sig->sig;
        r = nodo_aux->dato;
        delete(nodo_aux);
        return r;
    }
}

ArbolPasajeros FirstQueue(pColaArbol co){
    return co->primero->sig->dato;
}

ArbolPasajeros LastQueue(pColaArbol co){
    return co->ultimo->dato;
}
/*******************************
 * NO VISIBLE PARA EL USUARIO *
 ******************************************************************************/
/*                               COLA DE NODOS                                */
/******************************************************************************/


/******************************************************************************/
/*                                  ARBOLES                                   */
/******************************************************************************/

ArbolPasajeros CrearArbol(){
    return NULL;
}

bool ArbolVacio(ArbolPasajeros a){
    return (a == NULL);
}

int GradoArbol(ArbolPasajeros a){
    if(ArbolVacio(a)){
        return 0;
    }
    int grado = 1;
    if (a->izq != NULL)
        grado = grado + GradoArbol(a->izq);
    if (a->der != NULL)
        grado = grado + GradoArbol(a->der);
    return grado;
}

int AlturaArbol(ArbolPasajeros a){
    if(ArbolVacio(a))
        return 0;

    int alturaIzq = AlturaArbol(a->izq);
    int alturaDer = AlturaArbol(a->der);
    if(alturaIzq > alturaDer){
        return alturaIzq + 1;
    }else{
        return alturaDer + 1;
    }
}

bool InsertarValorArbol(ArbolPasajeros &a, Pasajero dato){
    if(ArbolVacio(a)){
        struct nodoArbol *nodo_aux;
        nodo_aux = new struct nodoArbol;
        nodo_aux->dato = dato;
        nodo_aux->izq = NULL;
        nodo_aux->der = NULL;
        nodo_aux->padre = NULL;
        a = nodo_aux;
        return true;
    }

    if(dato < a->dato){
        if(a->izq == NULL){
            a->izq = new nodoArbol;
            a->izq->dato = dato;
            a->izq->izq = NULL;
            a->izq->der = NULL;
            a->izq->padre = a;
            return true;
        }
        else{
            return InsertarValorArbol(a->izq, dato);
        }
    }
    else if(dato > a->dato){
        if(a->der == NULL){
            a->der = new nodoArbol;
            a->der->dato = dato;
            a->der->izq = NULL;
            a->der->der = NULL;
            a->der->padre = a;
            return true;
        }
        else{
            return InsertarValorArbol(a->der, dato);
        }
    }
    return false; // Caso en el que dato == a->dato

}

/**
 * @brief Busca un nodo en el Arbol de Pasajeros. Devuelve un nodo si lo encuentra, y NULL si no
 *
 * @param a
 * @param dato
 * @return ArbolPasajeros si lo encuentra, NULL si no.
 */
ArbolPasajeros ApuntarValorArbol(ArbolPasajeros a, Pasajero dato){
    if(ArbolVacio(a)){
        return NULL;
    }
    if(a->dato == dato){
        return a;
    }
    else{
        if(dato < a->dato){
            return ApuntarValorArbol(a->izq, dato);
        }
        else{
            return ApuntarValorArbol(a->der, dato);
        }
    }
}

/**
 * @brief Busca un pasaporte en el Arbol de Pasajeros. Devuelve un nodo si lo encuentra, y NULL si no
 *
 * @param a
 * @param dato
 * @return ArbolPasajeros si lo encuentra, NULL si no.
 */
ArbolPasajeros ApuntarValorArbol(ArbolPasajeros a, Pasaporte dato){
    if(ArbolVacio(a)){
        return NULL;
    }
    if(a->dato.pasaporte == dato){
        return a;
    }
    else{
        if(dato < a->dato.pasaporte){
            return ApuntarValorArbol(a->izq, dato);
        }
        else{
            return ApuntarValorArbol(a->der, dato);
        }
    }
}

bool BusquedaValorArbol(ArbolPasajeros a, Pasajero dato){
    if(ApuntarValorArbol(a,dato) == NULL) // dato no encontrado
        return false;
    else
        return true;
}

void RecorridoEntreOrdenArbolSimple(ArbolPasajeros a){
    if(ArbolVacio(a)){
        return;
    }
    if (a->izq != NULL)
        RecorridoEntreOrdenArbolSimple(a->izq);
    cout << muestraPasaporte(getPasaporte(a->dato)) << " ";
    if (a->der != NULL)
        RecorridoEntreOrdenArbolSimple(a->der);
}

void RecorridoAnchuraArbol(ArbolPasajeros a){
    // cout << "Recorrido en anchura: " << endl;;
    if(ArbolVacio(a)){
        cout << "Arbol vacio" << endl;
        return;
    }

    pColaArbol co = CreateQueue();
    ArbolPasajeros aux;
    InQueue(co, a);

    while(EmptyQueue(co) == false){
        aux = OutQueue(co);
        muestraPasajero(aux->dato);
        cout << endl;
        if(aux->izq != NULL)
            InQueue(co, aux->izq);
        if(aux->der != NULL)
            InQueue(co, aux->der);
    }
}

void PreOrdenArbol(ArbolPasajeros a){
    /*
    if(a->padre == NULL){
        cout << "Recorrido en preorden: " << endl;
    }
    */
    if(ArbolVacio(a)){
        return;
    }
    muestraPasajero(a->dato);
    cout << endl;
    if (a->izq != NULL)
        PreOrdenArbol(a->izq);
    if (a->der != NULL)
        PreOrdenArbol(a->der);
}

void EntreOrdenArbol(ArbolPasajeros a){
    /*
    if (a->padre == NULL){
        cout << "Recorrido en entreorden: " << endl;
    }
    */
    if(ArbolVacio(a)){
        return;
    }
    if (a->izq != NULL)
        EntreOrdenArbol(a->izq);
    muestraPasajero(a->dato);
    cout << endl;
    if (a->der != NULL)
        EntreOrdenArbol(a->der);
}

void PostOrdenArbol(ArbolPasajeros a){
    /*
    if (a->padre == NULL){
        cout << "Recorrido en postorden: " << endl;
    }
    */
    if(ArbolVacio(a)){
        return;
    }
    if (a->izq != NULL)
        PostOrdenArbol(a->izq);
    if (a->der != NULL)
        PostOrdenArbol(a->der);
    muestraPasajero(a->dato);
    cout << endl;
}

ArbolPasajeros getmin( ArbolPasajeros root){
    ArbolPasajeros curr = root;

    while (curr && curr->izq) {
        curr = curr->izq;
    }

    return curr;
}

bool BorrarValorArbol(ArbolPasajeros& a, Pasajero dato){
    if(ArbolVacio(a)){
        return false;
    }
    if(a->dato == dato){
        if(a->izq == NULL && a->der == NULL){
            delete a;
            a = NULL;
            return true;
        }
        else if(a->izq == NULL){
            ArbolPasajeros aux = a;
            a = a->der;
            a->padre = aux->padre;
            delete aux;
            return true;
        }
        else if(a->der == NULL){
            ArbolPasajeros aux = a;
            a = a->izq;
            a->padre = aux->padre;
            delete aux;
            return true;
        }
        else{
            ArbolPasajeros aux = getmin(a->der);
            a->dato = aux->dato;
            BorrarValorArbol(a->der, aux->dato);
            return true;
        }
    }
    else if(dato < a->dato){
        return BorrarValorArbol(a->izq, dato);
    }
    else if(dato > a->dato){
        return BorrarValorArbol(a->der, dato);
    }
    return false;
}

void BorrarArbol(ArbolPasajeros &a){
    if(!ArbolVacio(a->izq))
        BorrarArbol(a->izq);
    if(!ArbolVacio(a->der))
        BorrarArbol(a->der);
    a = NULL;
    delete (a);
}

void BorrarValorSubArbol(ArbolPasajeros &a, Pasajero dato){
    ArbolPasajeros nodo_borrar = ApuntarValorArbol(a,dato);
    BorrarArbol(nodo_borrar);
    a = NULL;
}

bool ArbolesIguales(ArbolPasajeros a1, ArbolPasajeros a2){
    if(ArbolVacio(a1) && ArbolVacio(a2))
        return true;

    // Si tienen grados diferentes, seguro que no son iguales. No hace falta que recorramos el grado
    // de todo el ArbolPasajeros en cada iteracion de la funcion recursiva, de ah� la condici�n.
    if ( (a1->padre == NULL) && (a2->padre == NULL) ){
        if (GradoArbol(a1) != GradoArbol(a2))
            return false;
    }

    if (a1->dato == a2->dato)
        return ArbolesIguales(a1->izq, a2->izq) && ArbolesIguales(a1->der, a2->der);
    else return false;
}

void InsertarPasajeroAleatorioArbol(ArbolPasajeros &a, int seed, int tipo){
    srand(seed);
    InsertarValorArbol(a,crearPasajeroAleatorio(seed,tipo));
}

void ImprimirArbol(ArbolPasajeros a){
    if(ArbolVacio(a)){
        return;
    }
    pColaArbol co = CreateQueue();
    ArbolPasajeros aux;
    InQueue(co, a);

    while(EmptyQueue(co) == false){
        aux = OutQueue(co);
        muestraPasajero(aux->dato);
        cout << endl;
        if(aux->izq != NULL)
            InQueue(co, aux->izq);
        if(aux->der != NULL)
            InQueue(co, aux->der);
    }
}

ArbolPasajeros ApuntaPasajeroAleatorio(ArbolPasajeros &a, int seed){
    if (ArbolVacio(a)){
        return NULL;
    }
    srand(seed);
    int num = rand() % GradoArbol(a) + 1;
    int decision = rand() % 2;

    cout << "num: " << num << endl;

    ArbolPasajeros aux = a;

    for (int i = 0; i < num; i++){
        if( aux->izq != NULL && decision == 0){
            aux = aux->izq;
        }
        else if( aux->der != NULL && decision == 1){
            aux = aux->der;
        }
        else{
            if (aux->padre == NULL)
                return aux;
            
            aux = aux->padre;
            break;
        }
    }
    return aux;
}

/******************************************************************************/
/*                                  ARBOLES                                   */
/******************************************************************************/
