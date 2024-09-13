#include "procesos.h"
#include <stdio.h>
#include <stdlib.h>



#define Q 4
#define TI (float)1

Proceso procesos[] = {
    {"PA\0",15, 9, 0, 0},
    {"PB\0",29, 3, 0, 0},
    {"PC\0",18, 14, 0, 0},
    {"ENDP\0",0,0,0,0} // utilizo el PID 0 como corte para la carga de procesos
};

ColaProcesos* colaReady = NULL;
float reloj = 0;


float sumaTiempoRetorno(ColaProcesos* c);
void printCola(ColaProcesos* c);

int cargarTodosLosProcesos(){
    colaReady = crearColaProcesos();
    if (colaReady == NULL) return -1;
    for(int i = 0; procesos[i].pid != 0; i++ ){
        agregarProceso(colaReady, procesos[i]);
    }
    return colaReady->cantElementos;
}


int main (){
    cargarTodosLosProcesos();
    Proceso p;
    ColaProcesos* colaTerminados = crearColaProcesos();
    float tiempoEspera = 0;
    char* evento = malloc(sizeof(char)*50);

    printf("%-8s%-50s%-20s%-8s\n", "RELOJ", "EVENTO", "PROCESO", "PID");
    while (!esVacia(colaReady)) {
        p = popProceso(colaReady);
        printf("%5.2f %-50s %-20s %-8d\n", reloj, "Rutina 2 Ready --> Run", p.descripcion, p.pid);
        tiempoEspera += (TI/2) * getCantidadProcesos(colaReady);
        reloj = reloj + TI/2;

        if (esVacia(colaReady)){
            sprintf(evento, "Se ejecuta por %d tiempo", p.ts);
            printf("\033[32m%5.2f %-50s %-20s %-8d\n\033[0m", reloj, evento, p.descripcion, p.pid);
            //printf("\033[32m\033[1m Instante %.2f:\033[0m Se ejecuta por %d tiempo el proceso: %s | PID: %d\n",reloj, p.ts, p.descripcion, p.pid );
            reloj += p.ts;
            p.ts = 0;

            printf("\033[36m%5.2f %-50s %-20s %-8d\n\033[0m", reloj, "Rutina 6 Run --> Terminado", p.descripcion, p.pid);
            //printf("\033[31m\033[1m Instante %.2f:\033[0m Rutina 6 Run --> Terminado para proceso %s | PID: %d\n",reloj, p.descripcion, p.pid);
            p.retorno = reloj;
            agregarProceso(colaTerminados, p);
            reloj += TI/2;
            
        }
        else{
            if (p.ts > Q){
                sprintf(evento, "Se ejecuta por %d tiempo", Q);
                printf("\033[32m%5.2f %-50s %-20s %-8d\n\033[0m", reloj, evento, p.descripcion, p.pid);
                //printf("\033[32m\033[1m Instante %.2f:\033[0m Se ejecuta por %d tiempo el proceso %s | PID: %d\n", reloj, Q, p.descripcion, p.pid);
                reloj += Q;
                p.ts -= Q;

                printf("\033[36m%5.2f %-50s %-20s %-8d\n\033[0m", reloj, "Rutina 3 Run --> Ready interrupcion por quantum", p.descripcion, p.pid);
                //printf("\033[31m\033[1m Instante %.2f:\033[0m Rutina 3 Run --> Ready interrupcion por quantum para proceso %s | PID: %d\n",reloj, p.descripcion, p.pid);
                reloj += TI/2;
                tiempoEspera += (Q + TI/2) * getCantidadProcesos(colaReady);
                agregarProceso(colaReady, p);
                
            }
            else{
                sprintf(evento, "Se ejecuta por %d tiempo", p.ts);
                printf("\033[32m%5.2f %-50s %-20s %-8d\n\033[0m", reloj, evento, p.descripcion, p.pid);
                //printf("\033[32m\033[1m Instante %.2f:\033[0m Se ejecuta por %d tiempo el proceso %s | PID: %d\n",reloj, p.ts, p.descripcion, p.pid);
                reloj += p.ts;
                p.ts = 0;

                printf("\033[36m%5.2f %-50s %-20s %-8d\n\033[0m", reloj, "Rutina 6 Run --> Terminado", p.descripcion, p.pid);
                //printf("\033[31m\033[1m Instante %.2f:\033[0m Rutina 6 Run --> Terminado para proceso %s | PID: %d \n",reloj, p.descripcion, p.pid);
                p.retorno = reloj;
                reloj += TI/2;
                tiempoEspera += (p.ts + TI/2) * getCantidadProcesos(colaReady);
                agregarProceso(colaTerminados, p);
                
            }
        }

        

        
    }
    printf("---------------------RESULTADOS---------------------\n");
    printf("Procesos terminados:\n");
    printCola(colaTerminados);
    printf("----------------------------------------------------\n");
    printf("Reloj = %.2f\n", reloj);
    printf("Tiempo de espera promedio: %.2f\n", tiempoEspera/getCantidadProcesos(colaTerminados));
    printf("Tiempo de retorno promedio: %.2f\n", sumaTiempoRetorno(colaTerminados)/getCantidadProcesos(colaTerminados));
    destruir(colaTerminados);
    destruir(colaReady);
}

float sumaTiempoRetorno(ColaProcesos* c){
    if (esVacia(c)){
        return 0;
    }
    else{
        Proceso p = popProceso(c);
        float aux = sumaTiempoRetorno(c);
        agregarProceso(c, p);
        return aux + p.retorno;
    }

}

void printCola(ColaProcesos* c){
    Proceso p;
    
    if(!esVacia(c))
    {
        p = popProceso(c);
        printCola(c);
        printf("%-50s %-8d %-8d %7.2f %7.2f\n", p.descripcion, p.pid, p.ts, p.llegada, p.retorno);
        agregarProceso(c, p);
    }
    else{
        printf("%s%-50s %-8s %-8s %-9s %-9s%s\n", "\033[32m\033[1m", "Proceso", "PID", "TS", "Llegada", "Retorno", "\033[0m");
    }
    
}


