#include "procesos.h"
#include <stdlib.h>



ColaProcesos* crearColaProcesos(){
    ColaProcesos* c = malloc(sizeof(ColaProcesos));
    if (c != NULL){
        c->cabeza = NULL;
        c->cola = NULL;
        c->cantElementos = 0;
    }
    return c;
}
int agregarProceso(ColaProcesos* c, Proceso p){
    Nodo* nodo = malloc(sizeof(Nodo));
    if (nodo == NULL) return -1;
    
    if (esVacia(c)){
        nodo->p = p;
        nodo->sig = NULL;
        nodo->ant = NULL;

        c->cabeza = nodo;
        c->cola = nodo;
    }
    else{
        nodo->p = p;
        nodo->sig = NULL;
        nodo->ant = c->cola;

        c->cola->sig = nodo;
        c->cola = nodo;
        
    }
    c->cantElementos++;
    return 0;
}
Proceso popProceso(ColaProcesos* c){
    Proceso p;
    if (!esVacia(c)){
        p = c->cabeza->p;
        if (c->cantElementos > 1){
                        
            c->cabeza = c->cabeza->sig;
            free(c->cabeza->ant);
            c->cabeza->ant = NULL;
        }
        else{
            c->cabeza = NULL;
            c->cola = NULL;
        }
        c->cantElementos--;
    }
    return p;
}
int getCantidadProcesos(ColaProcesos* c){
    return c->cantElementos;
}
int esVacia(ColaProcesos* c){
    return !c->cantElementos;
}
void destruir(ColaProcesos* c){
    if (!esVacia(c)){
        popProceso(c);
        destruir(c);
    }
    else{
        free(c);
    }
}