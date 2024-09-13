#ifndef PROCESOS_H
#define PROCESOS_H

typedef struct Proceso{
    char descripcion[20];
    unsigned int pid;
    unsigned int ts;
    float llegada;
    float retorno;
}Proceso;

typedef struct Nodo{
    Proceso p;
    struct Nodo* ant;
    struct Nodo* sig;
}Nodo;

typedef struct ColaProcesos{
    Nodo* cabeza;
    Nodo* cola;
    unsigned int cantElementos;
}ColaProcesos;

ColaProcesos* crearColaProcesos();
int agregarProceso(ColaProcesos* c, Proceso p);
Proceso popProceso(ColaProcesos* c);
int getCantidadProcesos(ColaProcesos* c);
int esVacia(ColaProcesos* c);
void destruir(ColaProcesos* c);


#endif

